#ifndef MYSQLQUERYMODEL_H
#define MYSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include "mainwindow.h"
class mysqlquerymodel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit mysqlquerymodel(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const override;
signals:

public slots:
};

#endif // MYSQLQUERYMODEL_H
