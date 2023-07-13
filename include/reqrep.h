#ifndef REQREP_H_
#define REQREP_H_
#include "parameter.h"
#include "utils.h"
// #include "udp.h"
#include <vector>
#include <thread>
#include <unistd.h>
#include <nng/nng.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>
using namespace std;
// extern ReqRepMulticast *udp ;
//未实现UDP功能
void fatal(const char *func, int rv);
struct Target
{
    char *ip;
    int port;
}; 
class REQ
{
    private:
        nng_socket req_sock;
        int send_timeout;
        int recv_timeout;
        bool is_async;
        Target target;
        char *name;
        int _send_count;
        vector<message> _queue;
    public:
        REQ(Target target,int send_timeout=100,int recv_timeout = 100,bool is_async = 0)
        {
            int rv;
            if((rv = nng_req0_open(&req_sock)) != 0)
                fatal("nng_req0_open", rv);//创建socket
            if((rv = nng_socket_set_ms(req_sock, NNG_OPT_SENDTIMEO, send_timeout)) != 0)
                fatal("nng_setopt_ms", rv);
            if((rv = nng_socket_set_ms(req_sock, NNG_OPT_RECVTIMEO, recv_timeout)) != 0)
                fatal("nng_setopt_ms", rv);
            char url[20];
            sprintf(url, "tcp://%s:%d", target.ip,target.port);
            cout<<"req::"<<url<<endl;
            if((rv = nng_dial(req_sock,url, NULL, 0)) != 0)
                fatal("nng_dial", rv);//连接到指定的URL
            is_async = is_async;
            _send_count = 0;
            // if(is_async)
            // {
            //     thread tid(&REQ::_send_thread,this);
            //     tid.detach();
            // }
        }
        ~REQ()
        {
            nng_close(req_sock);
            _queue.clear();
        }   
        void _enter();
        void _exit();
        void _close();
        void _send(char *topic,char *payload);
        message send(char *topic,char *payload);
        void _send_thread();
        void _recv();
        void recv();
};

class REP  //响应请求
{
    private:
        nng_socket rep_sock;
        Target addr;
        char *name;
        int _send_count;
        int port;
        vector<message> _queue;
    public:
        REP(Target addr)
        {
            addr=addr;
            int rv;
            if((rv = nng_rep0_open(&rep_sock)) != 0)
                fatal("nng_rep0_open", rv);//创建socket
            if((rv = nng_socket_set_ms(rep_sock, NNG_OPT_RECVTIMEO, 1000)) != 0)
                fatal("nng_setopt_ms", rv);
            char url[20];
            //监听端口
            port = addr.port;
            // while(1)
            // {
            sprintf(url, "tcp://%s:%d", addr,port);
            if((rv = nng_listen(rep_sock, url, NULL, 0)) == 0)
            {
                printf("listen to port %d\n",port);
                // break;
            }
                // else if (rv !=0)
                // {    
                //     printf(" port %d In Use\n",port);
                //     sleep(0.1);
                //     break;
                // }
            // }
        }
        ~REP()
        {
            nng_close(rep_sock);
            _queue.clear();
        }   
        void _enter();
        void _exit();
        void main_thread();
        void _close();
       // void notify_thread();暂时不考虑
        void loop_start();
        void loop_forever();
        void reply(char *topic,char *payload);
        
};

#endif