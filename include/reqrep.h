#ifndef REQREP_H_
#define REQREP_H_
#include "parameter.h"
#include "utils.h"
#include "udp.h"

#include <vector>
#include <thread>
#include <unistd.h>
#include <nng/nng.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>
#include "pickle.h"
using namespace std;
extern vector<message> msg_recv;
// extern ReqRepMulticast *udp ;
//未实现UDP功能
// void fatal(const char *func, int rv);
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
        Address self_address;
        int _send_count;
        vector<message> _queue;
        // ReqRepMulticast udp_node;
    public:
        REQ(Address nameaddr,int send_timeout,int recv_timeout ,bool is_async )
        {
            self_address = nameaddr;
            target = {nameaddr.ip,nameaddr.port};
            int rv;
            if((rv = nng_req0_open(&req_sock)) != 0)
                fatal("nng_req0_open", rv);//创建socket
            if((rv = nng_socket_set_ms(req_sock, NNG_OPT_SENDTIMEO, send_timeout)) != 0)
                fatal("nng_setopt_ms", rv);
            if((rv = nng_socket_set_ms(req_sock, NNG_OPT_RECVTIMEO, recv_timeout)) != 0)
                fatal("nng_setopt_ms", rv);
            char url[20];
            sprintf(url, "tcp://%s:%d", target.ip,target.port);
            if((rv = nng_dial(req_sock,url, NULL, 0)) != 0)
                fatal("nng_dial", rv);//连接到指定的URL
            this->is_async = is_async;
            _send_count = 0;

            if(is_async)
            { 
                thread tid(&REQ::_send_thread,this);
                tid.detach();
            }
            // udp_node.listen_loop();
        }
        ~REQ()
        {
            nng_close(req_sock);
           
        }   
        void _enter();
        void _exit();
        void _close();
        void _send(char *topic,PyObject *payload);
        message send(char *topic,PyObject *payload);
        void _send_thread();
        void _recv();
        void recv();
        char* find_ip();
        int find_port();
        int set_timeout(int send_timeout,int recv_timeout);
};
extern vector <REQ> reqlist;
class REP  //响应请求
{
    private:
        nng_socket rep_sock;
        Target addr;
        char *name;
        int _send_count;
        int port;
        ReqRepMulticast udp;
        vector<message> _queue;
    public:
        REP(Address nameaddr)
        {
            addr={nameaddr.ip,nameaddr.port};
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
    
            sprintf(url, "tcp://%s:%d", addr.ip,port);
            if((rv = nng_listen(rep_sock, url, NULL, 0)) == 0)
            {
                printf("listen to port %d\n",port);
                // break;
            }
            udp.loop(nameaddr,SELF_IP,addr.port);  //服务器不断发送消息，告知组播组中自己的ip变化
        }
        ~REP()
        {
            nng_close(rep_sock);
            _queue.clear();
        }   
        void _enter();
        void _exit();
        void main_thread(message (*func)(char *,PyObject *));
        void _close();
       // void notify_thread();暂时不考虑
        void loop_start(message (*func)(char *,PyObject *));
        void loop_forever(message (*func)(char *,PyObject *));

};
void req(Address name,char *topic,PyObject *payload,int send_timeout = 1000,int recv_timeout = 1000 ,bool is_async = true);
#endif