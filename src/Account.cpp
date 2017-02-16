#include "Account.h"
#include <QtWidgets/QInputDialog>

Account::Account(const QString &jid) : roster(this)
{
    auto disco = client.disco();
    disco->setSoftwareVersion("Juzen", "0.1");
    disco->addIdentity(Jreen::Disco::Identity("client", "type", "Juzen"));
    
    connect(&client, &Jreen::Client::connected, this, &Account::onConnected);

    loadAccount(jid);
}

Account::~Account()
{
}

void Account::loadAccount(const QString &jid)
{
    if (jid.isEmpty()) {
        return;
    }

    Settings settings(QString("accounts/%1/config").arg(jid));
    QString password = settings.get<QString>("password");
    int port = settings.get("port", -1);

    if (password.isEmpty()) {
        bool ok;
        password = QInputDialog::getText(nullptr, "Enter password", QString("Please, enter password for account %1:").arg(jid), QLineEdit::PasswordEchoOnEdit, QString(), &ok);
        if (ok && !password.isEmpty()) {
            settings.set<QString>("password", password);
        } else {
            return;
        }
    }
    this->jid = jid;

    client.setJID(jid);
    client.setPassword(password);
    if (port != -1) {
        client.setPort(port);
    }   
}

bool Account::isNull() const
{
    return jid.isEmpty();
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
