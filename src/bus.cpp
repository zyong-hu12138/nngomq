#include "bus.h"
#include <any>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <functional>
#include "pickle.h"
#define SEPARATOR "^&*;"

// vector <nng_socket> bus_socks; //用来监听连接的套接字
using namespace std;

Bus::Bus(char *ip,int port)
{
    int rv;
    if((rv = nng_bus0_open(&bus_sock)) != 0)
        sprintf(error,"nng_bus_open error:%s",strerror(errno));
    char url[20];
    sprintf(url, "tcp://%s:%d",ip,port);
    if((rv = nng_listen(bus_sock, url, NULL, 0)) != 0)
        fatal("nng_listen", rv);
    bus_multicast.loop(ip,port);
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
void Bus::publish(char* topic,PyObject* payload)
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
void Bus::recv(void (*func)(char*,PyObject*))
{
    function<void(char* ,PyObject*)> funcWrapper = func;
    thread tid(&Bus::_recv_thread,this,funcWrapper);
    tid.detach();
}
void Bus::_recv_thread(function <void(char*,PyObject*)> func)
{
    int rv;
    while(1)
    {   
        char *buf = NULL;
        size_t sz;
        size_t size;
        nng_socket tmp_sock = bus_sock; 
        if((rv = nng_recv(tmp_sock, &buf, &size, NNG_FLAG_ALLOC)) != 0)
            fatal("nng_recv", rv);
        if(rv==0)
        {
            char *topic = strtok(buf, SEPARATOR);
            char *payload_str = strtok(NULL, SEPARATOR);
            PyObject *payload ;
            getPyObjectAsString(payload_str,size-strlen(topic)-strlen(SEPARATOR),&payload);
            //得到反序列化后的payload
            
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
}
void Bus::_send_thread()
{
    int rv;
    while(1)
    {
        if(_queue.size() > 0)
        {
            message msg=_queue[0];
            char *topic=msg.topic;
            PyObject *payload=msg.payload;
            PyObject *pyBytes = congverStringToBytes(payload);
            char *buf = NULL;
            Py_ssize_t sz;
            PyBytes_AsStringAndSize(pyBytes, &buf, &sz);
            size_t size = strlen(topic) + strlen(SEPARATOR) + sz;
            char *str = new char[size];
            int len = sprintf(str, "%s%s", topic, SEPARATOR);
            memcpy(str + len, buf, sz);
            
            for(int i=0;i<bus_multicast.cnt;i++)
            {
                char *url = (char *)bus_multicast.urllist[i];
                nng_socket tmp_sock;
                if ((rv = nng_bus0_open(&tmp_sock)) != 0)
                    fatal("nng_bus_open", rv);
                if((rv = nng_dial(tmp_sock, url, NULL, 0)) != 0)
                    fatal("nng_dial", rv);
                if ((rv = nng_send(tmp_sock, str, sz, 0)) != 0)
                    fatal("nng_send", rv);
                nng_close(tmp_sock);
            }
            _queue.erase(_queue.begin());
           
        }
        else break;
    }
}
void Bus::on_message(char* topic,PyObject* payload)
{
    cout<<"topic:"<<topic<<endl;
    PyObject_Print(payload, stdout, Py_PRINT_RAW);
}

void Bus::display()
{
    cout<<"display"<<endl;
    cout<<bus_multicast.cnt<<endl;
    for(int i=0;i<bus_multicast.cnt;i++)
    {
        cout<<bus_multicast.urllist[i]<<endl;
    }
}
