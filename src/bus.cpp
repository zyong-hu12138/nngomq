#include "bus.h"
#include <any>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <functional>
#define SEPARATOR "^&*;"
 //负责组播的套接字;
BusMulticast bus_multicast;
// vector <nng_socket> bus_socks; //用来监听连接的套接字
using namespace std;

Bus::Bus(char *ip,int port)
{
    int rv;
    if((rv = nng_bus0_open(&bus_sock)) != 0)
        fatal("nng_bus_open", rv);
    if((rv = nng_socket_set_ms(bus_sock, NNG_OPT_RECVTIMEO, 10000)) != 0)
        fatal("nng_socket_set_ms", rv);
    char url[20];
    sprintf(url, "tcp://%s:%d",ip,port);
    // urls.push_back(url);
    // cout<<url<<endl;
    if((rv = nng_listen(bus_sock, url, NULL, 0)) != 0)
        fatal("nng_listen", rv);
    bus_multicast.loop(url);
    //告诉组播地址加入bus节点
}

void Bus::__enter__()
{
    int rv;
    if((rv = nng_bus0_open(&bus_sock)) != 0)
        fatal("nng_bus_open", rv);
}
void Bus::__exit__()
{
    nng_close(bus_sock);
}
void Bus::publish(char* topic,char* payload)
{
    message msg={topic,payload};
    _queue.push_back(msg);
    cout<<"publish sucess"<<endl;
}
void Bus::subscribe(char* topic)
{
    cout<<"subscribe"<<endl;
    topics.push_back(topic);
}
void Bus::close()
{
    nng_close(bus_sock);
    _queue.clear();
    topics.clear();
}
void Bus::loop_start()
{
    // thread tid4(&Bus::listen,this);
    // thread tid1(&Bus::_send_thread,this);
    // thread tid2(&Bus::_recv_thread,this);
//    thread tid3(&Bus::_notify_thread,this);
    // tid1.detach();
    // tid2.detach();
    // tid3.detach();
    // tid4.detach();
    _send_thread();
    // _recv_thread();
}
void Bus::recv(void (*func)(char*,char*))
{
    function<void(char* ,char*)> funcWrapper = func;
    thread tid(&Bus::_recv_thread,this,funcWrapper);
    tid.detach();
}
void Bus::_recv_thread(function <void(char*,char*)> func)
{
    int rv;
    char *buf = NULL;
    size_t sz;
    while(1)
    {    
        nng_socket tmp_sock = bus_sock; 
        if((rv = nng_recv(tmp_sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0)
            // cout<<"recv error"<<endl;
            fatal("nng_recv", rv);
        char *topic = strtok(buf, SEPARATOR);
        char *payload = strtok(NULL, SEPARATOR);
        // cout<<topic<<payload<<endl;
        for(int i=0;i<topics.size();i++)
        {
            char *t=(char*)topics[i].data();
            if(strcmp(topic,t) == 0)
            {   
                if(func != NULL)
                {
                    func(topic,payload);
                }
                else{
                    on_message(topic,payload);
                }
                
            }
        }
    }
}
void Bus::_send_thread()
{
    int rv;
    // cout<<_queue.size()<<endl;
    while(1)
    {
        if(_queue.size() > 0)
        {
            message msg=_queue[0];
            char *topic=msg.topic;
            char *payload=msg.payload;
            char *buf = new char[strlen(topic) + strlen(SEPARATOR) + strlen(payload) + 1];
            sprintf(buf, "%s%s%s", topic, SEPARATOR, payload);
            // cout<<"buf"<<endl;
            // cout<<buf<<endl;
            // if((rv = nng_send(bus_sock, buf, strlen(buf)+1, 0)) != 0)
            //     fatal("nng_send", rv);
            for (int i = 0; i < bus_multicast.urllist.size(); i++)
            {
                char *url = (char *)bus_multicast.urllist[i];
                nng_socket tmp_sock;
                if ((rv = nng_bus0_open(&tmp_sock)) != 0)
                    fatal("nng_bus_open", rv);
                if((rv = nng_dial(tmp_sock, url, NULL, 0)) != 0)
                    fatal("nng_dial", rv);
                if ((rv = nng_send(tmp_sock, buf, strlen(buf) + 1, 0)) != 0)
                    fatal("nng_send", rv);
                nng_close(tmp_sock);
            }
            // sleep(0.01);
            // if((rv = nng_send(bus_sock, buf, strlen(buf)+1, 0)) != 0)
            //     fatal("nng_send", rv);
            _queue.erase(_queue.begin());
            nng_free(buf, strlen(buf)+1);
        }
    }
}
void Bus::on_message(char* topic,char* payload)
{
    cout<<"topic:"<<topic<<endl;
    cout<<"payload:" <<payload<<endl;
}

void Bus::display()
{
    cout<<"display"<<endl;
    for(int i=0;i<bus_multicast.urllist.size();i++)
    {
        cout<<bus_multicast.urllist[i]<<endl;
    }
}

// void Bus::dial(char *ip,int port)//每个节点对应一个发送端口
// {
//     int rv;
//     nng_socket tmp_sock;
//     char url[20];
//     sprintf(url, "tcp://%s:%d", ip,port);
//     if((rv = nng_bus0_open(&tmp_sock)) != 0)
//         fatal("nng_bus_open", rv);
//     if((rv = nng_listen(tmp_sock, url, NULL, 0)) != 0)
//         fatal("nng_listen", rv);
//     if((rv = nng_dial(bus_sock, url, NULL, 0)) != 0)
//         fatal("nng_dial", rv);
//     bus_socks.push_back(tmp_sock);
//     urls.push_back(url);
//     // cout<<"dial sucess"<<endl;
//     // cout<<bus_socks.size()<<endl;
// }
// void Bus::dial()
// {
//     int rv;
//     for(int i=0;i<urls.size();i++)
//     {
//         char *url=(char*)urls[i].data();
//         // cout<<url<<endl;
//         nng_socket sock;
//         if((rv = nng_bus0_open(&sock)) != 0)
//             fatal("nng_bus_open", rv);
//         bus_socks.push_back(sock);
//         if((rv = nng_dial(sock, url, NULL, 0)) != 0)
//             fatal("nng_dial", rv);
//     }
// }

// void Bus::listen(char *ip,int port)
// {
//     int rv;
//     nng_socket tmp_sock;
//     char url[20];
//     sprintf(url, "tcp://%s:%d",ip,port);
//     urls.push_back(url);
//     // cout<<url<<endl;
//     if((rv = nng_listen(bus_sock, url, NULL, 0)) != 0)
//         fatal("nng_listen", rv);
//     // bus_socks.push_back(bus_sock);
   
// }
// void Bus::listen()
// {
//     int rv;
    
//     // cout<<urls.size()<<endl;
//     bus_socks.clear();
//     for(int i=0;i<urls.size();i++)
//     {
//         nng_socket tmp_sock;
//         char *url=(char*)urls[i].data();
//         // cout<<url<<endl;
//         if((rv = nng_bus0_open(&tmp_sock)) != 0)
//             fatal("nng_bus_open", rv);
//         if((rv = nng_listen(tmp_sock, url, NULL, 0)) != 0)
//             fatal("nng_listen", rv);
//         cout<<"listen sucess"<<endl;
//         bus_socks.push_back(tmp_sock);
//         nng_close(tmp_sock);
//     }
//     cout<<bus_socks.size()<<endl;
// }