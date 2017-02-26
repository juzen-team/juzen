#include "ContactResource.h"
#include <jreen/delayeddelivery.h>

bool operator<(const ContactResource::Ptr &lhs, const ContactResource::Ptr &rhs)
{
    if (lhs->presence().priority() < rhs->presence().priority()) {
        return true;
    }
    if (lhs->presence().priority() == rhs->presence().priority() && lhs->m_lastStatus < rhs->m_lastStatus) {
        return true;
    }
    return false;
}

ContactResource::ContactResource(const Jreen::Presence &presence, QObject *parent) : QObject(parent),
                                                                                     m_presence(presence)
{
    if (presence.containsPayload<Jreen::DelayedDelivery>()) {
        m_lastStatus = presence.payload<Jreen::DelayedDelivery>()->dateTime();
    }
}

ContactResource::~ContactResource()
{
}

QString ContactResource::resource() const
{
    return m_presence.from().resource();
}

QString ContactResource::presenceText() const
{
    return ContactResource::presenceText(presenceType());
}

QString ContactResource::presenceText(Jreen::Presence::Type type)
{
    switch (type) {
        case Jreen::Presence::Unavailable:
            return "Offline";

        case Jreen::Presence::Available:
            return "Online";

        case Jreen::Presence::Chat:
            return "Available for chat";

        case Jreen::Presence::Away:
            return "Away";

        case Jreen::Presence::DND:
            return "Do not disturb";

        case Jreen::Presence::XA:
            return "Not available";

        default:
            return "Unknown";
    }
}

Jreen::Presence::Type ContactResource::presenceType() const
{
    return m_presence.subtype();
}

Jreen::Presence ContactResource::presence() const
{
    return m_presence;
}

void ContactResource::setPresence(const Jreen::Presence &jpresence)
{
    this->m_presence = jpresence;
}

bool ContactResource::isOnline() const
{
    return presenceType() == Jreen::Presence::Available ||
           presenceType() == Jreen::Presence::Chat ||
           presenceType() == Jreen::Presence::Away ||
           presenceType() == Jreen::Presence::DND ||
           presenceType() == Jreen::Presence::XA;
}
