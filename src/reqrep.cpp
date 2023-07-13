#include "reqrep.h"
#include "utils.h"
// #include "udp.h"
#include <vector>
#include <thread>
#include <unistd.h>
#define SEPARATOR "^&*;"
using namespace std;
// ReqRepMulticast *udp;
//REQ类中函数的实现
void REQ::_enter()
{
    int rv;
    if((rv = nng_req0_open(&req_sock)) != 0)
        fatal("nng_setopt_ms", rv);
}
void REQ::_exit()
{
    nng_close(req_sock);
}
void REQ::_close()
{
    nng_close(req_sock);
    _queue.clear();
}
void REQ::_send(char *topic,char *payload)
{
    int rv;
    char *buf = NULL;
    Py_ssize_t sz;
    char *str = (char *)malloc(strlen(topic) + strlen(payload) + strlen(SEPARATOR) + 1);
    sprintf(str, "%s%s%s", topic, SEPARATOR, payload);
    PyObject *pyBytes = congverStringToBytes(str);
    // if((rv = nng_send(req_sock, msg, strlen(msg) + 1, 0)) != 0)
    //     fatal("nng_send", rv);
    PyBytes_AsStringAndSize(pyBytes, &buf, &sz);
    if((rv = nng_send(req_sock, buf, sz, 0)) != 0)
        fatal("nng_send", rv);
    free(str);
}
void REQ::_send_thread()
{
    while(1)
    {
        if(_queue.size() > 0)
        {
            message msg = _queue[0];
            _queue.erase(_queue.begin());
            _send(msg.topic,msg.payload);
            
            int rv;
            char *buf = NULL;
            size_t sz;
            if((rv = nng_recv(req_sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0)
                fatal("nng_recv", rv);
            getPyObjectAsString(buf,sz,buf);//反序列化后的结果
            char *topic = strtok(buf, SEPARATOR);
            char *payload = strtok(NULL, SEPARATOR);
            cout<<"req _recv"<<endl;
            cout<<topic<<payload<<endl;
        }
    }
}
message REQ::send(char *topic,char *payload)
{
    if(is_async)
    { 
        message msg={topic,payload};
        _queue.push_back(msg);
        return msg;
    }
    else
    {   
        _send(topic,payload);
        char *buf = NULL;
        size_t sz;
        int rv;
        if((rv = nng_recv(req_sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0)
            fatal("nng_recv", rv);
        getPyObjectAsString(buf,sz,buf);//反序列化后的结果
        char *topic = strtok(buf, SEPARATOR);
        char *payload = strtok(NULL, SEPARATOR);
        cout<<topic<<payload<<endl;
        message msg={topic,payload};
        return msg;
    }
}
void REQ::_recv()
{
    int rv;

    sleep(0.01);
    while(1)
    {
        char *buf = NULL;
        size_t sz;
        cout<<2<<endl;
        if((rv = nng_recv(req_sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0)
            fatal("nng_recv", rv);
        
        if(rv == 0)
        {
        getPyObjectAsString(buf,sz,buf);//反序列化后的结果
        char *topic = strtok(buf, SEPARATOR);
        char *payload = strtok(NULL, SEPARATOR);
        cout<<"req _recv"<<endl;
        cout<<topic<<payload<<endl;
        exit(0);
        }

    }
}
void REQ::recv()
{
    thread tid(&REQ::_recv,this);
    tid.detach();
}
//REP类中函数的实现
void REP::_enter()
{
    int rv;
    if((rv = nng_rep0_open(&rep_sock)) != 0)
        fatal("nng_rep0_open", rv);
}
void REP::_exit()
{
    nng_close(rep_sock);
}
void REP::main_thread(void (*func)(char* ,char *))
{
    int rv;
    char *buf = NULL;
    size_t sz;
    // cout<<2<<endl;
    sleep(0.01);
    while(1)
    {
        if((rv = nng_recv(rep_sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0)
            fatal("nng_recv", rv);
        if(rv==0)
        {
        cout<<"rep recv"<<endl;
        getPyObjectAsString(buf,sz,buf);//反序列化后的结果
        char *topic = strtok(buf, SEPARATOR);
        char *payload = strtok(NULL, SEPARATOR);
        // cout<<topic<<payload<<endl;
        message msg={topic,payload};
        _queue.push_back(msg);
        printf("REP GET: %s %s\n", topic, payload);
        if(func!=NULL)
        {
            func(topic,payload);
        }
        else{
            cout<<topic<<":"<<payload<<endl;
        }
        PyObject *pyBytes = congverStringToBytes("ok");
        Py_ssize_t sz;
        PyBytes_AsStringAndSize(pyBytes, &buf, &sz);
        cout<<"rep send"<<endl;
        if((rv = nng_send(rep_sock, buf,sz, 0)) != 0)
            fatal("nng_send", rv);
        // nng_free(buf,strlen(buf)+1);
        }
        
        
    }
}
void REP::loop_start(void (*func)(char* ,char *))
{
    thread tid(&REP::main_thread,this,func);
    tid.detach();//接收消息的线程
}
void REP::loop_forever(void (*func)(char* ,char *))
{
    main_thread(func);
}
void REP::reply(char *topic,char *payload)
{
    int rv;
    char *buf = NULL;
    size_t sz;
    char *msg = (char *)malloc(strlen(topic) + strlen(payload) + strlen(SEPARATOR) + 1);
    sprintf(msg, "%s%s%s", topic, SEPARATOR, payload);
    // cout<<"rep reply"<<endl;
    if((rv = nng_send(rep_sock, msg, strlen(msg) + 1, 0)) != 0)
        fatal("nng_send", rv);
        // cout<<"rep reply"<<endl;
        sleep(0.01);
    free(msg);
}

void fatal(const char *func, int rv)
{
    printf("%s: %s\n", func, nng_strerror(rv));
    exit(1);
}