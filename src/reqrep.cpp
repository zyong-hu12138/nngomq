//size 用于nng_socket函数中，sz用于自己反序列化函数中
#include "reqrep.h"
#include "utils.h"
#include "udp.h"
#include <vector>
#include <thread>
#include <unistd.h>
#include "pickle.h"
#include <mutex>

#define SEPARATOR "^&*;"
using namespace std;
mutex mtx;//声明互斥锁对象
vector<message> msg_recv;//rep返回队列
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

}
void REQ::_send(char *topic, PyObject *payload)
{
    int rv;
    char *buf = NULL;
    Py_ssize_t sz;
    PyObject *pyBytes = congverStringToBytes(payload);
    PyBytes_AsStringAndSize(pyBytes, &buf, &sz);//将payload 序列化后转成char*

    size_t size = strlen(topic) + sz + strlen(SEPARATOR) ;
    char *str = new char[size];
    int len = sprintf(str, "%s%s", topic, SEPARATOR);
    memcpy(str + len, buf, sz);
    //发送序列化后的消息
    if((rv = nng_send(req_sock, str, size , 0)) != 0) //此处超时
        fatal("nng_send", rv);
    free(str);
}

void REQ::_send_thread()
{   
    while(1)
    {
        if(_queue.size() == 0) continue;
        if(_queue.size() > 0)
        {   
            message msg = _queue[0];
            _send(msg.topic,msg.payload);
            int rv;
            char *buf = NULL;
            size_t size;
            size_t sz;
            if((rv = nng_recv(req_sock,&buf, &size, NNG_FLAG_ALLOC)) != 0)
                fatal("nng_recv", rv);
            //得到topic和序列化后的payload
            char *topic = "is_async";
            PyObject *payload ;
            getPyObjectAsString(buf, size, &payload);
            //得到payload的值
           message tmp = {topic,payload};
            msg_recv.push_back(tmp);
            _queue.erase(_queue.begin());
        }
    }
}
PyObject* REQ::send(char *topic,PyObject *payload)
{
    if(is_async)
    { 
        message msg;
        msg = {topic,payload};
        _queue.push_back(msg);
        return payload;
    }
    else
    {  
        _send(topic,payload);
        char *buf = NULL;
        size_t size;
        size_t sz;
        int rv;
        if((rv = nng_recv(req_sock, &buf, &size, NNG_FLAG_ALLOC)) != 0)
            fatal("nng_recv", rv);
        //得到topic 和序列化后的payload
        PyObject *payload ;
        getPyObjectAsString(buf, size, &payload);
        return payload;
    }
}
void REQ::_recv()
{
    int rv;
    while(1)
    {
        char *buf = NULL;
        size_t size;
        size_t sz;
        if((rv = nng_recv(req_sock, &buf, &size, NNG_FLAG_ALLOC)) != 0)
            fatal("nng_recv", rv);
        
        if(rv == 0)
        {
            //得到topic和序列化后的payload
            char *topic = strtok(buf, SEPARATOR);
            char *payload_str = strtok(NULL, SEPARATOR);
            PyObject *payload ;
            
            getPyObjectAsString(payload_str,size-strlen(topic)-strlen(SEPARATOR), &payload);
            exit(0);
        }

    }
}
void REQ::recv()
{
    thread tid(&REQ::_recv,this);
    tid.detach();
}

//设置rep的超时时间,成功设置返回0，否则返回-1
int REQ::set_timeout(int send_timeout , int recv_timeout)
{
    int rv;
    if(rv = nng_socket_set_ms(req_sock, NNG_OPT_SENDTIMEO, send_timeout) != 0)
        return -1;
    if(rv = nng_socket_set_ms(req_sock, NNG_OPT_RECVTIMEO, recv_timeout) != 0)
        return -1;
    return 0;
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
void REP::main_thread(PyObject* (*func)(char* ,PyObject *))//接收消息并通过回调函数回复消息,回调函数接收主题和payload，返回消息体
{
    int rv;
    char *buf = NULL;
    size_t sz;
    size_t size;
    while(1)
    {
        if((rv = nng_recv(rep_sock, &buf, &size, NNG_FLAG_ALLOC)) != 0)
            fatal("nng_recv", rv);
        if(rv==0)
        {
        //得到topic和序列化后的payload
        char *topic = strtok(buf, SEPARATOR);
        char *payload_str = strtok(NULL, SEPARATOR);
        PyObject *payload ;
        getPyObjectAsString(payload_str,size,&payload);
        message msg={topic,payload};
        _queue.push_back(msg);
        if(func!=NULL)
        {
            PyObject* reply_payload = func(topic,payload); //pyobect
        
            char *buf;
            buf = (char*)malloc(1024*10*1024*sizeof(char));
            PyObject *pyBytes = congverStringToBytes(reply_payload);
            Py_ssize_t sz;
            PyBytes_AsStringAndSize(pyBytes, &buf, &sz);
            char *str = new char[sz];
            memcpy(str , buf, sz);
            size_t size = sz;
            if((rv = nng_send(rep_sock, str, size , 0)) != 0)
                fatal("nng_send", rv);
            // free(buf);
        }
        else{
            char *buf = NULL;
            char * reply_topic = "no message";
            PyObject *reply_payload = Py_None;
            PyObject *pyBytes = congverStringToBytes(payload);
            Py_ssize_t sz;
            PyBytes_AsStringAndSize(pyBytes, &buf, &sz);
            size_t size = strlen(reply_topic) + strlen(buf) + strlen(SEPARATOR) + 1;
            char *str = new char[size];
            sprintf(str, "%s%s%s", topic, SEPARATOR, buf);
            //无回调函数时返回tpopic和空的payload
        
            if((rv = nng_send(rep_sock, str , size , 0)) != 0)
                fatal("nng_send", rv);
        }
        
    }
    }
}
void REP::loop_start(PyObject* (*func)(char *,PyObject *))
{
    thread tid(&REP::main_thread,this,func);
    tid.detach();//接收消息的线程
}
void REP::loop_forever(PyObject* (*func)(char *,PyObject *))
{
    main_thread(func);
}

int REQ::find_port()
{
    return self_address.port;
}
char* REQ::find_ip()
{
    return self_address.ip;
}
vector <REQ> reqlist;


PyObject *req(Address name,char *topic,PyObject *payload,int send_timeout,int recv_timeout ,bool is_async)
{
    int name_port = name.port;
    char *name_ip = name.ip;
    if(reqlist.size() == 0)
    {
        // REQ req(name,send_timeout,recv_timeout,is_async);//is_async==false时可以实现，true _queue有问题
        reqlist.emplace_back(name,send_timeout,recv_timeout,is_async);
        PyObject* recv_payload = reqlist[0].send(topic,payload);
        return recv_payload;
    }
  
    else
    {
        int i=0;
        for(i=0;i<reqlist.size();i++)
        {

            if(reqlist[i].find_port() == name_port && strcmp(reqlist[i].find_ip(),name_ip) == 0)
            {
                lock_guard<mutex> lock(mtx);
                if(reqlist[i].set_timeout(send_timeout , recv_timeout) != 0)
                    printf("set timeout failure");
                PyObject *recv_payload = reqlist[i].send(topic,payload);
                return recv_payload;
            }

        }
        if(i == reqlist.size())
            {
                REQ req(name,send_timeout,recv_timeout,is_async);
                reqlist.push_back(req);
                PyObject *recv_payload = reqlist[i].send(topic,payload);
                return recv_payload;
            }
    }
}