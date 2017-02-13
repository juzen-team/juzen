#include "ContactListModel.h"

ContactListModel::ContactListModel(QObject *parent) : QAbstractListModel(parent)
{
}

ContactListModel::~ContactListModel()
{
}

int ContactListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return items.size();
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= items.size() || index.row() < 0) {
        return QVariant();
    }

    auto item = items[index.row()];

    if (role == Qt::UserRole) {
        return item.name;
    }
    if (role == Qt::UserRole + 1) {
        return item.status;
    }
    if (role == Qt::UserRole + 2) {
        return item.photo;
    }
    if (role == Qt::UserRole + 3) {
        return item.mimeType;
    }

    return QVariant();
}

void ContactListModel::add(const QString &name, const QString &status, const QByteArray &photo, const QString &mimeType)
{
    Contact item;
    item.name = name;
    item.status = status;
    item.photo = photo;
    item.mimeType = mimeType;

    beginInsertRows(QModelIndex(), items.size(), items.size());
    items.append(item);
    endInsertRows();
}
