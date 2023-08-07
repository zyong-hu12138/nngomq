#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdint.h>
#include "pickle.h"
struct message
{
    char *topic;
    PyObject* payload;
};
int get_local_ip(char* ip);//循环获取本机IP地址

#endif
