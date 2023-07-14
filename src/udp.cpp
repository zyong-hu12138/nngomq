#include "udp.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>>
using  namespace std;
void fatal(const char *func, int rv)
{
    cout<<func<<" error"<<":"<<strerror(errno)<<endl;
    // exit(1);
}
void BusMulticast::recv_addr()//通过接收到的消息更新本节点的节点列表
{
    while(1)
    {
        char *buf = NULL;
        size_t sz;
        int rv;
        if((rv = recv(udp_sock, &buf, sz, 0)) == 0)
            fatal("recv", rv);
        recv_urllist.push_back(buf);
    }
}

void BusMulticast::notify(char *url)//定时广播自己节点的位置
{
    int rv=0;
    sockaddr_in destinationAddress{};
    destinationAddress.sin_family = AF_INET;
    destinationAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 目标地址
    destinationAddress.sin_port = htons(50001); // 目标端口号
    while(1)
    {   
        sleep(0.5);//定时广播自己地址，
        if((rv = sendto(udp_sock, url, strlen(url)+1, 0, reinterpret_cast<sockaddr*>(&destinationAddress), sizeof(destinationAddress))) == 0)
            fatal("send", rv);
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
    
    udp_url.sin_family = AF_INET;
    udp_url.sin_addr.s_addr = inet_addr("127.0.0.1"); // 根据给定的 IP 地址设置
    udp_url.sin_port = htons(50001); // 根据给定的端口号设置
    int rv;
    int optionValue = 1;
    int reuse = 1;
    if (rv = bind(udp_sock, reinterpret_cast<sockaddr*>(&udp_url), sizeof(udp_url)) != 0)
        fatal("socket_bind", rv);
    if (setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) 
        std::cerr << "Failed to set SO_REUSEADDR option." << std::endl;

    // if (setsockopt(udp_sock, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1) 
    //     std::cerr << "Failed to set SO_REUSEPORT option." << std::endl;
    int multicastTTL = 3;
    if (rv = setsockopt(udp_sock, IPPROTO_IP, IP_MULTICAST_TTL, reinterpret_cast<const char*>(&multicastTTL), sizeof(multicastTTL)) != 0)
        fatal("setsockopt", rv);

    thread t1(&BusMulticast::notify, this , url);
    thread t2(&BusMulticast::recv_addr, this);
    thread t3(&BusMulticast::update_addr, this);
    t1.detach();
    t2.detach();
    t3.detach();
}