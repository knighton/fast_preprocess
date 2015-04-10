#include <cstdio>
#include <Python.h>
#include <string>

#include "unicode.h"

using std::string;

static char FAST_PREPROCESS_DOC[] =
    "C++ extension that does preprocessing.";

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

static char STRIP_EXCESSIVE_REPEATS_ASCII_DOC[] =
    "(ASCII string, max char repeat count, dict) -> string with excessive "
    "repeats removed.\n"
    "\n"
    "Saves code point -> drop count to the dict.";

static PyObject* strip_excessive_repeats_ascii(PyObject* self, PyObject* args) {
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
    string r;
    for (int i = 1; i < input_str_len; ++i) {
        char c = input_str[i];

        if (c != prev_c) {
            prev_c = c;
            run_length = 1;
            r += c;
            continue;
        }

        ++run_length;
        if (run_length <= max_repeat_count) {
            r += c;
            continue;
        }

        PyObject* key = PyString_FromFormat("%c", c);
        if (!increment(chr2dropcount, key)) {
            return NULL;
        }
    }

    return PyString_FromStringAndSize(r.data(), r.size());
}

static char STRIP_EXCESSIVE_REPEATS_UTF8_DOC[] =
    "(raw bytes of a UTF-8 string, max chr repeat count, dict) -> string with "
    "excessive repeats removed.\n"
    "\n"
    "Saves code point -> drop count to the dict.";

static PyObject* strip_excessive_repeats_utf8(PyObject* self, PyObject* args) {
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

    string input_s = string(input_str, input_str_len);
    size_t byte_index = 0;
    unicode::CodePoint c;
    if (!unicode::ReadNextUTF8(input_s, &byte_index, &c)) {
        return PyString_FromString("");
    }

    unicode::CodePoint prev_c = c;
    int run_length = 1;
    string r;
    while (unicode::ReadNextUTF8(input_s, &byte_index, &c)) {
        if (c != prev_c) {
            prev_c = c;
            run_length = 1;
            r += c;
            continue;
        }

        ++run_length;
        if (run_length <= max_repeat_count) {
            r += c;
            continue;
        }

        PyObject* key = PyInt_FromLong(c);
        if (!increment(chr2dropcount, key)) {
            return NULL;
        }
    }

    return PyString_FromStringAndSize(r.data(), r.size());
}

static PyMethodDef FAST_PREPROCESS_METHODS[] = {
    {"strip_excessive_repeats_ascii", strip_excessive_repeats_ascii,
     METH_VARARGS, STRIP_EXCESSIVE_REPEATS_ASCII_DOC},
    {"strip_excessive_repeats_utf8", strip_excessive_repeats_utf8,
     METH_VARARGS, STRIP_EXCESSIVE_REPEATS_UTF8_DOC},
    {NULL, NULL, 0, NULL},
};

PyMODINIT_FUNC initfast_preprocess(void) {
	Py_InitModule3(
        "fast_preprocess", FAST_PREPROCESS_METHODS, FAST_PREPROCESS_DOC);
}
