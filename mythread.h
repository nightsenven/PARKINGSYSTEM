#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "mycamera.h"
class mythread : public QThread
{
    Q_OBJECT
public:
    explicit mythread(QObject *parent = 0);
    void get(mycamera *camera);
    void run();
signals:

public slots:

private:
    mycamera *camera;
};

#endif // MYTHREAD_H
