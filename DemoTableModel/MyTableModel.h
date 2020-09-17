#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QAbstractTableModel>

/**
 * @brief 表格展示的一行的数据结构，随便写的
 */
struct MyModelItem
{
    QString name; //姓名
    int age; //年龄
    QString info; //相关信息
};

/**
 * @brief 自定义TableModel，用于展示接口的使用
 * @details
 * 继承QAbstractTableModel需要实现至少三个接口：
 * rowCount、columnCount、data
 */
class MyTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MyTableModel(QObject *parent = nullptr);

    //自定义导入导出数据的接口
    void setModelData(const QList<MyModelItem> &datas);
    QList<MyModelItem> getModelData() const;
    //自定义插入行数据
    bool insertModelData(int row,const MyModelItem &datas);
    //自定义设置表头数据
    void setHorHeaderData(const QList<QString> &headers);

    //获取表头数据
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    //设置表头数据
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    //获取行数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //获取列数
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    //获取单元格数据
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //设置单元格数据
    bool setData(const QModelIndex &index, const QVariant &value,int role = Qt::EditRole) override;
    //单元格的可操作性标志位，如可编辑，可选中等
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    //添加行列
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    //bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    //移除行列
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    //bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    //数据
    QList<MyModelItem> modelData;
    //横项列表头
    QList<QString> horHeaderData;
};


#endif // MYTABLEMODEL_H
