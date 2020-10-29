#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "keyboradl.h"
namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private slots:
    void on_key_clicked();

    void on_login_2_clicked();

    void on_pushButton_4_clicked();

    void on_reg_clicked();

    void on_hide_clicked();

private:
    Ui::login *ui;
    int key;
    int passflag;
    keyboradl *mykey;
};

#endif // LOGIN_H
