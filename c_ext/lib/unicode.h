#ifndef UNICODE_H_
#define UNICODE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint32_t unicode_code_point_t;

// Result from append_utf8().
typedef enum append_utf8_result {
    // Successfully appended the code point.
    AUR_OK = 0,

    // Could not write the code point because the provided buffer did not have
    // enough room.  Needs this many more bytes, then try again.
    AUR_NEED_1 = 1,
    AUR_NEED_2 = 2,
    AUR_NEED_3 = 3,
    AUR_NEED_4 = 4,

    // The code point was invalid.
    AUR_INVALID_CODE_POINT = 5,

    // The string to append to was invalid (size > capacity).
    AUR_INVALID_STRING = 6,
} append_utf8_result_t;

// Append the code point's UTF-8 representation to the string.
append_utf8_result_t append_utf8(
    unicode_code_point_t chr, char* str, size_t* str_size, size_t str_capacity);

// Read the next code point from that point in the string.
// Returns true iff it read a code point.
bool read_next_utf8(
    const char* str, size_t str_size, size_t* byte_index,
    unicode_code_point_t* chr);

#endif  // UNICODE_H_
