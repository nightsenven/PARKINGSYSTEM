#ifndef NEWBUTTON_H
#define NEWBUTTON_H

#include <QPushButton>
#include <QMap>
class newbutton : public QPushButton
{
    Q_OBJECT
    int usearea,notusearea,flag;
    QString Area;
    QMap<QString,int> kvp;
    QList<int> button;
public:
    explicit newbutton(QMap<QString,int> kvp,QWidget *parent = 0);
    void get();
signals:

public slots:
    void select();
};

#endif // NEWBUTTON_H
