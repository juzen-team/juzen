#ifndef JUZEN_ROSTER_H
#define JUZEN_ROSTER_H

#include "Roster/Contact.h"
#include <jreen/abstractroster.h>
#include <jreen/message.h>
#include <jreen/pubsubmanager.h>
#include <jreen/vcardmanager.h>

class Account;
class Roster : public Jreen::AbstractRoster
{
    Q_OBJECT

public:
    using ContactsMap = QMap<QString, Contact::Ptr>;

    Roster(Account *m_account);
    virtual ~Roster();

signals:
    void contactAdded(Contact::Ptr contact);

private:
    void onItemAdded(Jreen::RosterItem::Ptr item) override;
    void onItemUpdated(Jreen::RosterItem::Ptr item) override;
    void onItemRemoved(const QString &jid) override;
    void onPresenceReceived(const Jreen::Presence &presence);
    void onIqReceived(const Jreen::IQ &iq);
    void onMessageReceived(const Jreen::Message &message);
    void onEventReceived(const Jreen::PubSub::Event::Ptr &event, const Jreen::JID &from);

    void onVCardFetched(const Jreen::VCard::Ptr &vcard, const Jreen::JID &jid);
    void onVCardUpdateDetected(const Jreen::JID &jid, const Jreen::VCardUpdate::Ptr &update);

    Contact::Ptr addItem(Jreen::RosterItem::Ptr item);

    Account *m_account;
    Jreen::VCardManager m_vcardManager;
    ContactsMap m_contacts;
};

#endif //JUZEN_ROSTER_H
