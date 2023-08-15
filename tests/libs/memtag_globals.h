#include <vector>

void check_tagged(const void* a);
void check_untagged(const void* a);
void check_matching_tags(const void* a, const void* b);
void check_eq(const void* a, const void* b);

void dso_check_assertions(bool enforce_tagged);
void dso_print_variables();

void print_variable_address(const char* name, const void* ptr);
void print_variables(const char* header,
                     const std::vector<std::pair<const char*, const void*>>& tagged_variables,
                     const std::vector<std::pair<const char*, const void*>>& untagged_variables);