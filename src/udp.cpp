#include "udp.h"
#include <iostream>
#include <vector>
#include <nng/nng.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/protocol/bus0/bus.h>
#include <algorithm>
#include <thread>
#include <unistd.h>
using  namespace std;

void fatal(const char *func, int rv)
{
    fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
    exit(1);
}
void BusMulticast::recv_addr()//通过接收到的消息更新本节点的节点列表
{
    int rv=0;
    if(rv = nng_bus0_open(&udp_sock_recv) != 0)
        fatal("nng_bus_open", rv); //创建套接
    if((rv = nng_listen(udp_sock_recv, udp_url, NULL, 0)) != 0)
        fatal("nng_listen", rv);
    while(1)
    {
        char *buf = NULL;
        size_t sz;
        if((rv = nng_recv(udp_sock_recv, &buf, &sz, NNG_FLAG_ALLOC)) != 0)
            fatal("nng_recv", rv);
        recv_urllist.push_back(buf);
        nng_free(buf, sz);
    }
}

void BusMulticast::notify(char *url)//定时广播自己节点的位置
{
    int rv=0;
    if(rv = nng_bus0_open(&udp_sock_send) != 0)
        fatal("nng_bus_open", rv); //创建套接字
        sleep(0.5);
    if((rv = nng_dial(udp_sock_send, udp_url ,NULL, 0)) != 0)
        fatal("nng_dial", rv);
    while(1)
    {   
        if((rv = nng_send(udp_sock_send, url, strlen(url)+1, 0)) != 0)
            fatal("nng_send", rv);
        sleep(0.5);//定时广播自己地址，
    }
}
void BusMulticast::update_addr()//更新
{
    while(1)
    {
        if(recv_urllist.size()>0)
        {
            char *url=recv_urllist[0];
            recv_urllist.erase(recv_urllist.begin());
            auto it = find(urllist.begin(), urllist.end(), url);
            if(it != urllist.end())
                continue;
            else
                urllist.push_back(url);
        }
    }
}
void BusMulticast::loop(char *url)
{
    thread t1(&BusMulticast::notify, this , url);
    thread t2(&BusMulticast::recv_addr, this);
    thread t3(&BusMulticast::update_addr, this);
    t1.detach();
    t2.detach();
    t3.detach();
}