#include "login.h"
#include "ui_login.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include "amds.h"
#include "mainwindow.h"
#include "reg.h"
#include <QDebug>
#include <QMessageBox>
login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    key=0;
    passflag=0;
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}

login::~login()
{
    delete ui;
}
//软键盘
void login::on_key_clicked()
{
    //打开软键盘
    if(key==0)
    {
        mykey = new keyboradl(this);
        mykey->setGeometry(30,140,341,141);
        mykey->raise();
        mykey->show();
        key=1;
    }
    else
    {
        key=0;
        delete mykey;
    }
}
//登录
void login::on_login_2_clicked()
{

    QSqlQuery query(qobject_cast<MainWindow *>(parentWidget())->mydb);
    if(!query.exec("select * from AMDtable"))
    {
        qDebug()<<"查询失败！";
        return ;
    }
     //获取保存查询到的结果
    QSqlRecord record = query.record();
    //验证登录
    while(query.next())
    {
        if(query.value(0).toString()==ui->lineEdit->text()&&query.value(1).toString()==ui->lineEdit_2->text())
        {
            AMDS *amd = new AMDS(qobject_cast<MainWindow *>(parentWidget()));
            delete this;
            amd->exec();
            return;
        }
    }
    QMessageBox::information(this,"提示","用户名或者密码错误！");
}
//关闭
void login::on_pushButton_4_clicked()
{
    delete this;
}
//注册
void login::on_reg_clicked()
{
    reg *newreg = new reg(this);
    newreg->exec();
}
//隐藏密码
void login::on_hide_clicked()
{
    if(passflag==0)
    {
        ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
        passflag=1;
    }
    else
    {
        ui->lineEdit_2->setEchoMode(QLineEdit::Password);
        passflag=0;
    }
}
