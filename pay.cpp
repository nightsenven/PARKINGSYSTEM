#include "pay.h"
#include "ui_pay.h"
#include <QMessageBox>
#include "mainwindow.h"
pay::pay(int cost, QString name, QString pointe, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pay)
{
    this->cost = cost;
    ui->setupUi(this);
    QString cmd = "亲爱的"+name+"您好，我是智能管理员小爱，很高兴为您服务。您在"+pointe+"位置的停车费用为"+QString::number(cost)+"元，请支付后出车，谢谢合作！";
    ui->textBrowser->setText(cmd);
    //ui->textBrowser->setAlignment(Qt::AlignCenter);
    key=0;
}

pay::~pay()
{
    delete ui;
}
//关闭取消
void pay::on_pushButton_2_clicked()
{
    qobject_cast<MainWindow *>(parentWidget())->ok=1;
    close();
}
//确定支付
void pay::on_pushButton_clicked()
{
    if(cost==ui->lineEdit->text().toInt())
    {
        QMessageBox::information(this,"提示","支付成功，祝您一路平安！");
        qobject_cast<MainWindow *>(parentWidget())->ok=0;
        close();
    }
    else if(cost>ui->lineEdit->text().toInt())
        QMessageBox::information(this,"提示","你好！您支付的余额不足,请重新输入！");
    else
    {
        QString smallchange = QString::number(ui->lineEdit->text().toInt()-cost);
        QString cmd = "你好！您支付了"+ui->lineEdit->text()+"元,找你"+smallchange+"元！";
        QMessageBox::information(this,"提示",cmd);
        qobject_cast<MainWindow *>(parentWidget())->ok=0;
        close();
    }
}
//软键盘
void pay::on_pushButton_3_clicked()
{
    //打开软键盘
    if(key==0)
    {
        mykey = new keyboradl(this);
        mykey->setGeometry(30,160,341,141);
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
