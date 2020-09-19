#ifndef MYSTYLEDDELEGATE_H
#define MYSTYLEDDELEGATE_H

#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>

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
};

#endif // MYSTYLEDDELEGATE_H
