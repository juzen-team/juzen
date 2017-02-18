#ifndef JUZEN_CONTACTRESOURCE_H
#define JUZEN_CONTACTRESOURCE_H

#include <jreen/presence.h>
#include <QtCore/QDateTime>
#include <QtCore/QObject>

class ContactResource : public QObject
{
    Q_OBJECT

public:
    using Ptr = QSharedPointer<ContactResource>;
    friend bool operator<(const ContactResource::Ptr &lhs, const ContactResource::Ptr &rhs);

    ContactResource(const Jreen::Presence &jpresence = Jreen::Presence(Jreen::Presence::Invalid, QString()), QObject *parent = Q_NULLPTR);
    virtual ~ContactResource();

    QString resource() const;
    QString presenceText() const;
    Jreen::Presence::Type presenceType() const;

    Jreen::Presence presence() const;
    void setPresence(const Jreen::Presence &jpresence);

private:
    Jreen::Presence jpresence;
    QDateTime lastStatus = QDateTime::currentDateTime();
};

#endif //JUZEN_CONTACTRESOURCE_H