#include <cassert>

#include "../cpp_ext/unicode.h"
#include "util.h"

#define TEST_PAGE_LEN_ASCII 40553
#define TEST_PAGE_LEN_UTF8 22692
#define TEST_PAGE_LEN_MAC_PYTHON_UTF8 23823

int main() {
    string s;
    assert(util::ReadFile("test/unicode_3_2_test_page.html", &s));

    assert(s.size() == TEST_PAGE_LEN_ASCII);

    size_t x = 0;
    unicode::CodePoint c;
    size_t count = 0;
    while (unicode::ReadNextUTF8(s, &x, &c)) {
        ++count;
    }

    assert(count == TEST_PAGE_LEN_UTF8);
}
