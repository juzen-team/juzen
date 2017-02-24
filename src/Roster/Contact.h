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

    Contact(Jreen::RosterItem::Ptr &rosterItem, QObject *parent = Q_NULLPTR);
    virtual ~Contact();

    QString jid() const;
    QString name() const;
    QPixmap photo() const;

    ContactResource::Ptr mainResource() const;
    QVector<ContactResource::Ptr> allResources() const;

    void presenceReceived(const Jreen::Presence &presence);
    void vCardFetched(const Jreen::VCard::Ptr &vcard);

signals:
    void contactChanged(const QString &jid);

private:
    void addOrChangeResource(const Jreen::Presence &presence);
    void removeResource(const QString &resource);

    QPixmap generateNoPhotoFiller() const;
    QPixmap contactPhoto;

    Jreen::RosterItem::Ptr m_rosterItem;
    QVector<ContactResource::Ptr> m_resources;
    Jreen::VCard::Ptr m_vcard;
};
Q_DECLARE_METATYPE(Contact::Ptr)

#endif //JUZEN_CONTACT_H
