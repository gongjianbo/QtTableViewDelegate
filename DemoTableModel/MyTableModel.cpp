#include "MyTableModel.h"

MyTableModel::MyTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

/*QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

bool MyTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}*/


int MyTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 10;
}

int MyTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 10;
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role==Qt::DisplayRole)
        return index.row();

    return QVariant();
}

/*bool MyTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags MyTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool MyTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
}

bool MyTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
}*/
