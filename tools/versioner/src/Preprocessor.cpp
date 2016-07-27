/*
 * Copyright 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Preprocessor.h"

#include <err.h>
#include <fcntl.h>
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <deque>
#include <fstream>
#include <string>
#include <unordered_map>

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/Twine.h>

#include "Arch.h"
#include "DeclarationDatabase.h"

using namespace std::string_literals;

static DeclarationAvailability calculateMacroGuard(const Declaration& declaration) {
  // To avoid redundant macro guards, the availability calculated by this function is the set
  // difference of 'targets marked-available' from 'targets the declaration is visible in'.
  // For example, a declaration that is visible always and introduced in 9 would return introduced
  // in 9, but the same declaration, except only visible in 9+ would return an empty
  // DeclarationAvailability.

  // TODO: Do this for more than just introduced.
  int global_min_api_visible = 0;
  ArchMap<int> arch_min_api;

  for (const auto& it : declaration.availability) {
    const CompilationType& type = it.first;

    if (global_min_api_visible == 0 || global_min_api_visible > type.api_level) {
      global_min_api_visible = type.api_level;
    }

    if (arch_min_api[type.arch] == 0 || arch_min_api[type.arch] > type.api_level) {
      arch_min_api[type.arch] = type.api_level;
    }
  }

  DeclarationAvailability decl_av;
  if (!declaration.calculateAvailability(&decl_av)) {
    fprintf(stderr, "versioner: failed to calculate availability for while preprocessing:\n");
    declaration.dump("", std::cerr, 2);
    exit(1);
  }

  DeclarationAvailability result;

  if (decl_av.global_availability.introduced > global_min_api_visible) {
    result.global_availability.introduced = decl_av.global_availability.introduced;
  }

  for (Arch arch : supported_archs) {
    if (decl_av.arch_availability[arch].introduced > arch_min_api[arch]) {
      result.arch_availability[arch].introduced = decl_av.arch_availability[arch].introduced;
    }
  }

  return result;
}

static std::deque<std::string> readFileLines(const std::string& path) {
  std::ifstream is(path.c_str());
  std::deque<std::string> result;
  std::string line;

  while (std::getline(is, line)) {
    result.push_back(std::move(line));
  }

  return result;
}

static std::string dirname(const std::string& path) {
  std::unique_ptr<char, decltype(&free)> path_copy(strdup(path.c_str()), free);
  return dirname(path_copy.get());
}

static bool mkdirs(const std::string& path) {
  struct stat st;
  if (stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
    return true;
  }

  std::string parent = dirname(path);
  if (parent == path) {
    return false;
  }

  if (!mkdirs(parent)) {
    return false;
  }

  if (mkdir(path.c_str(), 0700) != 0) {
    return false;
  }

  return true;
}

static void writeFileLines(const std::string& path, const std::deque<std::string>& lines) {
  if (!mkdirs(dirname(path))) {
    err(1, "failed to create directory '%s'", dirname(path).c_str());
  }

  std::ofstream os(path.c_str(), std::ios_base::out | std::ios_base::trunc);

  for (const std::string& line : lines) {
    os << line << "\n";
  }
}

using GuardMap = std::map<Location, DeclarationAvailability>;

static std::string generateGuardCondition(const DeclarationAvailability& avail) {
  return "1";
}

// Assumes that nothing crazy is happening (e.g. having the semicolon be in a macro)
static FileLocation findNextSemicolon(const std::deque<std::string>& lines, FileLocation start) {
  unsigned current_line = start.line;
  unsigned current_column = start.column;
  while (current_line < lines.size()) {
    size_t result = lines[current_line - 1].find_first_of(';', current_column - 1);

    if (result != std::string::npos) {
      FileLocation loc = {
        .line = current_line,
        .column = unsigned(result) + 1,
      };

      return loc;
    }

    ++current_line;
    current_column = 0;
  }

  errx(1, "failed to find semicolon");
}

static void rewriteFile(llvm::StringRef file_path, llvm::StringRef output_dir,
                        llvm::StringRef input_dir, const GuardMap& guard_map) {
  if (!file_path.startswith(input_dir)) {
    errx(1, "input file %s is not in %s\n", file_path.str().c_str(), input_dir.str().c_str());
  }

  // rel_path has a leading slash.
  llvm::StringRef rel_path = file_path.substr(input_dir.size(), file_path.size() - input_dir.size());
  std::string output_path = (llvm::Twine(output_dir) + rel_path).str();

  std::deque<std::string> file_lines = readFileLines(file_path.str());

  for (auto it = guard_map.rbegin(); it != guard_map.rend(); ++it) {
    const Location& loc = it->first;
    const DeclarationAvailability& avail = it->second;
    printf("Adding guard at %s:[%u:%u-%u-%u]\n", loc.filename.c_str(), loc.start.line,
           loc.start.column, loc.end.line, loc.end.column);

    std::string condition = generateGuardCondition(avail);
    std::string prologue = "\n#if "s + condition + "\n";
    std::string epilogue = "\n#endif /* " + condition + " */\n\n";

    FileLocation semi_loc = findNextSemicolon(file_lines, loc.end);
    file_lines[semi_loc.line - 1].insert(semi_loc.column, epilogue);
    file_lines[loc.start.line - 1].insert(loc.start.column - 1, prologue);
  }

  writeFileLines(output_path, file_lines);
}

bool preprocessHeaders(const std::string& preprocessor_output_path, const std::string& source_dir,
                       HeaderDatabase* database) {
  std::unordered_map<std::string, GuardMap> guards;

  for (const auto& symbol_it : database->symbols) {
    const std::string& symbol_name = symbol_it.first;
    const Symbol& symbol = symbol_it.second;

    for (const auto& decl_it : symbol.declarations) {
      const Location& location = decl_it.first;
      const Declaration& decl = decl_it.second;

      DeclarationAvailability macro_guard = calculateMacroGuard(decl);
      if (!macro_guard.empty()) {
        printf("Inserting macro guard (%s) around %s at %s\n", to_string(macro_guard).c_str(),
               symbol_name.c_str(), to_string(location).c_str());
        guards[location.filename][location] = macro_guard;
      }
    }
  }

  for (const auto& file_it : guards) {
    const std::string& filename = file_it.first;
    const GuardMap& guard_map = file_it.second;

    // TODO: Merge adjacent identical guards.
    // TODO: Merge adjacent non-identical guards.
    // TODO: Make sure that the Locations don't overlap.

    rewriteFile(filename, preprocessor_output_path, source_dir, guard_map);
  }

  return true;
}
