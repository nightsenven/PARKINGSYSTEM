#include "newbutton.h"
#include "mainwindow.h"
#include <QDebug>
#include "mybutton.h"
#include <QMessageBox>
newbutton::newbutton(QMap<QString, int> kvp, QWidget *parent) : QPushButton(parent)
{
    Area = qobject_cast<mybutton *>(parent)->Area;
    connect(this,SIGNAL(clicked()),this,SLOT(select()));
    this->kvp = kvp;
    button.clear();
}
//查询表格信息
void newbutton::get()
{
    QSqlQuery query(qobject_cast<MainWindow *>(parentWidget()->parentWidget())->mydb);
    if(!query.exec(QString("select * from CARSPACES where areaname = \"%1区\"").arg(Area)))
    {
        qDebug()<<"查询失败";
        return;
    }
    QSqlRecord record = query.record();
    int removeindex;
    while(query.next())
    {
        usearea = query.value(1).toInt();
        notusearea = 10-usearea;
        for(int i=3;i<record.count();i++)
        {
            if(query.value(i).toInt()==1)
                button.append(i-2);
            else
            {
                removeindex = button.indexOf(i-2);
                if(removeindex!=-1)
                    button.removeAt(removeindex);
            }
        }
    }
}
//选中标志
void newbutton::select()
{
    MainWindow *myparents = qobject_cast<MainWindow *>(parentWidget()->parentWidget());
    if(myparents->hascustomer==0)
    {
        QMessageBox::information(myparents,"提示","您还未刷卡，请先刷卡再停车！");
        return;
    }
    //查询
    get();

    newbutton *bt = qobject_cast<newbutton *>(sender());
    if(button.indexOf(bt->text().toInt())!=-1)
    {
        QMessageBox::information(myparents,"提示","该车位已有车辆！请您换个车位。");
        return;
    }
    bt->setStyleSheet("background-color: rgb(255, 0, 0);");
    //bt->setChecked(true);
    //bt->setEnabled(false);

    //获取车主停车的位置
    myparents->carpp=Area+bt->text();

    //获取当前时间
    QTime nowtime = QTime::currentTime();
    //保存到数据库PartingS表格
    myparents->add(nowtime.hour()*60*60+nowtime.minute()*60+nowtime.second());
    usearea--;
    notusearea = 10-usearea;
    QString temp = kvp.key(bt->text().toInt());
    //修改表格信息
    myparents->qm->setQuery(QString("update CARSPACES set usearea=%1 where areaname = \"%2区\"").arg(usearea).arg(Area));
    myparents->qm->setQuery(QString("update CARSPACES set notusearea=%1 where areaname = \"%2区\"").arg(notusearea).arg(Area));
    myparents->qm->setQuery(QString("update CARSPACES set %1=1 where areaname = \"%2区\"").arg(temp).arg(Area));
    //错误提示
    if(myparents->qm->lastError().isValid())
          qDebug() << myparents->qm->lastError();
    button.append(bt->text().toInt());
    myparents->set(--(myparents->carcount));
    myparents->hascustomer=0;
    myparents->closetimer();
    QMessageBox::information(myparents,"提示","停车成功，祝你生活愉快！");

}
