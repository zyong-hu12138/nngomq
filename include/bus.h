
#ifndef BUS_H_
#define BUS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <functional>
#include "udp.h"
#include "utils.h"
#include <nng/nng.h>
#include <nng/protocol/bus0/bus.h>
#include <any>
#include "pickle.h"
using namespace std;
#include <vector>
#include <thread>
#define SEPARATOR "^&*;"
//bus 通信urls还是必要的，记录已经有的节点dial 的地址，

// extern vector <nng_socket> bus_socks; //用来监听连接的套接字
// void fatal(char *func, int rv);
class Bus
{
    BusMulticast bus_multicast;
    private:
    char *ip;
    int port;
    vector <message> _queue;//定义消息队列
    vector <string> topics;//定义主题队列
    // vector <nng_socket>  bus_socks;//定义总线发送队列
    nng_socket bus_sock; //负责监听的套接字
    public:
        Bus(char *ip,int port);
        ~Bus()
        {
            nng_close(bus_sock);
            _queue.clear();
            topics.clear();
        }
        void __enter__();
        void __exit__();
        void publish(char* topic,PyObject* payload);//发送信息到总线，先传入队列中
        void subscribe(char* topic);
        void close();
        void loop_start();
        void on_message(char* topic,PyObject* payload);
        void _recv_thread(function <void(char*,PyObject*)> func);
        void _send_thread();
        void recv(void (*func)(char*,PyObject*));
        //void _notify_thread();
        void display();
};

#endif