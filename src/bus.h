#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "udp.h"
#include "utils.h"
#include <nng/nng.h>
#include <nng/protocol/bus0/bus.h>
#include <any>
using namespace std;
#include <vector>
#include <thread>
#define SEPARATOR "^&*;"
//bus 通信urls还是必要的，记录已经有的节点dial 的地址，
extern vector <string> urls;
extern vector <nng_socket> bus_socks; //用来监听连接的套接字
class Bus
{
    private:
    int _base_port;
    int _end_port;
    char *ip;
    int port;
    vector <message> _queue;//定义消息队列
    vector <string> topics;//定义主题队列
    // vector <nng_socket>  bus_socks;//定义总线发送队列
    nng_socket bus_sock;
    public:
        Bus();
        // {
            // int rv;
            // if((rv = nng_bus0_open(&bus_sock)) != 0)
            //     fatal("nng_bus_open", rv);
            // while(1)
            // {
            //     for(int p = _base_port; p <= _end_port; p++)
            //     {   
            //         char url[20];
            //         sprintf(url, "tcp://0.0.0.0:%d", p);//注意组播函数处也可能错误，需要更改
            //         if((rv = nng_listen(bus_sock, Bus_URL, NULL, 0)) != 0)
            //         {
            //             fatal("nng_listen", rv);
            //             break;
            //         }    
            //     }
            // }
            
        // }
        ~Bus()
        {
            nng_close(bus_sock);
            _queue.clear();
            topics.clear();
        }
        void dial(char *ip,int port);//每个bus节点dial到一个端口，该端口被所有节点监听
        // void dial();
        // void listen(char *ip,int port);
        void listen();
        void __enter__();
        void __exit__();
        void publish(char* topic,char* payload);//发送信息到总线，先传入队列中
        void subscribe(char* topic);
        void close();
        void loop_start();
        void on_message(char* topic,char* payload);
        void _recv_thread();
        void _send_thread();
        //void _notify_thread();

};
void display();