#ifndef JUZEN_CONTACTLISTMODEL_H
#define JUZEN_CONTACTLISTMODEL_H

#include "Roster/Contact.h"
#include <QtCore/QAbstractListModel>
#include <QtCore/QMap>

class ContactListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ContactListModel(QObject *parent = Q_NULLPTR);
    ~ContactListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::UserRole) const override;

    void add(const Contact::Ptr &contact);
    void change(const QString &jid);

private:
    QMap<QString, Contact::Ptr> contacts;
};

#endif //JUZEN_CONTACTLISTMODEL_H
