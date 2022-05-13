#pragma once
#include <QWidget>
#include <QTimer>
#include <QStandardItemModel>
#include <QStandardItem>

namespace Ui {
class DemoStandardModel;
}

/**
 * @brief 展示QTableView+QStandardItemModel的基本使用
 */
class DemoStandardModel : public QWidget
{
    Q_OBJECT
public:
    explicit DemoStandardModel(QWidget *parent = nullptr);
    ~DemoStandardModel();

private:
    void initModel();
    void initView();
    void initOperate();

public slots:
    //填充model的item
    void updateData();

private:
    Ui::DemoStandardModel *ui;
    //定时刷新
    QTimer *timer=nullptr;
    //固定列数
    static const int ColCount=10;
    //存储自定义数据的通用模型
    //文档https://doc.qt.io/qt-5/qstandarditemmodel.html
    QStandardItemModel *model=nullptr;
};
