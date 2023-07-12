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
    Bus bus1,bus2,bus3;
    char *topic1 = "hello";
    char *payload1 = "world";
    char *topic2 = "hi";
    bus2.subscribe(topic1);
    bus3.subscribe(topic2);
    bus1.dial("127.0.0.1",5003);
    // display();
    // bus2.listen();
    // cout<<"bus2 listen"<<endl;
    // bus3.listen();
    bus1.publish(topic1,payload1);
    bus1.publish(topic2,payload1);
    bus1.loop_start();
    bus2.loop_start();
    bus3.loop_start();


    return 0;
}
