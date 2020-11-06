#pragma once
#include <QAbstractListModel>
#include <QList>
template<class T>
class TaoListModelBase : public QAbstractListModel
{
public:
    using Super = QAbstractListModel;
    explicit TaoListModelBase(QObject *parent = nullptr);
    explicit TaoListModelBase(const QList<T> &datas, QObject *parent = nullptr);
    virtual ~TaoListModelBase() override;

public:
    //[begin] query data
    int rowCount(const QModelIndex &parent) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE QVariant data(int row, int role = Qt::DisplayRole) const;
    //[end] query data

    //[begin] reset data
    void resetData(const QList<T> &datas);
    const QList<T> &datas() const { return mDatas; }
    //[end] reset data

    //[begin] add data
    void append(const QList<T> &datas);
    void prepend(T obj);
    void insert(int row, const QList<T> &datas);
    //[end] add data

    //[begin] remove data
    void clear();
    void removeAt(int row);
    //[end] remove data

    //[begin] update data
    void updateData(int row, T data);
    //[end] update data

public:
    virtual void updateCalcInfo() { }

protected:
    QList<T> mDatas;
};

template<class T>
TaoListModelBase<T>::TaoListModelBase(QObject *parent) : Super(parent)
{
}
template<class T>
TaoListModelBase<T>::TaoListModelBase(const QList<T> &datas, QObject *parent)
    : Super(parent), mDatas(datas)
{
}
template<class T>
TaoListModelBase<T>::~TaoListModelBase()
{
    mDatas.clear();
}

template<class T>
int TaoListModelBase<T>::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mDatas.count();
}
template<class T>
QVariant TaoListModelBase<T>::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mDatas.size()) {
        return {};
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        auto data = mDatas.at(index.row());
        return QVariant::fromValue(data);
    }

    return {};
}
template<class T>
QVariant TaoListModelBase<T>::data(int row, int role) const
{
    if (row < 0 || row >= mDatas.size()) {
        return {};
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        auto data = mDatas.at(row);
        return QVariant::fromValue(data);
    }

    return {};
}
template<class T>
void TaoListModelBase<T>::resetData(const QList<T> &datas)
{
    auto oldObjs = mDatas;
    beginResetModel();
    mDatas = datas;
    endResetModel();
    qDeleteAll(oldObjs);
    updateCalcInfo();
}
template<class T>
void TaoListModelBase<T>::append(const QList<T> &datas)
{
    beginInsertRows({}, mDatas.count(), mDatas.count());
    mDatas.append(datas);
    endInsertRows();
    updateCalcInfo();
}
template<class T>
void TaoListModelBase<T>::prepend(T data)
{
    beginInsertRows({}, 0, 0);
    mDatas.prepend(data);
    endInsertRows();
    updateCalcInfo();
}
template<class T>
void TaoListModelBase<T>::insert(int row, const QList<T> &datas)
{
    if (row < 0 || row >= mDatas.size()) {
        return;
    }
    beginInsertRows({}, row, row);
    int srow = row;
    for (const auto &obj : datas) {
        mDatas.insert(srow, obj);
        srow++;
    }
    endInsertRows();
    updateCalcInfo();
}
template<class T>
void TaoListModelBase<T>::clear()
{
    beginRemoveRows({}, 0, mDatas.count());
    qDeleteAll(mDatas);
    mDatas.clear();
    endRemoveRows();
    updateCalcInfo();
}
template<class T>
void TaoListModelBase<T>::removeAt(int row)
{
    if (row < 0 || row >= mDatas.size()) {
        return;
    }
    beginRemoveRows({}, row, row);
    auto obj = mDatas.at(row);
    mDatas.removeAt(row);
    endRemoveRows();
    obj->deleteLater();
    updateCalcInfo();
}
template<class T>
void TaoListModelBase<T>::updateData(int row, T data)
{
    if (row < 0 || row >= mDatas.size()) {
        return;
    }
    auto oldObj = mDatas.at(row);
    mDatas[row] = data;
    emit dataChanged(index(row, 0), index(row, 0));
    oldObj->deleteLater();
    updateCalcInfo();
}
