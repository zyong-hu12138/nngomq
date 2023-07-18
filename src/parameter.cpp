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

class Addrlib
{
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