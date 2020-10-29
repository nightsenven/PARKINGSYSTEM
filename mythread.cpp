#include "mythread.h"

mythread::mythread(QObject *parent) : QThread(parent)
{

}
//获取运行对象
void mythread::get(mycamera *camera)
{
    this->camera=camera;
}

//任务函数
void mythread::run()
{
    while(1)
    {
        camera->discamera();
    }
}
