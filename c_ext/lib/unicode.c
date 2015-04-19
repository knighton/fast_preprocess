#include "unicode.h"

// Based on https://en.wikipedia.org/wiki/UTF-8.

append_utf8_result_t append_utf8(
        unicode_code_point_t n, char* s, size_t* s_size, size_t s_capacity) {
    if (!(*s_size <= s_capacity)) {
        return AUR_INVALID_STRING;
    }

    if (n < 0x80) {
        if (!(*s_size + 1ul <= s_capacity)) {
            return (append_utf8_result_t)(*s_size + 1ul - s_capacity);
        }

        s[(*s_size)++] = (char)n;
        return AUR_OK;
    }

    if (n < 0x800) {
        if (!(*s_size + 2ul <= s_capacity)) {
            return (append_utf8_result_t)(*s_size + 2ul - s_capacity);
        }

        s[(*s_size)++] = (char)((n >> 6) + 0xC0);
        s[(*s_size)++] = (char)((n & 0x3F) + 0x80);
        return AUR_OK;
    }

    if (n < 0x10000) {
        if (!(*s_size + 3ul <= s_capacity)) {
            return (append_utf8_result_t)(*s_size + 3ul - s_capacity);
        }

        s[(*s_size)++] = (char)((n >> 12) + 0xE0);
        s[(*s_size)++] = (char)(((n >> 6) & 0x3F) + 0x80);
        s[(*s_size)++] = (char)((n & 0x3F) + 0x80);
        return AUR_OK;
    }

    if (n < 0x110000) {
        if (!(*s_size + 4ul <= s_capacity)) {
            return (append_utf8_result_t)(*s_size + 4ul - s_capacity);
        }

        s[(*s_size)++] = (char)((n >> 18) + 0xF0);
        s[(*s_size)++] = (char)(((n >> 12) & 0x3F) + 0x80);
        s[(*s_size)++] = (char)(((n >> 6) & 0x3F) + 0x80);
        s[(*s_size)++] = (char)((n & 0x3F) + 0x80);
        return AUR_OK;
    }

    return AUR_INVALID_CODE_POINT;
}

bool read_next_utf8(
        const char* s, size_t s_size, size_t* x, unicode_code_point_t* n) {
    if (!(*x < s_size)) {
        return false;
    }

    uint8_t c0;
    uint8_t c1;
    uint8_t c2;
    uint8_t c3;

    c0 = (uint8_t)s[*x];
    ++(*x);

    if (c0 < 0x80) {
        *n = c0;
        return true;
    } else if (c0 < 0xC2) {
        // Continuation or overlong 2-byte sequence.
        goto ERROR1;
    } else if (c0 < 0xE0) {
        // 2-byte sequence.
        c1 = (uint8_t)s[*x];
        ++(*x);
        if ((c1 & 0xC0) != 0x80) {
            goto ERROR2;
        }
        *n = (uint32_t)(c0 << 6) + c1 - 0x3080;
        return true;
    } else if (c0 < 0xF0) {
        // 3-byte sequence.
        c1 = (uint8_t)s[*x];
        ++(*x);
        if ((c1 & 0xC0) != 0x80) {
            goto ERROR2;
        }
        if (c0 == 0xE0 && c1 < 0xA0) {
            goto ERROR2;    // overlong.
        }
        c2 = (uint8_t)s[*x];
        ++(*x);
        if ((c2 & 0xC0) != 0x80) {
            goto ERROR3;
        }
        *n = (uint32_t)(c0 << 12) + (uint32_t)(c1 << 6) + c2 - 0xE2080;
        return true;
    } else if (c0 < 0xF5) {
        // 4-byte sequence.
        c1 = (uint8_t)s[*x];
        ++(*x);
        if ((c1 & 0xC0) != 0x80) {
            goto ERROR2;
        }
        if (c0 == 0xF0 && c1 < 0x90) {
            goto ERROR2;  // Overlong.
        }
        if (c0 == 0xF4 && c1 >= 0x90) {
            goto ERROR2;  // > U+10FFFF.
        }
        c2 = (uint8_t)s[*x];
        ++(*x);
        if ((c2 & 0xC0) != 0x80) {
            goto ERROR3;
        }
        c3 = (uint8_t)s[*x];
        ++(*x);
        if ((c3 & 0xC0) != 0x80) {
            goto ERROR4;
        }
        *n = (uint32_t)(c0 << 18) + (uint32_t)(c1 << 12) + (uint32_t)(c2 << 6) + c3
            - 0x3C82080;
        return true;
    } else {
        // U+10FFFF.
        goto ERROR1;
    }

    ERROR4:
        --(*x);
    ERROR3:
        --(*x);
    ERROR2:
        --(*x);
    ERROR1:
        *n = c0 + 0xDC00;
    return true;
}
