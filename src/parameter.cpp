#include "parameter.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <map>
using namespace std;
#define SEPARATOR "^&*;"
char *SELF_IP;
char *IMX6Q_IP;
char *NANO_IP;
Addresslib Addrlib;
map<char*,Address> mymap; 
void _init_()
{
    mymap["VISION"]=Addrlib.VISION;
    mymap["RADAR"]=Addrlib.RADAR;
    mymap["NANO_CAM"]=Addrlib.NANO_CAM;
    mymap["NANO_LOC"]=Addrlib.NANO_LOC;
    mymap["NANO_LOG"]=Addrlib.NANO_LOG;
    mymap["APP_COMM"]=Addrlib.APP_COMM;
    mymap["MAIN"]=Addrlib.MAIN;
    mymap["COMMUNICATION"]=Addrlib.COMMUNICATION;
    mymap["PLAN"]=Addrlib.PLAN;
    mymap["DECISION"]=Addrlib.DECISION;
    mymap["CONTROL"]=Addrlib.CONTROL;
    mymap["LOG"]=Addrlib.LOG;
    mymap["LOCATION"]=Addrlib.LOCATION;
    mymap["test"]=Addrlib.test;
    IP_SET();
}
char* get_name_ip(char* name)
{
    return mymap[name].ip;
}
int get_name_port(char* name)
{
    return mymap[name].port;
}
void set_name_ip(char* name, char* ip)
{
    mymap[name].ip = ip;
}
void set_name_port(char* name, int port)
{
    mymap[name].port = port;
}
int IP_SET()
{
    int status = 0;
    char *ip=(char*)malloc(40*sizeof(char));
    while(1){
        status = get_local_ip(ip);
        cout<<1<<endl;
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