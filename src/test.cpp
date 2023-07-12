#include "utils.h"
#include "reqrep.h"
#include "parameter.h"
#include <vector>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
using namespace std;
#pragma warning
int main()
{   string ip="127.0.0.1";
    REP rep(Target {data(ip),51100});
    REQ req(Target {data(ip),51100},100,100,0);
    char *topic = "hello";
    char *payload = "world";
    req._send(topic,payload);//发送信息
    rep.loop_start();//rep接收消息
    rep.reply(topic,payload);
    req.recv();
    return 0;
}