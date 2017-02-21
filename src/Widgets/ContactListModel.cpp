#include "ContactListModel.h"
#include <QtCore/qdebug.h>

ContactListModel::ContactListModel(QObject *parent) : QAbstractListModel(parent)
{
}

ContactListModel::~ContactListModel()
{
}

int ContactListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return contacts.size();
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::UserRole) {
        return QVariant::fromValue(contacts.values()[index.row()]);
    }
    return QVariant();
}

void ContactListModel::add(const Contact::Ptr &contact)
{
    beginInsertRows(QModelIndex(), contacts.size(), contacts.size());
    contacts[contact->jid()] = contact;
    endInsertRows();
    connect(contact.data(), &Contact::contactChanged,
        [this](const QString &jid)
        {
            change(jid);
        }
    );
}

void ContactListModel::change(const QString &jid)
{
    QModelIndex mindex = index(std::distance(contacts.begin(), contacts.lowerBound(jid)));
    emit dataChanged(mindex, mindex);
}
