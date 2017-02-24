#ifndef JUZEN_ACCOUNT_H
#define JUZEN_ACCOUNT_H

#include "Roster/Roster.h"
#include "System/Settings.h"
#include <jreen/client.h>
#include <QtCore/QObject>

class Account: public QObject
{
    Q_OBJECT

public:
    Account(const QString &jid = QString());
    virtual ~Account();

    void createAccount(const QString &jid, const QString &password, int port);
    void loadAccount(const QString &jid);

    bool isNull() const;

    void connectToServer();

    Jreen::Client *client();
    Roster *roster();

private:
    void onConnect();
    void onDisconnect(Jreen::Client::DisconnectReason reason);
    void onRosterLoaded();

    QString encryptPassword(const QString &password);
    QString decryptPassword(const QString &crypted);

    QString m_jid;

    Jreen::Client m_client;
    Roster m_roster;
};

#endif //JUZEN_ACCOUNT_H
