#ifndef MYUART_H
#define MYUART_H

#include <QThread>
#include "myhead.h"
class myuart : public QThread
{
    Q_OBJECT
    friend class MainWindow;
    friend class newbutton;
    int fd;
    //无符号整型数组 int buf[6]
    struct timeval timeout;
    volatile unsigned int cardid ;
    int flag;
public:
    explicit myuart(QObject *parent = 0);
    ~myuart();
    int open_uart();
    void init_uart(int fd);
    int sendARequest(int fd);
    int sendBRequest(int fd);
    unsigned char CountBCC(unsigned char *buf,int n);
    void run();
signals:

public slots:
};

#endif // MYUART_H
