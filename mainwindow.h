#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mycamera.h"
#include "mythread.h"
#include "myuart.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QSqlRecord>
#include <QVariant>
#include <QTimer>
#include <QTime>
#include <QDate>
#include "mybutton.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class newbutton;
    friend class AMDS;
    friend class login;
    friend class reg;
    friend class mybutton;
    friend class carinfo;
    friend class pay;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void opencamera();
    void closecamera();
    void set(int count);
    void add(long nowtime);
    void add();
    void set();
    void recovery();
    void setCliecked();
    void get(QString table);
    void get();
    void closetimer();
    bool isExists(QString name);
    void countcost();
    bool paym();
    void stoppoint();
public slots:
    void settime();
    void scanning();
private slots:
    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void rollchar();
    void rollchar1();
private:
    Ui::MainWindow *ui;
    mycamera *camera;
    mythread *cthread;
    myuart *uart;
    QSqlDatabase mydb;
    QSqlQueryModel *qm;
    QTimer mytimer;
    QTimer mytimer1;
    QTimer mytimer2;
    QTimer mytimer3;
    QTime mytime;
    QDate mydate;

    QMap<QString,mybutton *> Area;
    QMap<QString,mybutton *>::const_iterator it;
    QMap<QString,int> useArea;
    QString carname; //车主姓名
    long carid;  //车牌号
    QString carphone; //车主号码
    QString carpp;  //停车的位置
    long intime;//入车时间
    int stopcost;  //停车费用
    double price1;
    double price2;
    double price3;
    int count,count1;
    int x,x1;
    int fontflag,fontflag1;
    int fontwidth,fontwidth1;
    QFont myfont,myfont1;
    int hascustomer;
    int carcount;
    QMap<QString,int> kvp;
    int ok;   //确定或者取消
};

#endif // MAINWINDOW_H
