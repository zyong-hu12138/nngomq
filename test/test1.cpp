#include "utils.h"
#include "bus.h"
#include "parameter.h"
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
    cout<<1<<endl;
    char *topic1 = "hello";
    char *payload1 = "world";
    char *topic2 = "hi";
    bus1.display();
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