#pragma once
#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include "MyStyledDelegate.h"

namespace Ui {
class DemoStyledDelegate;
}

/**
 * @brief 展示QTableView+QStyledItemDelegate的基本使用
 */
class DemoStyledDelegate : public QWidget
{
    Q_OBJECT
public:
    explicit DemoStyledDelegate(QWidget *parent = nullptr);
    ~DemoStyledDelegate();

private:
    void initModel();
    void initDelegate();
    void initView();
    void initOperate();

private:
    Ui::DemoStyledDelegate *ui;
    //存储自定义数据的通用模型
    QStandardItemModel *model=nullptr;
    //自定义delegate
    MyStyledDelegate *delegate=nullptr;
};
