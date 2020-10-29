#include "mybutton.h"
#include "ui_mybutton.h"
#include "mainwindow.h"
mybutton::mybutton(QString Area, QMap<QString, int> kvp, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mybutton)
{
    this->Area=Area;
    ui->setupUi(this,kvp);
    hide();
    connect(&mytimer,SIGNAL(timeout()),this,SLOT(set()));
    mytimer.start(100);
}

mybutton::~mybutton()
{
    delete ui;
}

//查询表格信息
void mybutton::get()
{
    area.clear();
    QSqlQuery query(qobject_cast<MainWindow *>(parentWidget())->mydb);
    if(!query.exec(QString("select * from CARSPACES where areaname = \"%1区\"").arg(Area)))
    {
        qDebug()<<"查询失败";
        return;
    }
    while(query.next())
    {
        usearea = query.value(1).toInt();
        notusearea = 10-usearea;
        area.insert(ui->pushButton,query.value(3).toInt());
        area.insert(ui->pushButton_2,query.value(4).toInt());
        area.insert(ui->pushButton_3,query.value(5).toInt());
        area.insert(ui->pushButton_4,query.value(6).toInt());
        area.insert(ui->pushButton_5,query.value(7).toInt());
        area.insert(ui->pushButton_6,query.value(8).toInt());
        area.insert(ui->pushButton_7,query.value(9).toInt());
        area.insert(ui->pushButton_8,query.value(10).toInt());
        area.insert(ui->pushButton_9,query.value(11).toInt());
        area.insert(ui->pushButton_10,query.value(12).toInt());
    }
}
//设置界面
void mybutton::set()
{
    get();
    for(auto it = area.cbegin();it!=area.cend();it++)
        if(it.value()==1)
            it.key()->setStyleSheet("background-color: rgb(255, 0, 0);");
        else
            it.key()->setStyleSheet("background-color: rgb(200, 200, 200);");
}
