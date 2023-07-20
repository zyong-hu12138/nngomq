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
#include <map>
///get the ip address of the current machine
using namespace std;
extern char *SELF_IP;//d定义全局变量SELF_IP
extern char *IMX6Q_IP;//定义全局变量IMX6Q_IP
extern char *NANO_IP;//定义全局变量PC_IP
extern char* error;
int IP_SET();

typedef struct{
    char* name;
    char* ip;
    int  port;
    char* ipc; 
} Address;


class Addresslib
{
    public:
    Address VISION={"VISION", NANO_IP, 51010, NULL};
    Address RADAR={"RADAR", NANO_IP, 51011, NULL};
    Address NANO_CAM={"NANO_COMM", NANO_IP, 51012, NULL};
    Address NANO_LOC={"NANO_LOC", NANO_IP, 51013, NULL};
    Address NANO_LOG={"NANO_LOG", NANO_IP, 51014, NULL};
    Address APP_COMM={"APP_COMM", "192.168.3.113", 51000, NULL};
    Address MAIN={"MAIN",IMX6Q_IP,51000,"ipc:///tmp/main.ipc"};
    Address COMMUNICATION={"COMMUNICATION", IMX6Q_IP, 51001, "ipc:///tmp/comm.ipc"};
    Address PLAN={"PLAN", IMX6Q_IP, 51002, "ipc:///tmp/plan.ipc"};
    Address DECISION={"DECISION", IMX6Q_IP, 51003, "ipc:///tmp/decision.ipc"};
    Address CONTROL={"CONTROL", IMX6Q_IP, 51004, "ipc:///tmp/control.ipc"};
    Address LOG={"LOG", IMX6Q_IP, 51005, "ipc:///tmp/log.ipc"};
    Address LOCATION={"LOCATION", IMX6Q_IP, 51006, "ipc:///tmp/location.ipc"};
    Address test={"test",SELF_IP,51000,"ipc:///tmp/test.ipc"};
};
extern Addresslib Addrlib;
extern map<string,Address*> mymap;

void _init_();
extern char *get_name_ip(string name);
extern int get_name_port(string name);
extern void set_name_ip(string name, char* ip);
extern void set_name_port(string name, int  port);

#endif