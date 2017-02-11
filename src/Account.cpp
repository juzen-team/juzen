#include "Account.h"
#include <iostream>

Account::Account(const QString &jid) : roster(this)
{
    connect(&client, &Jreen::Client::connected, this, &Account::onConnected);
    
    client.setJID(Jreen::JID(jid));
    client.setPassword("password");
    
    auto disco = client.disco();
    disco->setSoftwareVersion("Juzen", "0.1");
	disco->addIdentity(Jreen::Disco::Identity("client", "type", "Juzen"));
    
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
