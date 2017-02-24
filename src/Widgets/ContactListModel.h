#ifndef JUZEN_CONTACTLISTMODEL_H
#define JUZEN_CONTACTLISTMODEL_H

#include "Roster/Contact.h"
#include <QtCore/QAbstractListModel>
#include <QtCore/QVector>

class ContactListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum class SortType {
        Roster
    };

    ContactListModel(QObject *parent = Q_NULLPTR);
    ~ContactListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::UserRole) const override;

    void add(const Contact::Ptr &contact);
    void change(const QString &jid);

    void setSortType(SortType sortType);

private:
    void sort();

    QVector<Contact::Ptr> m_contacts;
    SortType m_sortType = SortType::Roster;
};

#endif //JUZEN_CONTACTLISTMODEL_H
