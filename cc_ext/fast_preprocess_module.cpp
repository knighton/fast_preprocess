#include <Python.h>

#include "utf8.h"

static char FAST_PREPROCESS_DOC[] =
    "C++ extension that does preprocessing.";

static char STRIP_EXCESSIVE_REPEATS_DOC[] =
    "(string, max repeat count, dict) -> string with excessive repeats dropped.\n"
    "\n"
    "Saves code point -> drop count to the dict.";

static PyObject* strip_excessive_repeats(PyObject* self, PyObject* args) {
    char* input_str;
    int input_str_len;
    int max_repeat_count;
    PyObject* chr2dropcount;
    if (!PyArg_ParseTuple(
            args, "s#IO!", &input_str, &input_str_len, &max_repeat_count,
            &PyDict_Type, &chr2dropcount)) {
        return NULL;
    }

    return NULL;
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
