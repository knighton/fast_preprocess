#include <cstdio>
#include <string>

using std::string;

namespace util {

bool FileSize(const char* file_name, size_t* file_size);

bool ReadFile(const char* file_name, string* text);

}  // namespace util
