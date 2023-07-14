#ifndef UDP_H_
#define UDP_H_
#include "parameter.h"
#include "utils.h"
#include "pickle.h"
#include <iostream>
#include <vector>
#include <nng/nng.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/protocol/bus0/bus.h>
#include <algorithm>
using  namespace std;
void fatal(const char *func, int rv);
class ReqRepMulticast
{
    private:
    char* udp_ip = "239.100.0.1";
    int udp_port=52000;
    nng_socket udp_sock;
    vector <string> addrlist;
    vector <string> recv_addrlist;
    public:
    ReqRepMulticast()
    {
        int rv=0;
        if(rv = nng_req0_open(&udp_sock) != 0)
            fatal("nng_bus_open", rv); //创建套接字
    }
    ~ReqRepMulticast()
    {
        nng_close(udp_sock);
    }
    // void _send(char *str);
    void update_addr(char *url);//接收其他节点端口，更新本地端口
    void noify();//通知其他节点，本节点端口
}; 

class BusMulticast
{
    public:
    char* udp_ip = "239.100.0.2";
    int udp_port = 52000;
    // char *udp_url="tcp://239.100.0.2:52000";
    char *udp_url="tcp://127.0.0.1:5200";
    vector <char*> urllist;
    vector <char*> recv_urllist;
    nng_socket udp_sock_recv;
    nng_socket udp_sock_send;
    int urllist_size=0;
    public:
    BusMulticast(){};
    ~BusMulticast()
    {
        nng_close(udp_sock_recv);
        nng_close(udp_sock_send);
    }
    // void _send(char *str);
    void recv_addr();//接收其他节点端口，压入recv-url
    void update_addr();
    void notify(char *url);//通知其他节点，本节点端口
    void loop(char *url);
};

#endif