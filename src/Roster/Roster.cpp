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

QString Roster::getPresenceText(const QString &jid) const
{
    if (presences.contains(jid)) {
        return presences[jid].second;
    }

    return "Unknown";
}

void Roster::onItemAdded(QSharedPointer<Jreen::RosterItem> item)
{
    std::cout << "item added: " << item->jid().toStdString() << std::endl;
    emit itemAdded(item);
}

void Roster::onPresenceReceived(const Jreen::Presence &presence)
{
    if (presence.isSubscription()) {
        std::cout << "subscriptionReceived (from: " << presence.from().full().toStdString() << "): " << presence.subtype() << std::endl;
    } else {
        QString jid = presence.from().bare();
        QString textPresence;
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

        if (presences.contains(jid)) {
            if (presences[jid].first < presence.priority()) {
                presences[jid] = QPair<int, QString>(presence.priority(), textPresence);
            }
        } else {
            presences[jid] = QPair<int, QString>(presence.priority(), textPresence);
        }

        std::cout << "presenceReceived (from: " << presence.from().full().toStdString() << "): " << presence.subtype() << std::endl;
    }
}
