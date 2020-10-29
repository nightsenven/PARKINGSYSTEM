#include "wheeltext.h"
#include "ui_wheeltext.h"

wheeltext::wheeltext(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wheeltext)
{
    ui->setupUi(this);
    i=0,j=50,k=100;
    flag=0;
    x=20;  //起始点
    count=0;
    myfont.setPointSize(20);
    myfont.setFamily("MV Boli");
    w_text = "WELCOM TO TDL-PARKING SPACE！";
    mywidth = QFontMetrics(myfont).width(w_text);

//    for(auto& i:temp)
//        w_text.append(i);
    //w_text="ＷＥＬＣＯＭＥ ＵＳＥ ＰＡＲＫＩＮＧ ＳＹＳＴＥＭ!";

    connect(&mytimer,SIGNAL(timeout()),this,SLOT(update()));
    mytimer.start(5);
}

wheeltext::~wheeltext()
{
    delete ui;
}


// 创建子事件循环，在子事件循环中，父事件循环仍然是可以执行的这种方法不会阻塞线程
void wheeltext::Delay_MSec(unsigned int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}
//画图工具显示字符串--//激活画家事件 //轮播字体
void wheeltext::paintEvent(QPaintEvent *event)
{
    //准备一个画家
    QPainter me(this);
    me.setFont(myfont);
    //准备画图工具
    QPen mypen(QColor(i,j,k));
    mypen.setStyle(Qt::SolidLine); //线条
    me.setPen(mypen);
    me.drawText(x,30,w_text.mid(count++,1));

    for(;count<=w_text.size();count++)
    {
        setcolor();
        QPen newpen(QColor(i,j,k));
        me.setPen(newpen);
        int newwidth = QFontMetrics(myfont).width(w_text.mid(0,count));
        me.drawText(x+newwidth,30,w_text.mid(count,1));
        QTest::qSleep(2);// 延时2毫秒
        //QThread::msleep(2);
        //Delay_MSec(5);
    }
    if(count>=w_text.size())
    {
        count=0;
    }
}

//设置字体颜色
void wheeltext::setcolor()
{
    if(flag==0)
    {
        i+=50;
        if(i>255)
        {
            i=255;
            j+=50;
            if(j>255)
            {
                j=255;
                k+=50;
                if(k>=200)
                {
                    k=200;
                    flag=1;
                }
            }
        }
    }
    else
    {
        k-=50;
        if(k<=100)
        {
            k=100;
            j-=50;
            if(j<=50)
            {
                j=50;
                i-=50;
                if(i<=0)
                {
                    i=0;
                    flag=0;
                }
            }
        }
    }
}





