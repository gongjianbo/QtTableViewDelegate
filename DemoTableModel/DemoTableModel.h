#pragma once
#include <QWidget>
#include "MyTableModel.h"

namespace Ui {
class DemoTableModel;
}

/**
 * @brief 展示QTableView+QAbstractTableModel的基本使用
 */
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
    //设置model的数据
    void setData();
    //从model导出数据
    void getData();
    //添加行
    void addRow();
    //删除行
    void delRow();
    //排序开关
    void sortEnableChange();

private:
    Ui::DemoTableModel *ui;
    //自定义model的实例，可以封装到自定义的view中
    MyTableModel *model;
    //保存和恢复选中项
    QModelIndex selectedIndex;
};
