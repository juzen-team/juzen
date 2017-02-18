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

void Roster::onPresenceReceived(const Jreen::Presence &presence)
{
    if (presence.isSubscription()) {
    } else {
        if (!contacts.contains(presence.from().bare())) {
            return;
        }
        contacts[presence.from().bare()]->presenceReceived(presence);
    }
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
