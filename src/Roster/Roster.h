#ifndef JUZEN_ROSTER_H
#define JUZEN_ROSTER_H

#include <jreen/abstractroster.h>

class Account;
class QString;

class Roster : public Jreen::AbstractRoster
{
    Q_OBJECT

public:
    Roster(Account *account);
    virtual ~Roster();

    QString getPresenceText(const QString &jid) const;

signals:
    void itemAdded(QSharedPointer<Jreen::RosterItem> item);

private:
    void onItemAdded(QSharedPointer<Jreen::RosterItem> item) override;
    void onPresenceReceived(const Jreen::Presence &presence);

    Account *account;

    QMap<QString, QPair<int, QString>> presences;
};

#endif //JUZEN_ROSTER_H
