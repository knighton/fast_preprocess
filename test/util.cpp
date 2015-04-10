#include <cstdio>
#include <string>

using std::string;

namespace util {

bool FileSize(const char* file_name, size_t* file_size) {
    FILE* f = fopen(file_name, "rb");
    if (!f) {
        return false;
    }

    fseek(f, 0, SEEK_END);
    *file_size = ftell(f);
    fclose(f);
    return true;
}

bool ReadFile(const char* file_name, string* text) {
    size_t size;
    if (!util::FileSize(file_name, &size)) {
        return false;
    }

    text->resize(size);

    FILE* f = fopen(file_name, "rb");
    if (!f) {
        return false;
    }

    if (fread(&(*text)[0], 1, size, f) != size) {
        text->clear();
        fclose(f);
        return false;
    }

    fclose(f);
    return true;
}

}  // namespace util
