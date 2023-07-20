#include "udp.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <chrono>
using  namespace std;
void fatal(const char *func, int rv)
{
    sprintf(error,"%s error:%s",func,strerror(errno));
    // cin.ignore();
    // cout<<func<<" error:"<<strerror(errno)<<endl;
}

listen_multicasr udp_node;
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
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // local_addr.sin_addr.s_addr = inet_addr(ip);
    local_addr.sin_port = htons(udp_port);

    if(bind(udp_sock,reinterpret_cast<sockaddr*>(&local_addr),sizeof(local_addr))<0)
        fatal("bind",errno);
    struct in_addr if_addr;
    if_addr.s_addr = inet_addr(SELF_IP);
    if(setsockopt(udp_sock,IPPROTO_IP,IP_MULTICAST_IF,&if_addr, sizeof(if_addr))<0)
        fatal("setsockopt",errno);

    //加入组播组
    ip_mreq multicast_req{};
    multicast_req.imr_multiaddr.s_addr = inet_addr(udp_ip);
    multicast_req.imr_interface.s_addr = htonl(INADDR_ANY);//self_ip

    if(setsockopt(udp_sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,reinterpret_cast<char*>(&multicast_req),sizeof(multicast_req))<0)
        fatal("setsockopt",errno);
    

}

void BusMulticast::multi_listen()//接收组播数据
{
    struct sockaddr_in sender;
    socklen_t sender_len=sizeof(sender);
    // cout << "multi listen!!!!!!" << endl;
    while(1)
    {
        char buf[1024];
        memset(buf, 0 ,sizeof(buf));
        struct sockaddr_in sender;
        socklen_t len = sizeof(struct sockaddr_in);
  
	    // int sender_len=sizeof(sender);
        int recv_size = recvfrom(udp_sock,buf,sizeof(buf)-1,0,  (struct sockaddr*)&sender,&sender_len);
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
        else if(buf!=NULL)
        {  
            // if(urllist.size()==0)  urllist.push_back(buf);
            // if(urllist.size()>0)
            // {
            //     auto it = find(urllist.begin(), urllist.end(), buf);
            //     if(it != urllist.end())
            //     {
            //         continue;
            //     }    
            //     else if(it == urllist.end())
            //     {    
            //         cout<<"url"<<buf<<endl;
            //         urllist.push_back(buf);
            //     }
            // }
            if(cnt==0) 
            {
                urllist[cnt] = (char*)malloc(sizeof(buf));
                strcpy(urllist[cnt],buf);
                
                cnt++;
            }//初始加入的节点
            int flag=0;
            for (int i=0;i<cnt;i++)
            {
                // cout << urllist[i] << ": " << buf << endl;
                if(strcmp(urllist[i],buf)==0)
                {
                    flag=1;
                    break;
                }
            }//判断是否已经存在
            if(flag==0) 
            {
                // cout<< "new url"<<endl;
                urllist[cnt] = (char*)malloc(sizeof(buf));
                strcpy(urllist[cnt],buf);
                cnt++;
                // cout<<cnt<<endl;
            }
        }
    }
}
//发送组播数据
void BusMulticast::multi_send(char *ip,int port)
{
    char buf[1024];
    while(1)
    {
        memset(buf, 0 ,sizeof(buf));
        sprintf(buf,"tcp://%s:%d",ip,port);
        int send_size = sendto(udp_sock,buf,sizeof(buf)-1,0,(struct sockaddr *)&udp_url,sizeof(udp_url));
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
        usleep(500000);
        }
    }
}

void BusMulticast::loop(char *ip,int port)
{
    multi_create(ip,port);
    // thread tid3(&BusMulticast::upgrade,this);
    thread tid1(&BusMulticast::multi_listen,this);
    thread tid2(&BusMulticast::multi_send,this,ip,port);
    // tid3.detach();
    tid1.detach();
    tid2.detach();
}

void ReqRepMulticast::multi_create()
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
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // local_addr.sin_addr.s_addr = inet_addr(ip);
    local_addr.sin_port = htons(udp_port);

    if(bind(udp_sock,reinterpret_cast<sockaddr*>(&local_addr),sizeof(local_addr))<0)
        fatal("bind",errno);
    
    struct in_addr if_addr;
    if_addr.s_addr = inet_addr(SELF_IP);
    if(setsockopt(udp_sock,IPPROTO_IP,IP_MULTICAST_IF,&if_addr, sizeof(if_addr))<0)
        fatal("setsockopt",errno);
    //加入组播组
    ip_mreq multicast_req{};
    multicast_req.imr_multiaddr.s_addr = inet_addr(udp_ip);
    multicast_req.imr_interface.s_addr = htonl(INADDR_ANY);//self_ip

    if(setsockopt(udp_sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,reinterpret_cast<char*>(&multicast_req),sizeof(multicast_req)) != 0)
        fatal("setsockopt",errno);
    
    }
void ReqRepMulticast::multi_listen()
{
    struct sockaddr_in sender;
    socklen_t sender_len=sizeof(sender);
    char *ip;
    while(1)
    {
        char buf[1024];
        memset(buf, 0 ,sizeof(buf));
        struct sockaddr_in sender;
        socklen_t len = sizeof(struct sockaddr_in);
        int recv_size = recvfrom(udp_sock,buf,sizeof(buf) - 1,0,(struct sockaddr*)&sender,&sender_len);
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
        else if(buf!=NULL)
        {   
            char *name = strtok(buf,SEPARATOR);
            char *ip = strtok(NULL,SEPARATOR);
            int port = atoi(strtok(NULL,SEPARATOR));
            // if(sender.sin_family == AF_INET)
            // {
            //     ip = inet_ntoa(sender.sin_addr);
            // }
            // if(sender.sin_family == AF_INET6)
            // {
            //     inet_ntop(AF_INET6, &sender.sin_addr, ip, sizeof(ip));
            // }
            if(strcmp(ip,get_name_ip(buf)) == 0)
                continue;
            else if(strcmp(ip,get_name_ip(buf)) != 0)
            {

                set_name_ip(name,ip);
                
            }//更新addresslib中的ip值
        }
    }
}

void ReqRepMulticast::multi_send(Address name,char *ip,int port)
{
    char buf[1024];
    while(1)
    {
        memset(buf, 0 ,sizeof(buf));
        sprintf(buf,"%s%s%s%s%d",name.name,SEPARATOR,ip,SEPARATOR,port);
        int send_size = sendto(udp_sock,buf,sizeof(buf)-1,0,(struct sockaddr *)&udp_url,sizeof(udp_url));
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
            usleep(500000);
        }
    }
}
void ReqRepMulticast::loop(Address name,char *ip,int port)
{
    multi_create();
    thread tid2(&ReqRepMulticast::multi_send,this,name,ip,port);
    tid2.detach();
}
void ReqRepMulticast::listen_loop()
{
    multi_create();
    thread tid1(&ReqRepMulticast::multi_listen,this);
    tid1.detach();
}