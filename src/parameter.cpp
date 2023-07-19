 
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
char *SELF_IP=(char*)malloc(40*sizeof(char));
char *IMX6Q_IP;
char *NANO_IP;
char *error =(char*)malloc (100*sizeof(char));
Addresslib Addrlib;
map<string,Address> mymap; 
void _init_()
{
    mymap.insert(make_pair("VISION",Addrlib.VISION));//插入元素
    mymap.insert(make_pair("RADAR",Addrlib.RADAR));
    mymap.insert(make_pair("NANO_CAM",Addrlib.NANO_CAM));
    mymap.insert(make_pair("NANO_LOC",Addrlib.NANO_LOC));
    mymap.insert(make_pair("NANO_LOG",Addrlib.NANO_LOG));
    mymap.insert(make_pair("APP_COMM",Addrlib.APP_COMM));
    mymap.insert(make_pair("MAIN",Addrlib.MAIN));
    mymap.insert(make_pair("COMMUNICATION",Addrlib.COMMUNICATION));
    mymap.insert(make_pair("PLAN",Addrlib.PLAN));
    mymap.insert(make_pair("DECISION",Addrlib.DECISION));
    mymap.insert(make_pair("CONTROL",Addrlib.CONTROL));
    mymap.insert(make_pair("LOG",Addrlib.LOG));
    mymap.insert(make_pair("LOCATION",Addrlib.LOCATION));
    mymap.insert(make_pair("test",Addrlib.test));
    IP_SET();

}
char* get_name_ip(string name)
{
    return mymap[name].ip;
}
int get_name_port(string name)
{
    return mymap[name].port;
}
void set_name_ip(string name, char* ip)
{
    mymap[name].ip = ip;
}
void set_name_port(string name, int port)
{
    mymap[name].port = port;
}
int IP_SET()
{
    int status = 0;
    while(1){
        status = get_local_ip(SELF_IP);
        if(SELF_IP == NULL) 
            {error = "can't get local ip"; 
            continue;};
        if(SELF_IP!=NULL)
            break;
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