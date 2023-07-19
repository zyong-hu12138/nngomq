#include <iostream>
#include "bus.h"
#include "pickle.h"
#include "parameter.h"
#include "reqrep.h"
#include "utils.h"
#include "udp.h"
///用该头文件即可

class catcherror
{
    public:
        catcherror()
        {
            _init_();
            Py_Initialize();
            pyPickleModule = PyImport_ImportModule("pickle");
            pyDumpsFunc = PyObject_GetAttrString(pyPickleModule, "dumps");
            pyLoadsFunc = PyObject_GetAttrString(pyPickleModule, "loads");
            ReqRepMulticast udp_node;
            udp_node.listen_loop();
        }
        ~catcherror()
        {
            Py_Finalize();
        }
        void error_print();
};
extern catcherror ce;