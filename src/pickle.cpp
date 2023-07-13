#include <Python.h>
#include <string.h>
#include "pickle.h"
using namespace std;
PyObject *congverStringToBytes(const char *str)
{   
    // Py_Initialize();

    PyObject *pyString = PyUnicode_FromString(str);
    PyObject *pyPickleModule = PyImport_ImportModule("pickle");
    PyObject *pyDumpsFunc = PyObject_GetAttrString(pyPickleModule, "dumps");
    PyObject *pyArgs = PyTuple_Pack(1, pyString);
    PyObject *pyBytes = PyObject_CallObject(pyDumpsFunc, pyArgs);
    Py_DECREF(pyString);
    Py_DECREF(pyPickleModule);
    Py_DECREF(pyDumpsFunc);
    Py_DECREF(pyArgs);
    // Py_Finalize();
    return pyBytes;
}
void getPyObjectAsString(const char *str, const size_t size, char *out)
{
    // Py_Initialize();
    PyObject *pyObject = PyBytes_FromStringAndSize(str, size);
    PyObject *pyPickleModule = PyImport_ImportModule("pickle");
    PyObject *pyLoadsFunc = PyObject_GetAttrString(pyPickleModule, "loads");
    PyObject *pyArgs = PyTuple_Pack(1, pyObject);
    PyObject *pyLoadedObject = PyObject_CallObject(pyLoadsFunc, pyArgs);
    PyObject *pyString = PyObject_Str(pyLoadedObject);

    const char *tmp = PyUnicode_AsUTF8(pyLoadedObject);
    strcpy(out,tmp);
    Py_DECREF(pyPickleModule);
    Py_DECREF(pyLoadsFunc);
    Py_DECREF(pyArgs);
    Py_DECREF(pyLoadedObject);
    Py_DECREF(pyString);
    // Py_Finalize();
}