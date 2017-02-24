#include "Roster/Roster.h"
#include "Account.h"
#include <QtCore/QSharedPointer>

#include <QtCore/qdebug.h>

Roster::Roster(Account *m_account) : AbstractRoster(m_account->client()),
                                     m_account(m_account),
                                     m_vcardManager(m_account->client())
{
    connect(m_account->client(), &Jreen::Client::presenceReceived, this, &Roster::onPresenceReceived);
    connect(&m_vcardManager, &Jreen::VCardManager::vCardFetched, this, &Roster::onVCardFetched);
    connect(&m_vcardManager, &Jreen::VCardManager::vCardUpdateDetected, this, &Roster::onVCardUpdateDetected);
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
        if (!m_contacts.contains(presence.from().bare())) {
            return;
        }
        m_contacts[presence.from().bare()]->presenceReceived(presence);
    }
}

void Roster::onVCardFetched(const Jreen::VCard::Ptr &vcard, const Jreen::JID &jid)
{
    if (!m_contacts.contains(jid.bare())) {
        return;
    }
    m_contacts[jid.bare()]->vCardFetched(vcard);
}

void Roster::onVCardUpdateDetected(const Jreen::JID &jid, const Jreen::VCardUpdate::Ptr &update)
{

}

Contact::Ptr Roster::addItem(Jreen::RosterItem::Ptr item)
{
    QString jid = item->jid();
    if (m_contacts.contains(jid)) {
        return m_contacts[jid];
    }

    auto contact = QSharedPointer<Contact>::create(item, this);
    m_contacts[jid] = contact;
    m_vcardManager.fetch(jid);
    return contact;
}
