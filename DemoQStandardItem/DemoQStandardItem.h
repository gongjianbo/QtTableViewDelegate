#ifndef DEMOQSTANDARDITEM_H
#define DEMOQSTANDARDITEM_H

#include <QWidget>
#include <QTimer>
#include <QStandardItemModel>
#include <QStandardItem>

namespace Ui {
class DemoQStandardItem;
}

/**
 * @brief 展示QTableView+QStandardItemModel的基本使用
 */
class DemoQStandardItem : public QWidget
{
    Q_OBJECT
public:
    explicit DemoQStandardItem(QWidget *parent = nullptr);
    ~DemoQStandardItem();

private:
    void initModel();
    void initView();
    void initOperate();

public slots:
    //填充model的item
    void updateData();

private:
    Ui::DemoQStandardItem *ui;
    //定时刷新
    QTimer *timer=nullptr;
    //固定列数
    static const int ColCount=10;
    //存储自定义数据的通用模型
    //文档https://doc.qt.io/qt-5/qstandarditemmodel.html
    QStandardItemModel *model=nullptr;
};

#endif // DEMOQSTANDARDITEM_H
