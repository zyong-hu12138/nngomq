#include "omq.h"
#include <iostream>
using namespace std;
catcherror ce;
void catcherror::error_print()
{
    
    if(error!=NULL && error!=temp)
    {
        cout<<error<<endl;
        
        char *temp = error;
    }
}