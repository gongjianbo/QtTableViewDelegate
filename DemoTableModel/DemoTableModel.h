#ifndef DEMOTABLEMODEL_H
#define DEMOTABLEMODEL_H

#include <QWidget>
#include "MyTableModel.h"

namespace Ui {
class DemoTableModel;
}

class DemoTableModel : public QWidget
{
    Q_OBJECT

public:
    explicit DemoTableModel(QWidget *parent = nullptr);
    ~DemoTableModel();

private:
    void initTable();
    void initOperate();

public slots:
    //填充model的item
    void updateData();

private:
    Ui::DemoTableModel *ui;

    MyTableModel *model;
};

#endif // DEMOTABLEMODEL_H
