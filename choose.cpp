#include "choose.h"
#include "ui_choose.h"
#include <QListWidgetItem>
#include "mainwindow.h"
#include "login.h"
#include <QDebug>
choose::choose(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::choose)
{
    ui->setupUi(this);
}

choose::~choose()
{
    delete ui;
}
//选择
void choose::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->text()=="登录")
    {
        login *newlogin = new login(qobject_cast<MainWindow *>(parentWidget()));
        qobject_cast<MainWindow *>(parentWidget())->setCliecked();
        close();
        newlogin->exec(); //模态显示
    }
    else if(item->text()=="取消")
    {
        qobject_cast<MainWindow *>(parentWidget())->setCliecked();
        close();
    }
}
