#include "MyTableModel.h"

MyTableModel::MyTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

void MyTableModel::setModelData(const QList<MyModelItem> &datas)
{
    //重置model数据之前调用beginResetModel，此时会触发modelAboutToBeReset信号
    beginResetModel();
    //重置model中的数据
    modelData=datas;
    //数据设置结束后调用endResetModel，此时会触发modelReset信号
    endResetModel();
    //注意：reset model后，选中的item会失效，我们可以自己写保存和恢复选中项的逻辑
    //如果表的行列数是固定的，只是数据变更了，我们可以用 dataChanged 信号来请求刷新。
    //emit dataChanged(index(0,0),index(RowMax-1,ColMax-1),QVector<int>());
}

QList<MyModelItem> MyTableModel::getModelData() const
{
    //将内存数据返回
    return modelData;
}

bool MyTableModel::insertModelData(int row, const MyModelItem &datas)
{
    //row为0就是开始，为rowcount就在尾巴
    if(row<0||row>rowCount())
        return false;
    //需要将操作放到beginInsertRows和endInsertRows两个函数调用之间
    beginInsertRows(QModelIndex(), row, row);
    //在接口对应行插入空数据
    modelData.insert(row,datas);
    endInsertRows();
    return true;
}

void MyTableModel::setHorHeaderData(const QList<QString> &headers)
{
    //自定义的表头设置接口
    horHeaderData=headers;
    emit headerDataChanged(Qt::Horizontal, 0, headers.count()-1);
}

QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //注意，如果用了sortproxymodel，这个section是实际数据的index，不是界面看到的index
    //区分横表头和竖表头
    if(orientation == Qt::Horizontal){
        //这里我们只设置居中对齐和文本
        if (role == Qt::DisplayRole){
            //这里把横项列表头的文本设计为可以设置的
            if(section>=0 && section<horHeaderData.count())
                return horHeaderData.at(section);
            return QString("Col %1").arg(section + 1);
        }else if(role == Qt::TextAlignmentRole){
            return Qt::AlignCenter;
        }
    }else{
        if (role == Qt::DisplayRole)
            return QString("Row %1").arg(section + 1);
        else if(role == Qt::TextAlignmentRole)
            return Qt::AlignCenter;
    }
    return QVariant();
}

bool MyTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    //设计为横项列表头可以设置
    if (orientation == Qt::Horizontal && section>=0 && section<horHeaderData.count()) {
        horHeaderData[section] = value.toString();
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int MyTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    //返回表格行数
    return modelData.count();
}

int MyTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    //返回表格列数
    return 3;
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        //DisplayRole返回显示的文本值
        const int row = index.row();
        switch(index.column())
        {
        case 0: return modelData.at(row).name;
        case 1: return modelData.at(row).age;
        case 2: return modelData.at(row).info;
        }
    }
    return QVariant();
}

bool MyTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //将界面修改的值进行保存
    if (index.isValid() && role == Qt::EditRole) {
        const int row = index.row();
        switch(index.column())
        {
        case 0: modelData[row].name = value.toString(); break;
        case 1: modelData[row].age = value.toInt(); break;
        case 2: modelData[row].info = value.toString(); break;
        }
        //发送信号触发刷新
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags MyTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    //单元格允许的操作，至少得是ItemIsEnabled的才能进行其他操作
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable;
}

bool MyTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    //row为0就是开始，为rowcount就在尾巴
    if(row<0||count<1||row>rowCount())
        return false;
    //需要将操作放到beginInsertRows和endInsertRows两个函数调用之间
    beginInsertRows(parent, row, row + count - 1);
    for(int i=row;i<row+count;i++)
    {
        //在接口对应行插入空数据
        modelData.insert(i,MyModelItem());
    }
    endInsertRows();
    return true;
}

bool MyTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(row<0||count<1||row+count>rowCount())
        return false;
    //需要将操作放到beginRemoveRows和endRemoveRows两个函数调用之间
    beginRemoveRows(parent, row, row + count - 1);
    for(int i=row+count-1;i>=row;i--)
    {
        //移除该行数据
        modelData.removeAt(i);
    }
    endRemoveRows();
    return true;
}
