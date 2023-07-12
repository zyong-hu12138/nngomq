#include<Python.h>
#include<iostream>
using namespace std;
int main()
{
    string c = "hello world!";
    Py_Initialize();//初始化python环境  
    PyRun_SimpleString("import pickle");//导入pickle模块
    PyRun_SimpleString("import io");//导入io模块
    PyObject *s = Py_BuildValue("s",c.c_str());//将c转换为python对象
    PyObject *obj;
    PyObject *f;
    PyRun_SimpleString("f = open('./pythonc/hello.pickle','wb')");//打开文件
    PyRun_SimpleString("pickle.dump(s,f)");//将s写入文件
    PyRun_SimpleString("f.close()");//关闭文件
    PyRun_SimpleString("f = open('./pythonc/hello.pickle','rb')");//打开文件
    PyRun_SimpleString("obj = pickle.load(f)");//从文件中读取s
    PyRun_SimpleString("f.close()");//关闭文件
    PyRun_SimpleString("print(obj)");//输出s
    Py_Finalize();//结束python环境
    return 0;
}