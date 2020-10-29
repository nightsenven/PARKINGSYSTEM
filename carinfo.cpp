#include "carinfo.h"
#include "ui_carinfo.h"
#include "mainwindow.h"
#include <QMessageBox>
carinfo::carinfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::carinfo)
{
    ui->setupUi(this);
    key=0;
}

carinfo::~carinfo()
{
    delete ui;
}
//关闭
void carinfo::on_pushButton_2_clicked()
{
    //车主姓名
    qobject_cast<MainWindow *>(parentWidget())->carname.clear();
    //车主号码
    qobject_cast<MainWindow *>(parentWidget())->carphone.clear();
    delete this;
}
//确认信息
void carinfo::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty()&&!ui->lineEdit_2->text().isEmpty())
    {
        //车主姓名
        qobject_cast<MainWindow *>(parentWidget())->carname=ui->lineEdit->text();
        //车主号码
        qobject_cast<MainWindow *>(parentWidget())->carphone=ui->lineEdit_2->text();
        delete this;
    }
    else
        QMessageBox::information(this,"提示","请输入完整的信息！");
}
//软键盘
void carinfo::on_pushButton_3_clicked()
{
    //打开软键盘
    if(key==0)
    {
        mykey = new keyboradl(this);
        mykey->setGeometry(30,150,341,141);
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
