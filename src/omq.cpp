#include "omq.h"
#include <iostream>
using namespace std;
catcherror ce;
void catcherror::error_print()
{
    char *temp;
    if(error!=NULL && error!=temp)
    {
        cout<<error<<endl;
        
        temp = error;
    }
}