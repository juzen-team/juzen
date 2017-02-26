#ifndef JUZEN_CONTACT_H
#define JUZEN_CONTACT_H

#include "Roster/ContactResource.h"
#include "Roster/ContactPhoto.h"
#include <jreen/abstractroster.h>
#include <jreen/activity.h>
#include <jreen/mood.h>
#include <jreen/stanzaextension.h>
#include <jreen/tune.h>
#include <jreen/vcard.h>
#include <jreen/vcardupdate.h>
#include <QtCore/QObject>
#include <QtGui/QPixmap>

class Roster;
class Contact : public QObject
{
    Q_OBJECT

public:
    using Ptr = QSharedPointer<Contact>;

    Contact(Jreen::RosterItem::Ptr &rosterItem, Roster *roster);
    virtual ~Contact();
    
    Roster *roster();

    QString jid() const;
    QString name() const;
    QPixmap photo() const;

    Jreen::Activity::Ptr activity() const;
    Jreen::Mood::Ptr mood() const;
    Jreen::Tune::Ptr tune() const;

    ContactResource::Ptr mainResource() const;
    QVector<ContactResource::Ptr> allResources() const;

    void presenceReceived(const Jreen::Presence &presence);
    void eventReceived(Jreen::Payload::Ptr &event);

    void updateVCard();
    void vCardFetched(const Jreen::VCard::Ptr &vcard);
    void vCardUpdated(const Jreen::VCardUpdate::Ptr &update);

signals:
    void contactChanged(const QString &jid);

private:
    void addOrChangeResource(const Jreen::Presence &presence);
    void removeResource(const QString &resource);

    ContactPhoto m_photo;

    Roster *m_roster;
    Jreen::RosterItem::Ptr m_rosterItem;
    QVector<ContactResource::Ptr> m_resources;
    Jreen::VCard::Ptr m_vcard;

    Jreen::Activity::Ptr m_activity;
    Jreen::Mood::Ptr m_mood;
    Jreen::Tune::Ptr m_tune;
};
Q_DECLARE_METATYPE(Contact::Ptr)

#endif //JUZEN_CONTACT_H
