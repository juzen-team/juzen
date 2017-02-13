#include "Account.h"

Account::Account(const QString &jid) : roster(this)
{
    auto disco = client.disco();
    disco->setSoftwareVersion("Juzen", "0.1");
	disco->addIdentity(Jreen::Disco::Identity("client", "type", "Juzen"));
    
    connect(&client, &Jreen::Client::connected, this, &Account::onConnected);

	setJid(jid);
}

Account::~Account()
{
}

bool Account::isNull() const
{
	return jid.isEmpty();
}

void Account::setJid(const QString &jid)
{
	this->jid = jid;

	Settings settings(QString("accounts/%1/config").arg(jid));

	client.setJID(jid);
	client.setPassword(settings.get<QString>("password"));
}

void Account::connectToServer()
{
	client.connectToServer();
}

Jreen::Client *Account::getClient()
{
    return &client;
}

Roster *Account::getRoster()
{
    return &roster;
}

void Account::onConnected()
{
    client.setPresence(Jreen::Presence::Available);
    roster.load();
}
