#include "mypushbutton.h"
#include <QDebug>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QApplication>

#include "keyboradl.h"
#include "mainwindow.h"

int mypushbutton::flag=0;
mypushbutton::mypushbutton(QWidget *parent) : QPushButton(parent)
{
    //关联信号与槽函数
    connect(this,SIGNAL(clicked()),this,SLOT(pressdown()));
}

//键盘按下
void mypushbutton::pressdown()
{
    //创建键盘事件
    QKeyEvent *keyevent;
    //获取按键按下的按键按钮
    mypushbutton *bt = qobject_cast<mypushbutton *>(sender());

    if(bt->text()=="Ent")    //回车键
    {
        keyevent=new QKeyEvent(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier,"");
    }
    else if(bt->text()=="Cap")  //大小写
    {
       if(flag==0) //大写
       {
           //把按钮变个颜色
           bt->setStyleSheet("background-color:rgb(255,255,255)");
           flag=1;
       }
       else     //小写
       {
           //把按钮变个颜色
           bt->setStyleSheet("background-color:rgb(200,200,200)");
           flag=0;
       }
        keyevent=new QKeyEvent(QEvent::KeyPress,Qt::Key_CapsLock,Qt::NoModifier,"");
    }
    else if(bt->text()=="Backspace")  //删除 //退格
    {
        keyevent=new QKeyEvent(QEvent::KeyPress,Qt::Key_Backspace,Qt::NoModifier,"");
    }
    else if(bt->text()=="Space")  //空格
    {
        keyevent=new QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier," ");
    }
    else
    {
        int key=bt->text().at(0).toLatin1();
        if(flag==0&&key>=65&&key<=91)  //小写
            keyevent = new QKeyEvent(QEvent::KeyPress,key,Qt::NoModifier, \
                                       QString(bt->text().at(0).toLatin1()+32));
        else
            keyevent = new QKeyEvent(QEvent::KeyPress,key,Qt::NoModifier,bt->text());

    }
    //发送该事件给输入框(本质上把事件发送给鼠标光标进入的那个组件)
    QCoreApplication::postEvent(QApplication::focusWidget(),keyevent);
}
