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
    REP rep(Addrlib.test);
    char *topic = "hello";
    char *payload = "world";
    void callback(char *topic,char *payload);
    rep.loop_start(callback);//rep接收消息
    for(int i=0;i<10;i++)
    {
        sleep(1);
        req(Addrlib.test,topic,payload,100,100,true);     
    }

    sleep(10);
    return 0;
}
void callback(char *topic,char *payload)
{
    cout<<"callback"<<endl;
    printf("topic:%s,payload:%s\n",topic,payload);
}