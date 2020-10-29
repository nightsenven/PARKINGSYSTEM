#ifndef CARINFO_H
#define CARINFO_H

#include <QDialog>
#include "keyboradl.h"
namespace Ui {
class carinfo;
}

class carinfo : public QDialog
{
    Q_OBJECT

public:
    explicit carinfo(QWidget *parent = 0);
    ~carinfo();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::carinfo *ui;
    int key;
    keyboradl *mykey;
};

#endif // CARINFO_H
