#ifndef PAY_H
#define PAY_H

#include <QDialog>
#include <keyboradl.h>
namespace Ui {
class pay;
}

class pay : public QDialog
{
    Q_OBJECT

public:
    explicit pay(int cost, QString name,QString pointe,QWidget *parent = 0);
    ~pay();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::pay *ui;
    int cost;
    int key;
    keyboradl *mykey;
};

#endif // PAY_H
