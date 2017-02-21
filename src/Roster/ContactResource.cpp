#include "ContactResource.h"
#include <jreen/delayeddelivery.h>

bool operator<(const ContactResource::Ptr &lhs, const ContactResource::Ptr &rhs)
{
    if (lhs->presence().priority() < rhs->presence().priority()) {
        return true;
    }
    if (lhs->presence().priority() == rhs->presence().priority() && lhs->lastStatus < rhs->lastStatus) {
        return true;
    }
    return false;
}

ContactResource::ContactResource(const Jreen::Presence &jpresence, QObject *parent) : QObject(parent),
                                                                                      jpresence(jpresence)
{
    if (jpresence.containsPayload<Jreen::DelayedDelivery>()) {
        lastStatus = jpresence.payload<Jreen::DelayedDelivery>()->dateTime();
    }
}

ContactResource::~ContactResource()
{
}

QString ContactResource::resource() const
{
    return jpresence.from().resource();
}

QString ContactResource::presenceText() const
{
    switch (jpresence.subtype()) {
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
    return jpresence.subtype();
}

Jreen::Presence ContactResource::presence() const
{
    return jpresence;
}

void ContactResource::setPresence(const Jreen::Presence &jpresence)
{
    this->jpresence = jpresence;
}
