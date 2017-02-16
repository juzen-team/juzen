#ifndef JUZEN_ACCOUNT_H
#define JUZEN_ACCOUNT_H

#include "Roster/Roster.h"
#include "System/Settings.h"

#include <jreen/client.h>
#include <QtCore/QObject>

class QString;
class Account: public QObject
{
    Q_OBJECT

public:
    Account(const QString &jid = QString());
    virtual ~Account();

    void loadAccount(const QString &jid);
    bool isNull() const;

    void connectToServer();

    Jreen::Client *getClient();
    Roster *getRoster();

private:
    void onConnected();

    Jreen::Client client;
    Roster roster;

    QString jid;
};

#endif //JUZEN_ACCOUNT_H
