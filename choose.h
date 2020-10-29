#ifndef CHOOSE_H
#define CHOOSE_H

#include <QWidget>
#include <QListWidgetItem>
namespace Ui {
class choose;
}

class choose : public QWidget
{
    Q_OBJECT

public:
    explicit choose(QWidget *parent = 0);
    ~choose();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::choose *ui;
};

#endif // CHOOSE_H
