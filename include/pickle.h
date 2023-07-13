#ifndef PICKLE_H_
#define PICKLE_H_
#include "Python.h"
#include <string.h>
using namespace std;
PyObject *congverStringToByes(const char *str);
void getPyObjectAsString(const char *str,const size_t size,char* out);
#endif