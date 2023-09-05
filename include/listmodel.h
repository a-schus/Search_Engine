#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QString>
#include <QVector>


class BaseListModel : public QAbstractTableModel
{
protected:
    QVector<QString>* dataVec;
    int rows;
    int columns = 1;
public:
    explicit BaseListModel(QObject *parent = nullptr);
    BaseListModel(QVector<QString>* inData, QObject *parent = nullptr);

public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool addRows(QStringList& rowsList);
    virtual bool removeRows(int row, int count, const QModelIndex &parent) override;
    virtual void clear();
};


class ListModel : public BaseListModel
{
public:
    explicit ListModel(QObject *parent = nullptr);
    ListModel(QVector<QString>* inData, QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role) const override;
};



class FileListModel : public BaseListModel
{
public:
    explicit FileListModel(QObject *parent = nullptr);
    FileListModel(QVector<QString>* inData, QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role) const override;
};

#endif // LISTMODEL_H
