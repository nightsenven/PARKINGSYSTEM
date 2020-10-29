#ifndef REG_H
#define REG_H

#include <QDialog>
#include "keyboradl.h"
#include "mainwindow.h"
namespace Ui {
class reg;
}

class reg : public QDialog
{
    Q_OBJECT

public:
    explicit reg(QWidget *parent = 0);
    ~reg();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::reg *ui;
    int key;
    keyboradl *mykey;
    MainWindow *pparent;
};

#endif // REG_H
