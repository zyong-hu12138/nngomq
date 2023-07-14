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
    char *ip = "127.0.0.1";
    int port = 50001;
    vector <char*> urllist;
    vector <char*> recv_urllist;
    int udp_sock=socket(AF_INET,SOCK_DGRAM,0);
    int urllist_size=0;
    sockaddr_in udp_url{};
    public:
    BusMulticast(){};
    ~BusMulticast() {};
    // void _send(char *str);
    void recv_addr();//接收其他节点端口，压入recv-url
    void update_addr();
    void notify(char *url);//通知其他节点，本节点端口
    void loop(char *url);
};

#endif