#include "mysqlquerymodel.h"

mysqlquerymodel::mysqlquerymodel(QWidget *parent) : QSqlQueryModel(parent)
{

}
//表格数据居中显示
QVariant mysqlquerymodel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    /*
      * 单元格内容居中
      */
    if (Qt::TextAlignmentRole == role) {
        value = Qt::AlignCenter;
    }
    return value;
}
