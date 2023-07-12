#include "parameter.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <vector>
using namespace std;
#define SEPARATOR "^&*;"
char *SELF_IP;
char *IMX6Q_IP;
char *NANO_IP;

// const Address VISION = {"VISION", NANO_IP, 51010, NULL};
// const Address RADAR = {"RADAR", NANO_IP, 51011, NULL};
// const Address NANO_COMM = {"NANO_COMM", NANO_IP, 51012, NULL};
// const Address NANO_LOC = {"NANO_LOC", NANO_IP, 51013, NULL};
// const Address NANO_LOG = {"NANO_LOG", NANO_IP, 51014, NULL};

// const Address APP_COMM = {"APP_COMM", "192.168.3.113", 51000, NULL};

// const Address MAIN = {"APP_COMM", "192.168.3.113", 51000, NULL}
// const Address COMMUNICATION = {"COMMUNICATION", IMX6Q_IP, 51001, "ipc:///tmp/comm.ipc"};
// const Address PLAN = {"PLAN", IMX6Q_IP, 51002, "ipc:///tmp/plan.ipc"};
// const Address DECISION = {"DECISION", IMX6Q_IP, 51003, "ipc:///tmp/decision.ipc"};
// const Address CONTROL = {"CONTROL", IMX6Q_IP, 51004, "ipc:///tmp/control.ipc"};
// const Address LOG = {"LOG", IMX6Q_IP, 51005, "ipc:///tmp/log.ipc"};
// const Address LOCATION = {"LOCATION", IMX6Q_IP, 51006, "ipc:///tmp/location.ipc"};

vector<Address> Addrlib={
    {"VISION", NANO_IP, 51010, NULL},
    {"RADAR", NANO_IP, 51011, NULL},
    {"NANO_COMM", NANO_IP, 51012, NULL},
    {"NANO_LOC", NANO_IP, 51013, NULL},
    {"NANO_LOG", NANO_IP, 51014, NULL},
    {"APP_COMM", "192.168.3.113", 51000, NULL},
    {"MAIN",IMX6Q_IP,51000,"ipc:///tmp/main.ipc"},
    {"COMMUNICATION", IMX6Q_IP, 51001, "ipc:///tmp/comm.ipc"},
    {"PLAN", IMX6Q_IP, 51002, "ipc:///tmp/plan.ipc"},
    {"DECISION", IMX6Q_IP, 51003, "ipc:///tmp/decision.ipc"},
    {"CONTROL", IMX6Q_IP, 51004, "ipc:///tmp/control.ipc"},
    {"LOG", IMX6Q_IP, 51005, "ipc:///tmp/log.ipc"},
    {"LOCATION", IMX6Q_IP, 51006, "ipc:///tmp/location.ipc"}

};

char* get_name_ip(char* name)
{
    for(int i=0;i<Addrlib.size();i++)
    {
        if(strcmp(name, Addrlib[i].name) == 0)
        {
            return (char*)Addrlib[i].ip;
        }
    }
    return NULL;
}
char* get_name_port(char* name)
{
    for(int i=0;i<Addrlib.size();i++)
    {
        if(strcmp(name, Addrlib[i].name) == 0)
        {
            return (char*)Addrlib[i].port;
        }
    }
    return NULL;
}
void set_name_ip(char* name, char* ip)
{
    for(int i=0;i<Addrlib.size();i++)
    {
        if(strcmp(name, Addrlib[i].name) == 0)
        {
            Addrlib[i].ip = ip;
        }
    }
}
void set_name_port(char* name, int port)
{
    for(int i=0;i<Addrlib.size();i++)
    {
        if(strcmp(name, Addrlib[i].name) == 0)
        {
            Addrlib[i].port = port;
        }
    }
}
int IP_SET()
{
    int status = 0;
    while(1){
        status = get_local_ip(SELF_IP);
        if(SELF_IP == NULL) sleep(1);
        else break;
    }
    if(status == -1){
        printf("get local ip error\n");
        return -1;
        /////////////////////////////////////////////
        ////应考虑不同主机和固定IP的区别,此处暂时不考虑/////
        ///////////////////////////////////
    }
    IMX6Q_IP = "192.168.3.112";
    NANO_IP = "192.168.3.11";
}