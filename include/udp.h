#include "parameter.h"
#include "utils.h"
#include "pickle.h"
#include <iostream>
#include <vector>
#include <nng/nng.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/protocol/bus0/bus.h>
#include <algorithm>
using  namespace std;
void fatal(const char *func, int rv)
{
    cout<<"error"<<"\n"<<func<<rv<<endl;
    exit(1);
}
class ReqRepMulticast
{
    private:
    char* udp_ip = "239.100.0.1";
    int udp_port=52000;
    nng_socket udp_sock;
    vector <string> addrlist;
    vector <string> recv_addrlist;
    public:
    ReqRepMulticast()
    {
        int rv=0;
        if(rv = nng_req0_open(&udp_sock) != 0)
            fatal("nng_bus_open", rv); //创建套接字
    }
    ~ReqRepMulticast()
    {
        nng_close(udp_sock);
    }
    // void _send(char *str);
    void update_addr(char *url);//接收其他节点端口，更新本地端口
    void noify();//通知其他节点，本节点端口
}; 
class BusMulticast
{
    public:
    char* udp_ip = "239.100.0.2";
    int udp_port = 52000;
    char *udp_url="tcp://239.100.0.2:52000";
    vector <string> urllist;
    vector <string> recv_urllist;
    nng_socket udp_sock;
    int urllist_size;
    public:
    BusMulticast()
    {
        int rv=0;
        if(rv = nng_bus0_open(&udp_sock) != 0)
            fatal("nng_bus_open", rv); //创建套接字
    }
    ~BusMulticast()
    {
        nng_close(udp_sock);
    }
    // void _send(char *str);
    void update_addr();//接收其他节点端口，更新本地端口
    void notify();//通知其他节点，本节点端口
    void loop();
};