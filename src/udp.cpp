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

void BusMulticast::multi_create(char *ip,int port)//正确创建组播组并绑定以及加入组播组
{
    // 创建套接字
    udp_sock = socket(AF_INET , SOCK_DGRAM , 0);
    if(udp_sock < 0)
        fatal("socket",errno);
    //设置套接字选项，允许重用本地地址和端口
    int reuse = 1;
    if(setsockopt(udp_sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))<0)
        fatal("setsockopt",errno);

    //设置组播地址和端口
    udp_url.sin_family = AF_INET;
    udp_url.sin_addr.s_addr = inet_addr(udp_ip);
    udp_url.sin_port = htons(udp_port);

    //绑定到本地地址和端口
    local_addr.sin_family = AF_INET;
    // local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_addr.s_addr = inet_addr(ip);
    local_addr.sin_port = htons(port);
    if(bind(udp_sock,reinterpret_cast<sockaddr*>(&local_addr),sizeof(local_addr))<0)
        fatal("bind",errno);
    if(setsockopt(udp_sock,IPPROTO_IP,IP_MULTICAST_IF,reinterpret_cast<char*>(&local_addr), sizeof(local_addr))<0)
        fatal("setsockopt",errno);
    //加入组播组
    ip_mreq multicast_req{};
    multicast_req.imr_multiaddr.s_addr = inet_addr(udp_ip);
    // multicast_req.imr_interface.s_addr = htonl(INADDR_ANY);
    multicast_req.imr_interface.s_addr = inet_addr(ip);
    multicast_req.imr_interface.s_addr = htonl(INADDR_ANY);
    // local_addr.imr_interface.s_addr = inet_addr(ip);
    if(setsockopt(udp_sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,reinterpret_cast<char*>(&multicast_req),sizeof(multicast_req))<0)
        fatal("setsockopt",errno);
}

void BusMulticast::multi_listen()//接收组播数据
{
    while(1)
    {
        char buf[1024];
        memset(buf, 0 ,sizeof(buf));
        struct sockaddr_in sender;
        socklen_t len = sizeof(struct sockaddr_in);
	    // int sender_len=sizeof(sender);
        int recv_size = recvfrom(udp_sock,buf,sizeof(buf),0,(struct sockaddr*)&sender,&len);
        if(recv_size == -1)
        {
            cout<<"Failed to receive data."<<endl;
            break;
        }
        else if(recv_size == 0)
        {
            cout<<"Connection closed."<<endl;
            break;
        }
        else
        {   
            cout<<"listen"<<buf<<endl;
            recv_urllist.push_back(buf);
        }
    }
}
//发送组播数据
void BusMulticast::multi_send(char *ip,int port)
{
    int fd = socket (AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
        fatal("socket",errno);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(udp_ip);
    addr.sin_port = htons(udp_port);

    char buf[1024];
    while(1)
    {
        sprintf(buf,"tcp://%s:%d",ip,port);
        int send_size = sendto(fd,buf,sizeof(buf),0,reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        if(send_size == -1)
        {
            cout<<"Failed to send data."<<endl;
            break;
        }
        else if(send_size == 0)
        {
            cout<<"Connection closed."<<endl;
            break;
        }
        else
        {
            sleep(5);
        }
    }
}
void BusMulticast::upgrade()
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

void BusMulticast::loop(char *ip,int port)
{
    multi_create(ip,port);
    thread tid1(&BusMulticast::multi_listen,this);
    thread tid2(&BusMulticast::multi_send,this,ip,port);
    thread tid3(&BusMulticast::upgrade,this);
    tid1.detach();
    tid2.detach();
    tid3.detach();
}