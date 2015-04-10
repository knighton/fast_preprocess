#include <cstdio>
#include <Python.h>
#include <string>

#include "utf8.h"

using std::string;

static char FAST_PREPROCESS_DOC[] =
    "C++ extension that does preprocessing.";

static char STRIP_EXCESSIVE_REPEATS_DOC[] =
    "(string, max repeat count, dict) -> string with excessive repeats dropped.\n"
    "\n"
    "Saves code point -> drop count to the dict.";

static PyObject* strip_excessive_repeats(PyObject* self, PyObject* args) {
    // Extract args (input string, max repeat count, chr -> drop count dict).
    char* input_str;
    int input_str_len;
    int max_repeat_count;
    PyObject* chr2dropcount;
    if (!PyArg_ParseTuple(
            args, "s#IO!", &input_str, &input_str_len, &max_repeat_count,
            &PyDict_Type, &chr2dropcount)) {
        return NULL;
    }

    // If input string is empty, we're done.
    if (!input_str_len) {
        return PyString_FromString("");
    }

    // For each character,
    char prev_chr = input_str[0];
    int run_length = 1;
    string r;
    for (int i = 1; i < input_str_len; ++i) {
        char c = input_str[i];

        // If this character is different, reset state.
        if (c != prev_chr) {
            prev_chr = c;
            run_length = 1;
            r += c;
            continue;
        }

        // If it's the same, we're done if the run isn't too long.
        ++run_length;
        if (run_length <= max_repeat_count) {
            r += c;
            continue;
        }

        // Note excessive repeats.
        PyObject* key = PyInt_FromLong(c);
        PyObject* value = PyDict_GetItem(chr2dropcount, key);
        PyObject* new_value;
        if (!value) {
            new_value = PyInt_FromLong(1);
        } else {
            new_value = PyInt_FromLong(PyInt_AS_LONG(value) + 1);
        }
        if (PyDict_SetItem(chr2dropcount, key, new_value)) {
            return NULL;
        }
    }

    return PyString_FromStringAndSize(r.data(), r.size());
}

static PyMethodDef FAST_PREPROCESS_METHODS[] = {
    {"strip_excessive_repeats", strip_excessive_repeats, METH_VARARGS,
     STRIP_EXCESSIVE_REPEATS_DOC},
    {NULL, NULL, 0, NULL},
};

PyMODINIT_FUNC initfast_preprocess(void) {
	Py_InitModule3(
        "fast_preprocess", FAST_PREPROCESS_METHODS, FAST_PREPROCESS_DOC);
}
