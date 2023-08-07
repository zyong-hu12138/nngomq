#include <Python.h>
#include <string.h>
#include "pickle.h"
#include <iostream>
using namespace std;
PyObject *pyPickleModule ;
PyObject *pyDumpsFunc ;
PyObject *pyLoadsFunc;
PyObject *congverStringToBytes(PyObject *inputObj)//Pyobject
{   
    PyObject *pyArgs = PyTuple_Pack(1, inputObj);
    PyObject *pyBytes = PyObject_CallObject(pyDumpsFunc, pyArgs);
    Py_DECREF(pyArgs);
    // Py_Finalize();
    return pyBytes;
}

void getPyObjectAsString(const char *str, const size_t size, PyObject **outputObj)
{
    PyObject *pyObject = PyBytes_FromStringAndSize(str, size);
    PyObject *pyArgs = PyTuple_Pack(1, pyObject);
    *outputObj = PyObject_CallObject(pyLoadsFunc, pyArgs);
    Py_DECREF(pyArgs);
}