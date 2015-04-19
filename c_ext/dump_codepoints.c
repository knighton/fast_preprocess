#include <assert.h>
#include <stdio.h>

#include "lib/unicode.h"
#include "lib/util.h"

void help(const char* name_of_program, FILE* out_f) {
    fprintf(out_f,
        "\n"
        "    Usage:\n"
        "    * %s (input file)\n"
        "    * %s (input file) (output file)\n"
        "\n"
        "Dumps the UTF-8 code points of the input file to the output file,\n"
        "one human-readable integer per line.\n"
        "\n"
        "If an output file is not provided, writes to stdout.\n",
        name_of_program, name_of_program);
}

bool dump_code_points(const char* in_f_name, FILE* out) {
    if (!out) {
        return false;
    }

    char* str = NULL;
    size_t str_size = ~0ul;
    if (!util_read_file(in_f_name, &str, &str_size)) {
        return false;
    }

    unicode_code_point_t c;
    size_t str_index = 0;
    while (read_next_utf8(str, str_size, &str_index, &c)) {
        fprintf(out, "%u\n", c);
    }
    return true;
}

int main(int argc, char* argv[]) {
    const char* in_f;
    FILE* out_f;
    if (argc == 2) {
        in_f = argv[1];
        out_f = stdout;
    } else if (argc == 3) {
        in_f = argv[1];
        out_f = fopen(argv[2], "wb");
    } else {
        help(argv[0], stderr);
        return 1;
    }

    dump_code_points(in_f, out_f);
}
