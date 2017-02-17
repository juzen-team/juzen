#ifndef JUZEN_CONTACT_H
#define JUZEN_CONTACT_H

#include <jreen/abstractroster.h>
#include <jreen/vcard.h>
#include <QtCore/QObject>
#include <QtGui/QPixmap>

class Contact: public QObject
{
    Q_OBJECT

public:
    using Ptr = QSharedPointer<Contact>;

    Contact(Jreen::RosterItem::Ptr &rosterItem, QObject *parent = Q_NULLPTR);
    virtual ~Contact();

    QString getJid() const;

    QString getName() const;
    QString getPresenceText() const;
    QPixmap getPhoto() const;

    void vCardFetched(const Jreen::VCard::Ptr &vcard);

signals:
    void contactChanged(const QString &jid);

private:
    void generatePhoto();

    Jreen::RosterItem::Ptr rosterItem;

    QString name;
    QString presenceText;
    QPixmap photo;
};
Q_DECLARE_METATYPE(Contact::Ptr)

#endif //JUZEN_CONTACT_H
