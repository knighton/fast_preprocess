#include <assert.h>
#include <Python.h>

#include "lib/hashmap.h"
#include "lib/unicode.h"
#include "lib/util.h"

#define UNUSED(a) &(a)

// A file listing the decimal integer code points of all digit-like Unicode
// characters.
#define FILE_DIGITS "c_ext/config/digits.txt"
#define APPROX_NUM_DIGITS 400

static char FAST_PREPROCESS_DOC[] =
    "C++ extension that does preprocessing.\n"
    "\n"
    "Methods that operate on Unicode objects:\n"
    "- destutter_unicode\n"
    "\n"
    "Methods that operate on raw bytes:\n"
    "- destutter_ascii_bytes\n"
    "- destutter_utf8_encoded_bytes\n";

bool increment(PyObject* key2count, PyObject* key) {
    PyObject* value = PyDict_GetItem(key2count, key);
    PyObject* new_value;
    if (!value) {
        new_value = PyInt_FromLong(1);
    } else {
        new_value = PyInt_FromLong(PyInt_AS_LONG(value) + 1);
    }
    return !PyDict_SetItem(key2count, key, new_value);
}

static char DESTUTTER_UNICODE_DOC[] =
    "(unicode, max repeat, dict) -> unicode without excessive repeats.\n"
    "\n"
    "Saves code point -> drop count to the dict.";

static Hashmap* DIGITS;

static PyObject* destutter_unicode(PyObject* self, PyObject* args) {
    UNUSED(self);

    Py_UNICODE* input_str;
    int input_str_len;
    int max_repeat_count;
    PyObject* chr2dropcount;
    if (!PyArg_ParseTuple(
            args, "u#IO!", &input_str, &input_str_len, &max_repeat_count,
            &PyDict_Type, &chr2dropcount)) {
        return NULL;
    }

    if (!input_str_len) {
        return PyUnicode_FromStringAndSize("", 0);
    }

    Py_UNICODE prev_c = input_str[0];
    int run_length = 1;
    Py_UNICODE* r = (Py_UNICODE*)malloc(input_str_len * sizeof(Py_UNICODE));
    int r_used = 0;
    r[r_used++] = input_str[0];
    int i;
    for (i = 1; i < input_str_len; ++i) {
        Py_UNICODE c = input_str[i];

        unicode_code_point_t code_point = c;
        if (c != prev_c || hashmapContainsKey(DIGITS, &code_point)) {
            prev_c = c;
            run_length = 1;
            r[r_used++] = c;
            continue;
        }

        ++run_length;
        if (run_length <= max_repeat_count) {
            r[r_used++] = c;
            continue;
        }

        PyObject* key = PyUnicode_FromUnicode(&c, 1);
        if (!increment(chr2dropcount, key)) {
            free(r);
            return NULL;
        }
    }

    PyObject* ret = PyUnicode_FromUnicode(r, r_used);
    free(r);
    return ret;
}

static char DESTUTTER_ASCII_BYTES_DOC[] =
    "(ASCII string, max char repeat count, dict) -> string with excessive "
    "repeats removed.\n"
    "\n"
    "Saves code point -> drop count to the dict.";

static PyObject* destutter_ascii_bytes(PyObject* self, PyObject* args) {
    UNUSED(self);

    char* input_str;
    int input_str_len;
    int max_repeat_count;
    PyObject* chr2dropcount;
    if (!PyArg_ParseTuple(
            args, "s#IO!", &input_str, &input_str_len, &max_repeat_count,
            &PyDict_Type, &chr2dropcount)) {
        return NULL;
    }

    if (!input_str_len) {
        return PyString_FromString("");
    }

    char prev_c = input_str[0];
    int run_length = 1;
    char* r = (char*)malloc(input_str_len);
    size_t r_used = 0;
    r[r_used++] = input_str[0];
    int i;
    for (i = 1; i < input_str_len; ++i) {
        char c = input_str[i];

        unicode_code_point_t code_point = c;
        if (c != prev_c || hashmapContainsKey(DIGITS, &code_point)) {
            prev_c = c;
            run_length = 1;
            r[r_used++] = c;
            continue;
        }

        ++run_length;
        if (run_length <= max_repeat_count) {
            r[r_used++] = c;
            continue;
        }

        PyObject* key = PyString_FromFormat("%c", c);
        if (!increment(chr2dropcount, key)) {
            free(r);
            return NULL;
        }
    }

    PyObject* ret = PyString_FromStringAndSize(r, r_used);
    free(r);
    return ret;
}

static char DESTUTTER_UTF8_ENCODED_BYTES_DOC[] =
    "(raw bytes of a UTF-8 string, max chr repeat count, dict) -> string with "
    "excessive repeats removed.\n"
    "\n"
    "Saves code point -> drop count to the dict.";

static PyObject* destutter_utf8_encoded_bytes(PyObject* self, PyObject* args) {
    UNUSED(self);

    char* input_str;
    int input_str_len;
    int max_repeat_count;
    PyObject* chr2dropcount;
    if (!PyArg_ParseTuple(
            args, "s#IO!", &input_str, &input_str_len, &max_repeat_count,
            &PyDict_Type, &chr2dropcount)) {
        return NULL;
    }

    if (!input_str_len) {
        return PyString_FromString("");
    }

    size_t byte_index = 0;
    unicode_code_point_t c;
    if (!read_next_utf8(input_str, input_str_len, &byte_index, &c)) {
        return PyString_FromString("");
    }

    unicode_code_point_t prev_c = c;
    int run_length = 1;
    char* r = (char*)malloc(input_str_len);
    size_t r_used = 0;
    size_t r_capacity = input_str_len;
    if (append_utf8(c, r, &r_used, r_capacity) != AUR_OK) {
        free(r);
        return NULL;
    }
    while (read_next_utf8(input_str, input_str_len, &byte_index, &c)) {
        if (c != prev_c || hashmapContainsKey(DIGITS, &c)) {
            prev_c = c;
            run_length = 1;
            if (append_utf8(c, r, &r_used, r_capacity) != AUR_OK) {
                free(r);
                return NULL;
            }
            continue;
        }

        ++run_length;
        if (run_length <= max_repeat_count) {
            if (append_utf8(c, r, &r_used, r_capacity) != AUR_OK) {
                free(r);
                return NULL;
            }
            continue;
        }

        PyObject* key = PyInt_FromLong(c);
        if (!increment(chr2dropcount, key)) {
            free(r);
            return NULL;
        }
    }

    PyObject* ret = PyString_FromStringAndSize(r, r_used);
    free(r);
    return ret;
}

static PyMethodDef FAST_PREPROCESS_METHODS[] = {
    {"destutter_unicode", destutter_unicode, METH_VARARGS,
     DESTUTTER_UNICODE_DOC},
    {"destutter_ascii_bytes", destutter_ascii_bytes, METH_VARARGS,
     DESTUTTER_ASCII_BYTES_DOC},
    {"destutter_utf8_encoded_bytes", destutter_utf8_encoded_bytes, METH_VARARGS,
     DESTUTTER_UTF8_ENCODED_BYTES_DOC},
    {NULL, NULL, 0, NULL},
};

bool fast_preprocess_init_digits() {
    // Load the file.
    char* text;
    size_t text_size;
    if (!util_read_file(FILE_DIGITS, &text, &text_size)) {
        fprintf(stderr, "Could not read \"%s\".\n", FILE_DIGITS);
        return false;
    }

    // Create the hashmap.
    int num_expected_entries = APPROX_NUM_DIGITS;
    DIGITS = hashmapCreate(
        num_expected_entries, &hashmapIntHash, &hashmapIntEquals);

    // Load each code point into the hashmap.
    size_t index = 0;
    unicode_code_point_t value;
    int count = 0;
    while (util_extract_int_from_ascii(text, text_size, &index, &value)) {
        unicode_code_point_t* key = malloc(sizeof(unicode_code_point_t));
        *key = value;
        hashmapPut(DIGITS, key, NULL);
        ++count;
    }

    return true;
}

PyMODINIT_FUNC initfast_preprocess(void) {
    if (!fast_preprocess_init_digits()) {
        return;
    }

    Py_InitModule3(
        "fast_preprocess", FAST_PREPROCESS_METHODS, FAST_PREPROCESS_DOC);
}
