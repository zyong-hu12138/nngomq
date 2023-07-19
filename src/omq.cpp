#include "omq.h"
#include <iostream>
using namespace std;
catcherror ce;
ReqRepMulticast udp_node;
void catcherror::error_print()
{
    char *temp;
    if(error!=NULL && error!=temp)
    {
        cout<<error<<endl;
        
        temp = error;
    }
}