#include "omq.h"
#include <vector>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
using namespace std;
#pragma warning
PyObject* convertIntToPyObject(int value);

int main()
{   
    REP rep(Addrlib.test);
    char topic[24] = "hello";
    PyObject *payload = convertIntToPyObject(1);
    PyObject* callback(char *topic,PyObject *payload);
    rep.loop_start(callback);//rep接收消息
    PyObject* recv = req(Addrlib.test,topic,payload,100,100,false);  
    sleep(1);
    //python printf(msg_recv[0].payload);
    PyObject_Print(recv, stdout, Py_PRINT_RAW);
    return 0;
}
PyObject*  callback(char *topic,PyObject* payload)
{
    PyObject *tmp = convertIntToPyObject(1);
    if(payload == tmp)
    {
        cout<<"callback"<<endl;
        // msg.payload = PyUnicode_FromString("ajvfskvsadkalsn");
        // msg.payload = convertIntToPyObject(1);
        // msg.payload = convertIntToPyObject(1);
        PyObject *argModule = PyImport_ImportModule("argparse");
        PyObject* namespaceClass = PyObject_GetAttrString(argModule, "Namespace");
        PyObject* aObj = PyObject_CallObject(namespaceClass, nullptr);

        PyObject* globals = PyDict_New();
        PyObject* locals = PyDict_New();

        // 创建一个Python字典用于存储 {'front_long': 1, 'front_short': 0, 'left_fish': 2}
        PyObject* cameraIdDict = PyDict_New();
        PyDict_SetItemString(cameraIdDict, "front_long", PyLong_FromLong(1));
        PyDict_SetItemString(cameraIdDict, "front_short", PyLong_FromLong(0));
        PyDict_SetItemString(cameraIdDict, "left_fish", PyLong_FromLong(2));

        // 将创建的字典赋值给 aObj.camera_id
        PyObject_SetAttrString(aObj, "camera_id", cameraIdDict);

        return aObj;
    }
    else
    {
        cout<<"error"<<endl;
        PyObject *payload = convertIntToPyObject(0);
        return payload;
    }
    // cout<<"callback"<<endl;
    // printf("topic:%s,payload:%s\n",topic,payload);
}
PyObject* convertIntToPyObject(int value) {
    return PyLong_FromLong(value);
}
