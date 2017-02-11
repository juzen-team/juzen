#ifndef JUZEN_ACCOUNT_H
#define JUZEN_ACCOUNT_H

#include "Roster/Roster.h"
#include <jreen/client.h>
#include <QtCore/QObject>
#include <QtCore/QString>

class Account: public QObject
{
    Q_OBJECT

public:
    Account(const QString &jid);
    virtual ~Account();

    Jreen::Client *getClient();

private:
    void onConnected();

    Jreen::Client client;
    Roster roster;

    QString jid;
};

#endif //JUZEN_ACCOUNT_H
