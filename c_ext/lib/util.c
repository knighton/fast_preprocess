#include <stdio.h>
#include <stdlib.h>

#include "util.h"

bool util_extract_int_from_ascii(
        const char* text, size_t text_size, size_t* index, uint32_t* value) {
    if (!(*index < text_size)) {
        return false;
    }

    // Skip until we find the first digit.
    while (true) {
        char c = text[*index];
        ++(*index);

        if ('0' <= c && c <= '9') {
            *value = c - '0';
            break;
        }

        if (*index == text_size) {
            return false;
        }
    }

    // Read the rest of the digits.
    while (*index < text_size) {
        char c = text[*index];
        ++(*index);

        if (!('0' <= c && c <= '9')) {
            return true;
        }

        *value *= 10;
        *value += c - '0';
    }

    return true;
}

bool util_read_file(const char* file_name, char** text, size_t* text_size) {
    FILE* f = fopen(file_name, "rb");
    if (!f) {
        *text = NULL;
        *text_size = 0;
        return false;
    }

    fseek(f, 0, SEEK_END);
    *text_size = ftell(f);
    rewind(f);
    
    *text = malloc(*text_size);
    if (!*text) {
        return false;
    }

    size_t actually_read;
    if ((actually_read = fread(*text, 1, *text_size, f)) != *text_size) {
        free(*text);
        *text = NULL;
        *text_size = 0;
        fclose(f);
        return false;
    }

    fclose(f);
    return true;
}
