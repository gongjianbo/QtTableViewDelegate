#pragma once
//为了方便使用各控件，所以直接引入了整个widgets模块
//实际项目不要这么写
#include <QtWidgets>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>

/**
 * @brief 自定义QStyledItemDelegate，演示渲染和编辑的接口自定义
 * @note delegate 没有非编辑状态提供插入控件的接口，有两种方式
 * 1.view 的 setIndexWidget 插入显示静态数据的 widget
 * 2.delegate 的 paint 中使用 QStyle 的 drawControl 绘制
 */
class MyStyledDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MyStyledDelegate(QObject *parent = nullptr);

    //渲染相关的接口
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    //编辑相关的接口
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

private:
    //自定义的paint相关的辅助函数
    void initMyStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const;
    QString getDisplayText(const QModelIndex &index) const;
};
