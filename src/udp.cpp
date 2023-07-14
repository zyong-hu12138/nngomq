#include "udp.h"
#include <iostream>
#include <vector>
#include <nng/nng.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/protocol/bus0/bus.h>
#include <algorithm>
#include <thread>
using  namespace std;

void BusMulticast::update_addr()//通过这个函数将所有连接到udp地址的url告知给所有的列表
{
    int rv=0;
    char *buf = NULL;
    size_t sz;
    if((rv = nng_recv(udp_sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0)
        fatal("nng_recv", rv);
    auto it = find(urllist.begin(), urllist.end(), buf);
    if(it != urllist.end())
    {
        urllist.erase(it);
        recv_urllist.push_back(buf);
    }
    else
    {
        urllist.push_back(buf);
    }
}
void BusMulticast::notify()
{
    int rv=0;
    int tmp_size = urllist.size();
    if(tmp_size == this->urllist_size)
        return;
    else
    {
        this->urllist_size = tmp_size;
        for(int i=0;i<urllist.size();i++)
    {
        char *url = (char *)urllist[i].data();
        if((rv = nng_send(udp_sock, url, strlen(url)+1, 0)) != 0)
            fatal("nng_send", rv);
    }
    }
}
void BusMulticast::loop()
{
    thread t1(&BusMulticast::notify, this);
    thread t2(&BusMulticast::update_addr, this);
}