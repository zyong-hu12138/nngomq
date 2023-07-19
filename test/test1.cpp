#include "utils.h"
#include "bus.h"
#include "parameter.h"
#include "udp.h"
#include <vector>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
using namespace std;
int main()
{
    void callback(char*topic,char*payload);
    Bus bus1("127.0.0.1",5001),bus2("127.0.0.1",5002),bus3("127.0.0.1",5003);
    sleep(1);
    char *topic1 = "hello";
    char *payload1 = "world";
    char *topic2 = "hi";
    bus3.display();
    bus2.subscribe(topic1);
    bus3.subscribe(topic2);
    bus1.publish(topic1,payload1);
    bus1.publish(topic2,payload1);
    bus2.recv(*callback);
    bus3.recv(*callback);
    bus1._send_thread();
    return 0;
}
void callback(char*topic,char*payload)
{
    cout<<"callback"<<endl;
    printf("topic:%s,payload:%s\n",topic,payload);
}

// int main()
// {
//     BusMulticast node1,node2,node3;
//     node1.multi_create("127.0.0.1",5001);
//     node2.multi_create("127.0.0.1",5002);
//     node3.multi_create("127.0.0.1",5003);
//     node1.multi_send("127.0.0.1",5001);
//     node1.multi_send("hello",5001);
//     node2.multi_listen();
//     node3.multi_listen();
//     node2.upgrade();
//     for(int i=0;i<node2.urllist.size();i++)
//     {
//         cout<<node2.urllist[i]<<endl;
//     }
//     node1.multi_send("hello",5001);
//     node2.multi_listen();
//     node3.multi_listen();
//     node2.upgrade();
//     for(int i=0;i<node2.urllist.size();i++)
//     {
//         cout<<node2.urllist[i]<<endl;
//     }
// }
