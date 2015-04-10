#ifndef CC_BASE_UNICODE_H_
#define CC_BASE_UNICODE_H_

#include <string>
#include <cstdint>

using std::string;

namespace unicode {

typedef uint32_t CodePoint;

// Append the code point's UTF-8 representation to the string.
// Returns false if the code point is invalid.
bool AppendUTF8(CodePoint chr, string* append_to);

// Returns true iff it read a code point.
bool ReadNextUTF8(const string& input_string, size_t* byte_index,
                  CodePoint* chr);

}  // namespace unicode

#endif  // CC_BASE_UNICODE_H_
