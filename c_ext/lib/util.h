#ifndef UTIL_H_
#define UTIL_H_

#include <stdbool.h>
#include <stdint.h>

// Read until we see digits.  Then, extract those digits.  Leave 'index' pointed
// at the first non-digit character.
//
// Returns true if it was able to extract an unsigned int (so false means end of
// text).
bool util_read_next_uint32_from_ascii(
    const char* text, size_t text_size, size_t* index, uint32_t* value);

// Load a entire file into a buffer and return it (caller takes ownership of
// 'text' if it is non-NULL).
//
// Returns true if was able to read the file.
bool util_read_file(const char* file_name, char** text, size_t* text_size);

#endif  // UTIL_H_
