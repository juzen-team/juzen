#include "Account.h"
#include <iostream>

Account::Account(const QString &jid) : roster(this),
                                       settings(QString("accounts/%1/config").arg(jid))
{
    client.setJID(jid);
    client.setPassword(settings.get<QString>("password"));

    auto disco = client.disco();
    disco->setSoftwareVersion("Juzen", "0.1");
	disco->addIdentity(Jreen::Disco::Identity("client", "type", "Juzen"));
    
    connect(&client, &Jreen::Client::connected, this, &Account::onConnected);

    client.connectToServer();
}

Account::~Account()
{
}

Jreen::Client *Account::getClient()
{
    return &client;
}

void Account::onConnected()
{
    client.setPresence(Jreen::Presence::Available);
    roster.load();
}
