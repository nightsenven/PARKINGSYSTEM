#ifndef WHEELTEXT_H
#define WHEELTEXT_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QThread>
#include <QTest>
namespace Ui {
class wheeltext;
}

class wheeltext : public QWidget
{
    Q_OBJECT

public:
    explicit wheeltext(QWidget *parent = 0);
    ~wheeltext();

    void Delay_MSec(unsigned int msec);
    void paintEvent(QPaintEvent *event);
    void setcolor();
public slots:

private:
    Ui::wheeltext *ui;
    QTimer mytimer;
    int x;
    QFont myfont;
    QString w_text;
    int mywidth;
    int count;
    int i,j,k;
    int flag;
};

#endif // WHEELTEXT_H
