#include "Contact.h"
#include "Roster.h"
#include <jreen/stanzaextension.h>
#include <QtCore/QSharedPointer>
#include <algorithm>

#include <QtCore/QDebug>

Contact::Contact(Jreen::RosterItem::Ptr &rosterItem, Roster *roster) : QObject(roster),
                                                                       m_roster(roster),
                                                                       m_rosterItem(rosterItem),
                                                                       m_photo(this)
{
    m_photo.generatePhoto();
}

Contact::~Contact()
{
}

Roster *Contact::roster()
{
    return m_roster;
}

QString Contact::jid() const
{
    return m_rosterItem->jid();
}

QString Contact::name() const
{
    if (m_vcard) {
        return m_vcard->formattedName();
    }

    if (m_rosterItem) {
        return m_rosterItem->name();
    }

    return QString();    
}

QPixmap Contact::photo() const
{
    return m_photo.photo();
}

Jreen::Activity::Ptr Contact::activity() const
{
    return m_activity;
}

Jreen::Mood::Ptr Contact::mood() const
{
    return m_mood;
}

Jreen::Tune::Ptr Contact::tune() const
{
    return m_tune;
}

ContactResource::Ptr Contact::mainResource() const
{
    if (m_resources.size() == 0) {
        return QSharedPointer<ContactResource>();
    }
    return m_resources[0];
}

QVector<ContactResource::Ptr> Contact::allResources() const
{
    return m_resources;
}

void Contact::presenceReceived(const Jreen::Presence &presence)
{
    if (presence.subtype() == Jreen::Presence::Unavailable) {
        if (m_resources.size() > 0) {
            removeResource(presence.from().resource());
        }
    } else {
        addOrChangeResource(presence);
    }
    std::sort(m_resources.rbegin(), m_resources.rend());
    emit contactChanged(jid());
}

void Contact::eventReceived(Jreen::Payload::Ptr &event)
{
    auto activity = Jreen::payload_cast<Jreen::Activity>(event);
    if (activity) {
        if (activity->general() == Jreen::Activity::InvalidGeneral || activity->general() == Jreen::Activity::EmptyGeneral) {
            m_activity.clear();
        } else {
            m_activity = activity;
        }
        emit contactChanged(jid());
    }

    auto mood = Jreen::payload_cast<Jreen::Mood>(event);
    if (mood) {
        if (mood->type() == Jreen::Mood::Invalid || mood->type() == Jreen::Mood::Empty) {
            m_mood.clear();
        } else {
            m_mood = mood;
        }
        emit contactChanged(jid());
    }
}

void Contact::updateVCard()
{
    m_roster->fetchVCard(jid());
}

void Contact::vCardFetched(const Jreen::VCard::Ptr &vcard)
{
    m_photo.setPhoto(vcard->photo().data(), vcard->photo().mimeType());
    this->m_vcard = vcard;
    emit contactChanged(jid());
}

void Contact::vCardUpdated(const Jreen::VCardUpdate::Ptr &update)
{
    if (update && update->hasPhotoInfo()) {
        if (!m_photo.setPhoto(update->photoHash())) {
            updateVCard();
        }
    }
}

void Contact::addOrChangeResource(const Jreen::Presence &presence)
{
    auto r = std::find_if(m_resources.begin(), m_resources.end(),
        [&presence](auto r)
        {
            return r->resource() == presence.from().resource();
        }
    );
    if (r != m_resources.end()) {
        (*r)->setPresence(presence);
    } else {
        auto resource = QSharedPointer<ContactResource>::create(presence, this);
        m_resources.push_back(resource);
    }
}

void Contact::removeResource(const QString &resource)
{
    m_resources.erase(std::remove_if(m_resources.begin(), m_resources.end(),
        [&resource](auto r)
        {
            return r->resource() == resource;
        }
    ));
}
