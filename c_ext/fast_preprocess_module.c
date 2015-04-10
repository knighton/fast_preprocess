#include <Python.h>

#include "unicode.h"

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

static PyObject* destutter_unicode(PyObject* self, PyObject* args) {
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

        if (c != prev_c) {
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

        if (c != prev_c) {
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
        return NULL;
    }
    while (read_next_utf8(input_str, input_str_len, &byte_index, &c)) {
        if (c != prev_c) {
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

PyMODINIT_FUNC initfast_preprocess(void) {
	Py_InitModule3(
        "fast_preprocess", FAST_PREPROCESS_METHODS, FAST_PREPROCESS_DOC);
}
