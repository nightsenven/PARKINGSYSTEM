#include "amds.h"
#include "ui_amds.h"
#include <QDebug>

AMDS::AMDS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AMDS)
{
    ui->setupUi(this);
    //qm.setQuery("select * from "+ui->tablename->text());

    key=0;
    ui->tableView->setModel(&qm);
    //根据内容自动调整列宽
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //设置tableview的表头
    QSqlQuery query(qobject_cast<MainWindow *>(parentWidget())->mydb);
    qm.setQuery(query);
    if(query.exec("select * from "+ui->tablename->text()))
    {
        QSqlRecord record = query.record();

        int row=0,column=0;
//        while(1)
//        {
//            if(i>=record.count())
//                break;
//            qm.setHeaderData(i++,Qt::Horizontal,"车主姓名");
//            qm.setHeaderData(i++,Qt::Horizontal,"车牌号");
//            qm.setHeaderData(i++,Qt::Horizontal,"车主号码");
//            qm.setHeaderData(i++,Qt::Horizontal,"停车位置");
//        }
        while(query.next())
        {
            //表格数据居中显示
            for(column=0;column<record.count();column++)
                query.value(column) = qm.data(ui->tableView->model()->index(row,column));
            row++;
        }
    }
}

AMDS::~AMDS()
{
    delete ui;
}
//关闭
void AMDS::on_pushButton_5_clicked()
{
    delete this;
}
//查询表格
void AMDS::on_pushButton_3_clicked()
{
    if(!ui->tablename->text().isEmpty())
    {
        qm.setQuery("select * from "+ui->tablename->text());
        //错误提示
        if(qm.lastError().isValid())
              qDebug() << qm.lastError();
    }

}
//修改计费标准
void AMDS::on_pushButton_clicked()
{
    if(!ui->cost->text().isEmpty())
        qobject_cast<MainWindow *>(parentWidget())->price1=ui->cost->text().toDouble();
}

void AMDS::on_pushButton_2_clicked()
{
    if(!ui->cost1->text().isEmpty())
        qobject_cast<MainWindow *>(parentWidget())->price2=ui->cost1->text().toDouble();
}

void AMDS::on_pushButton_4_clicked()
{
    if(!ui->cost2->text().isEmpty())
        qobject_cast<MainWindow *>(parentWidget())->price3=ui->cost2->text().toDouble();
}
//软键盘
void AMDS::on_pushButton_6_clicked()
{
    if(key==0)
    {
        key=1;
        mykey = new keyboradl(this);
        mykey->setGeometry(50,80,340,140);
        mykey->raise();
        mykey->show();
    }
    else
    {
        key=0;
        delete mykey;
    }
}
