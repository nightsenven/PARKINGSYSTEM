#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wheeltext.h"
#include <QSqlError>
#include <QDebug>
#include "choose.h"
#include <QFontMetrics>
#include <QFont>
#include "carinfo.h"
#include "pay.h"
#include <QMessageBox>
#include <QAbstractButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //打开摄像头
    camera = new mycamera();
    qm = new QSqlQueryModel();
    cthread = new mythread;
    cthread->get(camera);
    //定义并初始化串口
    uart=new myuart();
    //开启读卡模式
    uart->start();

    //安装数据库驱动
    mydb=QSqlDatabase::addDatabase("QSQLITE");

    //设置数据库名字
    mydb.setDatabaseName("ps.db");
    //打开并判断是否打开成功
    if(!mydb.open())
    {
        QSqlError err = mydb.lastError();
        qDebug()<<mydb.lastError();
        qDebug()<<err.text();
        qDebug()<<err.type();
        close();
    }
    hascustomer=0;
    ok=0;
    //创建数据库表格
    //QSqlQuery tempquery(mydb);
    qm->setQuery("create table if not exists PartingS(carname text not null,  \
                 carid int not null,phone text not null,partkingpoint text not null,\
                 nowtime int not null);",mydb);
    qm->setQuery("create table if not exists AMDtable(name text not null,   \
                 pass text not null);",mydb);
    qm->setQuery("create table CARSPACES(areaname text not null,usearea int not null,   \
                 notusearea int not null,one int not null,two int not null,\
                 three int not null,four int not null,five int not null,six int not null,\
                 seven int not null,eight int not null,nine int not null,ten int not null);",mydb);
    //错误提示
    if(qm->lastError().isValid())
          qDebug() << qm->lastError();
    else
        add();  //添加表格信息

    connect(&mytimer,SIGNAL(timeout()),this,SLOT(settime()));
    mytimer.start(1000);
    mytime = QTime::currentTime();
    mydate = QDate::currentDate();
    ui->time->display(mydate.toString()+QString(":")+mytime.toString());

    connect(&mytimer1,SIGNAL(timeout()),this,SLOT(rollchar()));
    mytimer1.start(300);
    connect(&mytimer3,SIGNAL(timeout()),this,SLOT(rollchar1()));
    //计费
    price1=10;
    price2=2;
    price3=20;
    get();    //获取剩余车位数
    set(carcount);
    set();
    for(int i=0;i<5;i++)
    {
        mybutton *bt = new mybutton(QString(QChar('A'+i)),kvp,this);
        Area.insert(QString(QChar('A'+i))+"区停车位",bt);
    }
    //显示车库
    it = Area.cbegin();
    it.value()->show();
    it.value()->setGeometry(QRect(90, 310, 611, 91));
    ui->label_5->setText(it.key());
    count=0;
    x=0;
    fontflag=0;
    fontwidth=0;
    count1=0;
    x1=0;
    fontflag1=0;
    fontwidth1=0;
    myfont.setFamily("楷体");
    myfont.setPointSize(20);
    ui->label_6->setFont(myfont);
    myfont1.setFamily("楷体");
    myfont1.setPointSize(14);
    ui->label_7->setFont(myfont1);
    connect(&mytimer2,SIGNAL(timeout()),this,SLOT(scanning()));
    mytimer2.start(100);
    opencamera();  //打开摄像头
}

MainWindow::~MainWindow()
{
    closecamera();  //关闭摄像头
    delete ui;
}
//设置剩余车位数量
void MainWindow::set(int count)
{
    ui->carcount->display(QString::number(count));
}
//添加数据库
void MainWindow::add(long nowtime)
{
    qm->setQuery(QString("insert into PartingS values(\"%1\",%2,\"%3\",\"%4\",%5);"). \
                        arg(carname).arg(carid).arg(carphone).\
                        arg(carpp).arg(nowtime),mydb);
}
//添加数据库表格信息
void MainWindow::add()
{
    for(int i=0;i<5;i++)
        qm->setQuery(QString("insert into CARSPACES values(\"%1\",10,0,0,0,0,0,0,0,0,0,0,0);").arg(QString(QChar('A'+i))+"区"),mydb);
}
//设置停车位置对应的编号
void MainWindow::set()
{
    kvp["one"]=1;
    kvp["two"]=2;
    kvp["three"]=3;
    kvp["four"]=4;
    kvp["five"]=5;
    kvp["six"]=6;
    kvp["seven"]=7;
    kvp["eight"]=8;
    kvp["nine"]=9;
    kvp["ten"]=10;
}
//还原
void MainWindow::recovery()
{
    ui->pprice->display(0);
    ui->ptime->display(0);
}
//设置按钮可点
void MainWindow::setCliecked()
{
    ui->pushButton_13->setEnabled(true);
}
//获取表格信息
void MainWindow::get(QString table)
{
    useArea.clear();
    QSqlQuery query(mydb);
    if(!query.exec("select * from "+table))
    {
        qDebug()<<"查询失败";
        return;
    }
    while(query.next())
    {
        //添加
        useArea.insert(query.value(0).toString(),query.value(1).toInt());
    }
}
//获取剩余车位数
void MainWindow::get()
{
    carcount=0;
    QSqlQuery query(mydb);
    if(!query.exec("select * from CARSPACES"))
    {
        qDebug()<<"查询失败";
        return;
    }
    while(query.next())
    {
        carcount+=query.value(1).toInt();
    }
}
//关闭
void MainWindow::closetimer()
{
    recovery();
    ui->label_7->clear();
    mytimer3.stop();
}
//判断该车主是否存在表中 已存在表示此时出车，不存在表示入车
bool MainWindow::isExists(QString name)
{
    QSqlQuery query(mydb);
    if(!query.exec(QString("select * from PartingS where carname=\"%1\"").arg(name)))
    {
        qDebug()<<"查询失败";
        return false;
    }
    while(query.next())
    {
        carpp = query.value(3).toString();
        intime = query.value(4).toInt();
        return true;   //存在
    }
    return false;  //不存在
}
//计算停车时间
void MainWindow::countcost()
{
    //计算停车时间
    QTime outtime = QTime::currentTime();
    long outtimes = outtime.hour()*3600+outtime.minute()*60+outtime.second();
    double stoptime = (outtimes-intime)/3600;
    if((int)stoptime<=2)
        stopcost=10;
    else
    {
        stopcost = ((int)stoptime+1)*2;
        if(stopcost>20)
            stopcost=20;
    }
    ui->pprice->display(stopcost);

    ui->ptime->display(QString::number(stoptime, 'f', 1));
}
//计算停车费用
bool MainWindow::paym()
{
    //打开对话框进行缴费
    pay mypay(stopcost,carname,carpp,this);
    mypay.exec();
    //判断按了哪个按钮
    if(ok==0)//点击了确定键
        return true;
    else     //点击了取消键
        return false;
}
//清空停车位置（修改）
void MainWindow::stoppoint()
{
    QString carwhere = carpp.mid(0,1);  //哪个区的车库
    QString carpoint = kvp.key(carpp.mid(1,1).toInt());  //具体哪个位置
    int usedcars = useArea.value(carwhere+"区");
    qm->setQuery(QString("update CARSPACES set usearea=%1 where areaname = \"%2区\"").arg(++usedcars).arg(carwhere));
    qm->setQuery(QString("update CARSPACES set notusearea=%1 where areaname = \"%2区\"").arg(10-usedcars).arg(carwhere));
    qm->setQuery(QString("update CARSPACES set %1=1 where areaname=\"%2区\"").arg(carpoint).arg(carwhere));
    get();    //获取剩余车位数
    set(carcount);
}

//设置时间
void MainWindow::settime()
{
    mytime = QTime::currentTime();
    mydate = QDate::currentDate();
    ui->time->display(mydate.toString()+QString(":")+mytime.toString());
}
//扫描
void MainWindow::scanning()
{
    //刷卡成功
    if(uart->flag==1)
    {

        carid = uart->cardid;
        hascustomer=1;
        uart->flag=0;
        carinfo *newcarinfo = new carinfo(this);
        newcarinfo->exec();
        //点击了确认键
        if(!carname.isEmpty()&&!carphone.isEmpty())
        {
            if(!isExists(carname)) //入车
            {
                //拍照
                camera->Photoshop(QString("%1_1").arg(carname).toLatin1().data());
                get("CARSPACES");  //获取车库使用情况
                mytimer3.start(300);
            }
            else    //出车
            {
                //拍照
                camera->Photoshop(QString("%1_2").arg(carname).toLatin1().data());
                get("CARSPACES"); //获取车库使用情况
                countcost();  //计算停车时间
                //计算停车费用
                while(1)
                {
                    if(paym())
                    {
                        //清空停车位置
                        stoppoint();
                        //删除数据库表格
                        qm->setQuery("delete from PartingS where carname='"+carname+"'");
                        hascustomer=0;
                        recovery();
                        break;
                    }
                    else
                    {
                        //出车失败
                        QMessageBox messageBox(QMessageBox::Information,"提示","缴费失败，无法出车！是否继续缴费？",QMessageBox::Yes | QMessageBox::No, this);
                        if(messageBox.exec()==QMessageBox::No)
                        {
                            hascustomer=0;
                            break;
                        }
                    }
                }
            }
        }
        else
            hascustomer=0;
    }
}

////打开摄像头
void MainWindow::opencamera()
{
    cthread->start();
}

////关闭摄像头
void MainWindow::closecamera()
{
    cthread->terminate();
    if(cthread->wait())
        qDebug()<<"关闭摄像头成功！";
}

//上一个车库
void MainWindow::on_pushButton_11_clicked()
{

    if(it==Area.cbegin())
        return;
    it.value()->hide();
    it--;
    it.value()->setGeometry(QRect(90, 310, 611, 91));
    it.value()->raise();
    it.value()->show();
    ui->label_5->setText(it.key());
}
//下一个车库
void MainWindow::on_pushButton_12_clicked()
{
    it.value()->hide();
    it++;
    if(it==Area.cend())
    {
        (--it).value()->show();
        return;
    }

    it.value()->setGeometry(QRect(90, 310, 611, 91));
    it.value()->raise();
    it.value()->show();
    ui->label_5->setText(it.key());
}
//管理员操作
void MainWindow::on_pushButton_13_clicked()
{
    choose *newchoose = new choose(this);
    newchoose->setGeometry(720,40,75,100);
    newchoose->show();
    ui->pushButton_13->setEnabled(false);
}
//滚动字幕
void MainWindow::rollchar()
{
    QString cmd = "好消息！好消息！本停车场限时优惠！从今日起停车超过三小时按每小时"+QString::number(price2)+"元计费，每天上限"+QString::number(price3)+"元！每天上限"+QString::number(price3)+"元！三小时以内包括三小时停车费用只要"+QString::number(price1)+"元！";
    if(fontflag==0)
    {
        if(fontwidth<=ui->label_6->width())
        {
            ui->label_6->setText(cmd.mid(x,++count));
            ui->label_6->setAlignment(Qt::AlignRight);
        }
        else
        {
            ui->label_6->setText(cmd.mid(++x,++count));
            ui->label_6->setAlignment(Qt::AlignLeft);
        }
    }
    else
    {
        ui->label_6->setText(cmd.mid(++x,--count));
        ui->label_6->setAlignment(Qt::AlignLeft);
    }
    //字符串的宽度
    fontwidth = QFontMetrics(myfont).width(cmd.mid(x,count));
    if(count>=cmd.size())
    {
        fontflag=1;
    }
    if(x>=cmd.size()+10)
    {
        x=0;
        count=0;
        fontflag=0;
    }
}
//滚动字幕
void MainWindow::rollchar1()
{
    int A = useArea.value("A区");
    int B = useArea.value("B区");
    int C = useArea.value("C区");
    int D = useArea.value("D区");
    int E = useArea.value("E区");
    QString cmd = QString("亲爱的%1你好！,当前车库A区还剩%2个位置,B区还剩%3个位置,C区还剩%4个位置,D区还剩%5个位置,E区还剩%6个位置。").
                    arg(carname).arg(A).arg(B).arg(C).
                    arg(D).arg(E);
    if(fontflag1==0)
    {
        if(fontwidth1<=ui->label_7->width())
        {
            ui->label_7->setText(cmd.mid(x1,++count1));
            ui->label_7->setAlignment(Qt::AlignRight);
        }
        else
        {
            ui->label_7->setText(cmd.mid(++x1,++count1));
            ui->label_7->setAlignment(Qt::AlignLeft);
        }
    }
    else
    {
        ui->label_7->setText(cmd.mid(++x1,--count1));
        ui->label_7->setAlignment(Qt::AlignLeft);
    }
    //字符串的宽度
    fontwidth1 = QFontMetrics(myfont1).width(cmd.mid(x1,count1));
    if(count1>=cmd.size())
    {
        fontflag1=1;
    }
    if(x1>=cmd.size()+10)
    {
        x1=0;
        count1=0;
        fontflag1=0;
    }
}
