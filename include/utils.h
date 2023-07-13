#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdint.h>
struct message
{
    char *topic;
    char* payload;
};
int get_local_ip(char* ip);//循环获取本机IP地址

#endif
