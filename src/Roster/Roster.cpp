#include "Roster/Roster.h"
#include "Account.h"
#include <QtCore/QSharedPointer>

#include <QtCore/qdebug.h>

Roster::Roster(Account *account) : AbstractRoster(account->getClient()),
                                   account(account),
                                   vcardmgr(account->getClient())
{
    connect(account->getClient(), &Jreen::Client::presenceReceived, this, &Roster::onPresenceReceived);

    connect(&vcardmgr, &Jreen::VCardManager::vCardFetched, this, &Roster::onVCardFetched);
    connect(&vcardmgr, &Jreen::VCardManager::vCardUpdateDetected, this, &Roster::onVCardUpdateDetected);
}

Roster::~Roster()
{
}

void Roster::onItemAdded(QSharedPointer<Jreen::RosterItem> item)
{
    auto contact = addItem(item);
    emit contactAdded(contact);
}

void Roster::onItemUpdated(Jreen::RosterItem::Ptr item)
{
    qDebug() << "item updated: " << item->jid();
}

void Roster::onItemRemoved(const QString &jid)
{
    qDebug() << "item removed: " << jid;
}

void Roster::onLoaded(const QList<Jreen::RosterItem::Ptr> &items)
{
    for (auto item : items) {
        addItem(item);
    }
    emit loaded(contacts);
}

void Roster::onPresenceReceived(const Jreen::Presence &presence)
{
    /*if (presence.isSubscription()) {
        std::cout << "subscriptionReceived (from: " << presence.from().full().toStdString() << "): " << presence.subtype() << std::endl;
    } else {
        switch (presence.subtype()) {
            case Jreen::Presence::Unavailable:
                textPresence = "Offline";
                break;
            case Jreen::Presence::Available:
                textPresence = "Online";
                break;

            case Jreen::Presence::Chat:
                textPresence = "Available for chat";
                break;

            case Jreen::Presence::Away:
                textPresence = "Away";
                break;

            case Jreen::Presence::DND:
                textPresence = "Do not disturb";
                break;

            case Jreen::Presence::XA:
                textPresence = "Not available";
                break;

            default:
                textPresence = "Unknown";
        }
    }*/
}

void Roster::onVCardFetched(const Jreen::VCard::Ptr &vcard, const Jreen::JID &jid)
{
    if (!contacts.contains(jid.bare())) {
        return;
    }

    contacts[jid.bare()]->vCardFetched(vcard);
}

void Roster::onVCardUpdateDetected(const Jreen::JID &jid, const Jreen::VCardUpdate::Ptr &update)
{

}

Contact::Ptr Roster::addItem(Jreen::RosterItem::Ptr item)
{
    QString jid = item->jid();
    if (contacts.contains(jid)) {
        return contacts[jid];
    }

    auto contact = QSharedPointer<Contact>::create(item, this);
    contacts[jid] = contact;
    vcardmgr.fetch(jid);
    return contact;
}
