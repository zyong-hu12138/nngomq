//parameters.h
#ifndef PARAMETERS_H_
#define PARAMETERS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <ifaddrs.h>
///get the ip address of the current machine
using namespace std;
extern char *SELF_IP;//d定义全局变量SELF_IP
extern char *IMX6Q_IP;//定义全局变量IMX6Q_IP
extern char *NANO_IP;//定义全局变量PC_IP

int IP_SET();

typedef struct{
    char* name;
    char* ip;
    int  port;
    char* ipc; 
}Address;

// extern const Address VISION ;
// extern const Address RADAR ;
// extern const Address NANO_COMM ;
// extern const Address NANO_LOC ;
// extern const Address NANO_LOG ;
// extern const Address APP_COMM ;
// extern const Address MAIN ;
// extern const Address COMMUNICATION ;
// extern const Address PLAN ;
// extern const Address DECISION ;
// extern const Address CONTROL  ;
// extern const Address LOG  ;
// extern const Address LOCATION ;
extern vector<Address> Addrlib;
extern char *get_name_ip(char* name);
extern char *get_name_port(char* name);
extern void set_name_ip(char* name, char* ip);
extern void set_name_port(char* name, int  port);

#endif