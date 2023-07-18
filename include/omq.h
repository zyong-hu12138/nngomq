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
            Py_Initialize();
            _init_();
        }
        ~catcherror()
        {
            Py_Finalize();
        }
        void error_print();
};
extern catcherror ce;