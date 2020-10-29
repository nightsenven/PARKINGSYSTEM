#ifndef AMDS_H
#define AMDS_H

#include <QDialog>
#include "mainwindow.h"
#include "mysqlquerymodel.h"
#include "keyboradl.h"
namespace Ui {
class AMDS;
}

class AMDS : public QDialog
{
    Q_OBJECT

public:
    explicit AMDS(QWidget *parent = 0);
    ~AMDS();

private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::AMDS *ui;
    mysqlquerymodel qm;
    int key;
    keyboradl *mykey;
};

#endif // AMDS_H
