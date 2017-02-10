#ifndef JUZEN_ROSTER_H
#define JUZEN_ROSTER_H

#include <jreen/abstractroster.h>

class Account;
class Roster : public Jreen::AbstractRoster
{
    Q_OBJECT

public:
    Roster(Account *account);
    virtual ~Roster();

private:
    void onItemAdded(QSharedPointer<Jreen::RosterItem> item) override;
    void onPresenceReceived(const Jreen::Presence &presence);

    Account *account;
};

#endif //JUZEN_ROSTER_H
