#include "reg.h"
#include "ui_reg.h"
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
reg::reg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reg)
{
    ui->setupUi(this);
    pparent = qobject_cast<MainWindow *>(parentWidget()->parentWidget());
    key=0;
}

reg::~reg()
{
    delete ui;
}
//注册 保存到数据库
void reg::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty()&&!ui->lineEdit_2->text().isEmpty())
    {
        pparent->qm->setQuery(QString("insert into AMDtable values(\"%1\",%2);"). \
                            arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text()),pparent->mydb);
        //错误提示
        if(pparent->qm->lastError().isValid())
              qDebug() << pparent->qm->lastError();
        delete this;
    }
    else
        QMessageBox::information(this,"提示","注册失败：注册信息不全！");
}
//关闭
void reg::on_pushButton_2_clicked()
{
    delete this;
}
//软键盘
void reg::on_pushButton_3_clicked()
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
