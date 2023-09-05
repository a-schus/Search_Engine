#include "listmodel.h"

#include <QColor>
#include <QFile>


BaseListModel::BaseListModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

BaseListModel::BaseListModel(QVector<QString>* inData, QObject *parent) :
    dataVec(inData), rows(dataVec->size()), QAbstractTableModel{parent}
{

}


int BaseListModel::rowCount(const QModelIndex &parent) const
{
    return rows;
}

int BaseListModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

bool BaseListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row > rows || row < 0)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    rows += count;
    endInsertRows();
    return true;
}

bool BaseListModel::addRows(QStringList &rowsList)
{
    int firstRow = rows;
    insertRows(firstRow, rowsList.size(), QModelIndex());
    for (auto& row :rowsList) {
        QModelIndex index = this->index(firstRow, 0);
        setData(index, row, Qt::EditRole);
        ++firstRow;
    }

    return true;
}

bool BaseListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    dataVec->push_back(value.toString());

    emit dataChanged(index, index);

    return true;
}

bool BaseListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row + count > rows || row < 0)
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    rows -= count;
    endRemoveRows();
    dataVec->erase(dataVec->begin() + row, dataVec->begin() + row + count);

    return true;
}

void BaseListModel::clear()
{
    removeRows(0, rows, QModelIndex());
}




ListModel::ListModel(QObject *parent)
    : BaseListModel{parent}
{

}

ListModel::ListModel(QVector<QString>* inData, QObject *parent) :
    BaseListModel(inData, parent)
{

}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return QVariant(dataVec->at(index.row()));
    }

    return QVariant();
}



FileListModel::FileListModel(QObject *parent)
    : BaseListModel{parent}
{

}

FileListModel::FileListModel(QVector<QString>* inData, QObject *parent) :
    BaseListModel(inData, parent)
{

}

QVariant FileListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return QVariant(dataVec->at(index.row()));
    }

    if (role == Qt::ForegroundRole) {
        if (!QFile::exists(dataVec->at(index.row())))
            return QVariant(QColor("red"));
    }

    return QVariant();
}


