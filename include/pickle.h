#ifndef PICKLE_H_
#define PICKLE_H_
#include <Python.h>
#include <string.h>
using namespace std;
extern PyObject *pyPickleModule ;
extern PyObject *pyDumpsFunc ;
extern PyObject *pyLoadsFunc;
PyObject *congverStringToBytes(PyObject *inputObj);
void getPyObjectAsString(const char *str,const size_t size, PyObject **outputObj);
#endif