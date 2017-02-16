#include "Account.h"
#include <QtWidgets/QInputDialog>

Account::Account(const QString &jid) : roster(this)
{
    auto disco = client.disco();
    disco->setSoftwareVersion("Juzen", "0.1");
    disco->addIdentity(Jreen::Disco::Identity("client", "type", "Juzen"));
    
    connect(&client, &Jreen::Client::connected, this, &Account::onConnected);

    if (!jid.isEmpty()) {
        setJid(jid);
    }
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

    QString password = settings.get<QString>("password");
    if (password.isEmpty()) {
        bool ok;
        password = QInputDialog::getText(nullptr, "Enter password", QString("Please, enter password for account %1:").arg(jid), QLineEdit::PasswordEchoOnEdit, QString(), &ok);
        if (ok && !password.isEmpty()) {
            settings.set<QString>("password", password);
        } else {
            this->jid = QString();
            return;
        }
    }

    client.setJID(jid);
    client.setPassword(password);
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
