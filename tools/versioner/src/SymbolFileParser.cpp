/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SymbolFileParser.h"

#include <android-base/strings.h>

#include <fstream>
#include <string>
#include <unordered_map>

#include <iostream>
#include <err.h>

namespace {

using TagList = std::vector<std::string>;

struct SymbolEnt {
  std::string name;
  TagList tags;
};

using SymbolList = std::vector<SymbolEnt>;

struct Version {
 public:
  std::string name;
  std::string base;
  SymbolList symbols;
  TagList tags;
};

class SymbolFileParser {
 public:
  SymbolFileParser(const std::string& path, const CompilationType& type)
    : file_path(path), compilation_type(type), file(path, std::ios_base::in),
      curr_line_num(0) {
  }

  std::optional<SymbolMap> parse() {
    if (!file) {
      return std::optional<SymbolMap>();
    }

    SymbolMap symbol_map;
    while (hasNextLine()) {
      auto&& version_opt = parseVersion();
      if (!version_opt) {
        return std::optional<SymbolMap>();
      }

      auto&& version = *version_opt;
      if (isInArch(version.tags) && isInApi(version.tags)) {
        for (auto&& symbol : version.symbols) {
          if (isInArch(symbol.tags) && isInApi(symbol.tags)) {
            symbol_map[symbol.name] = getSymbolType(symbol.tags);
          }
        }
      }
    }
    return symbol_map;
  }

 private:
  bool hasNextLine() {
    std::string line;
    while (std::getline(file, line)) {
      ++curr_line_num;

      size_t hash_pos = line.find('#');
      curr_line = android::base::Trim(line.substr(0, hash_pos));
      if (!curr_line.empty()) {
        if (hash_pos != std::string::npos) {
          curr_tags = parseTags(line.substr(hash_pos + 1));
        } else {
          curr_tags = std::vector<std::string>();
        }
        return true;
      }
    }
    return false;
  }

  static std::vector<std::string> parseTags(const std::string& tags_line) {
    std::vector<std::string> tags = android::base::Split(tags_line, " \t");
    tags.erase(std::remove(tags.begin(), tags.end(), ""), tags.end());
    return tags;
  }

  std::optional<Version> parseVersion() {
    size_t start_line_num = curr_line_num;

    std::string::size_type lparen = curr_line.find('{');
    if (lparen == std::string::npos) {
      errx(1, "%s:%zu: error: expected '{' cannot be found in this line",
           file_path.c_str(), curr_line_num);
    }

    std::string name = android::base::Trim(curr_line.substr(0, lparen));
    TagList tags = std::move(curr_tags);
    SymbolList symbols;

    bool global_scope = true;
    bool cpp_scope = false;

    while (hasNextLine()) {
      size_t rparen_pos = curr_line.find('}');
      if (rparen_pos != std::string::npos) {
        size_t semicolon_pos = curr_line.find(';', rparen_pos + 1);
        if (semicolon_pos == std::string::npos) {
          errx(1, "%s:%zu: error: the line that ends a scope must end with ';'",
               file_path.c_str(), curr_line_num);
        }

        if (cpp_scope) {
          cpp_scope = false;
          continue;
        }

        std::string base = android::base::Trim(
          curr_line.substr(rparen_pos + 1, semicolon_pos - 1));
        return std::make_optional(Version{name, base, symbols, tags});
      }

      if (android::base::StartsWith(curr_line, R"(extern "C++" {)")) {
        cpp_scope = true;
        continue;
      }

      if (cpp_scope) {
        continue;
      }

      size_t colon_pos = curr_line.find(':');
      if (colon_pos != std::string::npos) {
        std::string visibility =
          android::base::Trim(curr_line.substr(0, colon_pos));
        if (visibility == "global") {
          global_scope = true;
          continue;
        }
        if (visibility == "local") {
          global_scope = false;
          continue;
        }
        errx(1, "%s:%zu: error: unknown version visibility: %s",
             file_path.c_str(), curr_line_num, visibility.c_str());
      }

      if (global_scope) {
        size_t semicolon_pos = curr_line.find(';');
        if (semicolon_pos == std::string::npos) {
          errx(1, "%s:%zu: error: symbol name line must end with ';'",
               file_path.c_str(), curr_line_num);
        }

        std::string symbol_name =
          android::base::Trim(curr_line.substr(0, semicolon_pos));

        size_t asterisk_pos = symbol_name.find('*');
        if (asterisk_pos != std::string::npos) {
          errx(1, "%s:%zu: error: global symbol name must not have wildcards",
               file_path.c_str(), curr_line_num);
        }
        symbols.push_back(SymbolEnt{symbol_name, curr_tags});
      }
    }

    errx(1, "%s:%zu: error: scope started from %zu must be closed before EOF",
         file_path.c_str(), curr_line_num, start_line_num);
  }

  static NdkSymbolType getSymbolType(const TagList& tags) {
    for (auto&& tag : tags) {
      if (tag == "var") {
        return NdkSymbolType::variable;
      }
    }
    return NdkSymbolType::function;
  }

  bool isInArch(const TagList& tags) const {
    bool has_arch_tags = false;
    for (auto&& tag :  tags) {
      auto it = arch_name_map.find(tag);
      if (it == arch_name_map.end()) {
        continue;
      }
      if (it->second == compilation_type.arch) {
        return true;
      }
      has_arch_tags = true;
    }
    return !has_arch_tags;
  }

  bool isInApi(const TagList& tags) const {
    bool api_level_arch = false;
    std::string api_level;
    std::string api_level_arch_prefix =
      "api-level-" + to_string(compilation_type.arch) + "=";

    bool intro_arch = false;
    std::string intro;
    std::string intro_arch_perfix =
      "introduced-" + to_string(compilation_type.arch) + "=";

    for (const std::string&tag : tags) {
      // Check api-level=
      if (android::base::StartsWith(tag, "api-level=") && !api_level_arch) {
        api_level = tag;
        continue;
      }
      if (android::base::StartsWith(tag, api_level_arch_prefix)) {
        api_level = tag;
        api_level_arch = true;
        continue;
      }

      // Check introduced=
      if (android::base::StartsWith(tag, "introduced=") && !intro_arch) {
        intro = tag;
        continue;
      }
      if (android::base::StartsWith(tag, intro_arch_perfix)) {
        intro = tag;
        intro_arch = true;
        continue;
      }

      if (tag == "future") {
        return compilation_type.api_level == future_api;
      }
    }

    if (intro.empty() && api_level.empty()) {
      return true;
    }

    if (!api_level.empty()) {
      // If api-level= is specified, it must be an exact match (for versioner
      // unit tests).
      return compilation_type.api_level == decodeApiLevelValue(api_level);
    }

    return compilation_type.api_level >= decodeApiLevelValue(intro);
  }

  static int decodeApiLevelValue(const std::string& tag) {
    std::string api_level = tag.substr(tag.find('=') + 1);
    auto it = api_codename_map.find(api_level);
    if (it != api_codename_map.end()) {
      return it->second;
    }
    return std::stoi(api_level);
  }

 private:
  const std::string& file_path;
  const CompilationType& compilation_type;

  std::ifstream file;
  std::string curr_line;
  std::vector<std::string> curr_tags;
  size_t curr_line_num;
};

}  // anonymous namespace


std::optional<SymbolMap> parseSymbolFile(const std::string& file_path,
                                         const CompilationType& type) {

  SymbolFileParser parser(file_path, type);
  return parser.parse();
}
