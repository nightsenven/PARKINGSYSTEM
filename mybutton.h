#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include "newbutton.h"
#include <QDebug>
#include <QMap>
#include <QTimer>
namespace Ui {
class mybutton;
}

class mybutton : public QWidget
{
    Q_OBJECT
    friend class newbutton;
public:
    explicit mybutton(QString Area,QMap<QString,int> kvp, QWidget *parent = 0);
    ~mybutton();
    void get();
public slots:
    void set();
private:
    Ui::mybutton *ui;
    QMap<newbutton *,int> area;
    QString Area;
    int usearea,notusearea;
    QTimer mytimer;
};

#endif // MYBUTTON_H
