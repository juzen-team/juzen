#include "Roster/Roster.h"
#include "Account.h"

#include <iostream>

Roster::Roster(Account *account) : AbstractRoster(account->getClient()),
                                   account(account)
{
    connect(account->getClient(), &Jreen::Client::presenceReceived, this, &Roster::onPresenceReceived);
}

Roster::~Roster()
{
}

void Roster::onItemAdded(QSharedPointer<Jreen::RosterItem> item)
{
    std::cout << "item added: " << item->jid().toStdString() << std::endl;
}

void Roster::onPresenceReceived(const Jreen::Presence &presence)
{
    if (presence.isSubscription()) {
        std::cout << "subscriptionReceived (from: " << presence.from().full().toStdString() << "): " << presence.subtype() << std::endl;
    } else {
        std::cout << "presenceReceived (from: " << presence.from().full().toStdString() << "): " << presence.subtype() << std::endl;
    }
}
