#include "utils.h"
#include "pickle.h"
#include "reqrep.h"
#include "parameter.h"
#include "pickle.h"
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
    Py_Initialize();
    string ip="127.0.0.1";
    REP rep(Target {data(ip),51100});
    REQ req(Target {data(ip),51100},100,100,true);
    char *topic = "hello";
    char *payload = "world";
    void callback(char *topic,char *payload);
    rep.loop_start(callback);//rep接收消息
    req.send(topic,payload);//发送信息
    sleep(10);
    // Py_Finalize();
    return 0;
}
void callback(char *topic,char *payload)
{
    cout<<"callback"<<endl;
    printf("topic:%s,payload:%s\n",topic,payload);
}