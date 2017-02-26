#include "Roster/Roster.h"
#include "Account.h"
#include <jreen/mood.h>
#include <jreen/stanzaextension.h>
#include <QtCore/QSharedPointer>

#include <QtCore/qdebug.h>

Roster::Roster(Account *m_account) : AbstractRoster(m_account->client()),
                                     m_account(m_account),
                                     m_vcardManager(m_account->client())
{
    connect(m_account->client(), &Jreen::Client::presenceReceived, this, &Roster::onPresenceReceived);
    connect(m_account->client(), &Jreen::Client::iqReceived, this, &Roster::onIqReceived);
    connect(m_account->client(), &Jreen::Client::messageReceived, this, &Roster::onMessageReceived);
    connect(m_account->pubSubManager(), &Jreen::PubSub::Manager::eventReceived, this, &Roster::onEventReceived);
    connect(&m_vcardManager, &Jreen::VCardManager::vCardFetched, this, &Roster::onVCardFetched);
    connect(&m_vcardManager, &Jreen::VCardManager::vCardUpdateDetected, this, &Roster::onVCardUpdateDetected);
}

Roster::~Roster()
{
}

Account *Roster::account()
{
    return m_account;
}

Jreen::VCardReply *Roster::fetchVCard(const QString &jid)
{
    return m_vcardManager.fetch(jid);
}

void Roster::onItemAdded(Jreen::RosterItem::Ptr item)
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

void Roster::onIqReceived(const Jreen::IQ &iq)
{
}

void Roster::onMessageReceived(const Jreen::Message &message)
{
    qDebug() << "onMessageReceived " << message.from().full() << " " << message.body();
}

void Roster::onEventReceived(const Jreen::PubSub::Event::Ptr &event, const Jreen::JID &from)
{
    if (!m_contacts.contains(from.bare())) {
        return;
    }

    for (auto item : event->items()) {
        m_contacts[from.bare()]->eventReceived(item);
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
    if (!m_contacts.contains(jid.bare())) {
        return;
    }
    m_contacts[jid.bare()]->vCardUpdated(update);
}

Contact::Ptr Roster::addItem(Jreen::RosterItem::Ptr item)
{
    QString jid = item->jid();
    if (m_contacts.contains(jid)) {
        return m_contacts[jid];
    }

    auto contact = QSharedPointer<Contact>::create(item, this);
    m_contacts[jid] = contact;
    return contact;
}
