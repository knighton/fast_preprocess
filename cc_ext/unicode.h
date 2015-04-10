#ifndef CC_BASE_UNICODE_H_
#define CC_BASE_UNICODE_H_

#include <string>

using std::string;

namespace unicode {

typedef uint32_t CodePoint;

// Append the code point's UTF-8 representation to the string.
// Returns false if the code point is invalid.
bool AppendUTF8(CodePoint chr, string* append_to);

// Returns false when it hits the end of the string.
bool ReadNextUTF8(const string& input_string, size_t* byte_index,
                  CodePoint* chr);

}  // namespace unicode

#endif  // CC_BASE_UNICODE_H_