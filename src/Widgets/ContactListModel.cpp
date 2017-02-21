#include "ContactListModel.h"
#include <jreen/presence.h>
#include <QtCore/QMap>

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
    if (role == Qt::ToolTipRole) {
        auto contact = contacts[index.row()];

        auto tooltip = QString("<b>%1</b> (%2)<br />").arg(contact->name(), contact->jid());
        for (auto &resource : contact->allResources()) {
            tooltip += QString("<b>%1</b> (%2): %3<br />").arg(resource->resource(), QString::number(resource->presence().priority()), resource->presenceText());
        }
        return tooltip;
    }
    if (role == Qt::UserRole) {
        return QVariant::fromValue(contacts[index.row()]);
    }
    return QVariant();
}

void ContactListModel::add(const Contact::Ptr &contact)
{
    beginInsertRows(QModelIndex(), contacts.size(), contacts.size());
    contacts.push_back(contact);
    endInsertRows();
    sort();

    connect(contact.data(), &Contact::contactChanged,
        [this](const QString &jid)
        {
            change(jid);
        }
    );
}

void ContactListModel::change(const QString &jid)
{
    auto it = std::find_if(contacts.cbegin(), contacts.cend(),
        [&jid](const Contact::Ptr contact)
        {
            return contact->jid() == jid;
        }
    );
    if (it == contacts.cend()) {
        return;
    }

    QModelIndex mindex = index(std::distance(contacts.cbegin(), it));
    emit dataChanged(mindex, mindex);

    sort();
}

void ContactListModel::setSortType(ContactListModel::SortType sortType)
{
    this->sortType = sortType;
}

void ContactListModel::sort()
{
    auto sortTypeRoster =
        [](Contact::Ptr contact1, Contact::Ptr contact2)
        {
            auto textComparator =
                [](Contact::Ptr contact1, Contact::Ptr contact2)
                {
                    auto compareResult = contact1->name().compare(contact2->name(), Qt::CaseInsensitive);
                    if (compareResult != 0) {
                        return compareResult < 0;
                    }

                    return contact1->jid().compare(contact2->jid(), Qt::CaseInsensitive) < 0;
                };

            auto contact1PresenceType = (contact1->mainResource() ? contact1->mainResource()->presenceType() : Jreen::Presence::Unavailable);
            auto contact2PresenceType = (contact2->mainResource() ? contact2->mainResource()->presenceType() : Jreen::Presence::Unavailable);

            if (contact1PresenceType == contact2PresenceType) {
                return textComparator(contact1, contact2);
            }

            static QMap<Jreen::Presence::Type, int> presencePositions = {
                { Jreen::Presence::Unavailable, 5 },
                { Jreen::Presence::Available, 1 },
                { Jreen::Presence::Chat, 0 },
                { Jreen::Presence::Away, 2 },
                { Jreen::Presence::DND, 3 },
                { Jreen::Presence::XA, 4 }
            };

            if (!presencePositions.contains(contact1PresenceType) && !presencePositions.contains(contact2PresenceType)) {
                return textComparator(contact1, contact2);
            } else if (!presencePositions.contains(contact1PresenceType)) {
                return false;
            } else if (!presencePositions.contains(contact2PresenceType)) {
                return true;
            }

            return presencePositions[contact1PresenceType] < presencePositions[contact2PresenceType];
        };

    emit layoutAboutToBeChanged();
    std::sort(contacts.begin(), contacts.end(), sortTypeRoster);
    emit layoutChanged();
}
