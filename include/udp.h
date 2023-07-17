#ifndef UDP_H_
#define UDP_H_
#include "parameter.h"
#include "utils.h"
#include "pickle.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>

using  namespace std;
void fatal(const char *func, int rv);

class BusMulticast
{
    public:
    // char *udp_url="tcp://239.100.0.2:52000";
    char *udp_ip = "239.0.0.1";
    int udp_port = 50001;
    vector <char*> urllist;
    vector <char*> recv_urllist;
    int udp_sock;
    int urllist_size=0;
    sockaddr_in udp_url{};
    sockaddr_in local_addr{};
    public:
    BusMulticast(){};
    ~BusMulticast() {};
    // void _send(char *str);
    void multi_create(char *ip,int port);//正确创建组播组并绑定以及加入组播组
    void multi_listen();//监听组播组
    void multi_send(char *ip,int port);//组播信息
    void upgrade();//更新url列表
    void loop(char *ip,int port);//创建线程，循环组播
};

#endif