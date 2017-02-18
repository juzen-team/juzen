#ifndef JUZEN_CONTACT_H
#define JUZEN_CONTACT_H

#include "Roster/ContactResource.h"
#include <jreen/abstractroster.h>
#include <jreen/vcard.h>
#include <QtCore/QObject>
#include <QtGui/QPixmap>

class Roster;
class Contact: public QObject
{
    Q_OBJECT

public:
    using Ptr = QSharedPointer<Contact>;

    Contact(Jreen::RosterItem::Ptr &rosterItem, Roster *roster);
    virtual ~Contact();

    QString getJid() const;

    QString getName() const;
    QString getPresenceText() const;
    QPixmap getPhoto() const;

    void presenceReceived(const Jreen::Presence &presence);
    void vCardFetched(const Jreen::VCard::Ptr &vcard);

signals:
    void contactChanged(const QString &jid);

private:
    void addOrChangeResource(const Jreen::Presence &presence);
    void removeResource(const QString &resource);
    
    void generatePhoto();

    Jreen::RosterItem::Ptr rosterItem;
    Roster *roster;

    QVector<ContactResource::Ptr> resources;

    QString name;
    QPixmap photo;
};
Q_DECLARE_METATYPE(Contact::Ptr)

#endif //JUZEN_CONTACT_H
