#include "omq.h"
#include <vector>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
using namespace std;
#pragma warning
int main()
{   
    string ip="127.0.0.1";
    REP rep(Addrlib.test);
    char *topic = "hello";
    char *payload = "world";
    void callback(char *topic,char *payload);
    rep.loop_start(callback);//rep接收消息
    req(Addrlib.test,topic,payload,100,100,true);
    // Py_Finalize();
    return 0;
}
void callback(char *topic,char *payload)
{
    cout<<"callback"<<endl;
    printf("topic:%s,payload:%s\n",topic,payload);
}