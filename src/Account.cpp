#include "Account.h"
#include <iostream>

Account::Account(const QString &jid) : roster(this)
{

}

Account::~Account();

Jreen::Client *Account::getClient() const
{
    return &client;
}

void Account::onConnected()
{
    client.setPresence(Jreen::Presence::Available);
    roster.load();
}
