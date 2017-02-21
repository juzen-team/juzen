#include "Account.h"
#include "Crypto/QBlowfish.h"
#include <QtWidgets/QInputDialog>

Account::Account(const QString &jid) : roster(this)
{
    auto disco = client.disco();
    disco->setSoftwareVersion("Juzen", "0.1");
    disco->addIdentity(Jreen::Disco::Identity("client", "type", "Juzen"));
    
    connect(&client, &Jreen::Client::connected,
        [this]()
        {
            roster.load();
        }
    );
    connect(&roster, &Roster::loaded,
        [this]()
        {
            client.setPresence(Jreen::Presence::Available);
        }
    );

    loadAccount(jid);
}

Account::~Account()
{
}

void Account::createAccount(const QString &jid, const QString &password, int port)
{
    this->jid = jid;

    Settings settings(QString("accounts/%1/config").arg(jid));
    settings.set("password", encryptPassword(password));
    settings.set("port", port);
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
            settings.set<QString>("password", encryptPassword(password));
        } else {
            return;
        }
    }
    this->jid = jid;

    client.setJID(jid);
    client.setPassword(decryptPassword(password));
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

QString Account::encryptPassword(const QString &password)
{
    QBlowfish blowfish(jid.toUtf8());
    blowfish.setPaddingEnabled(true);
    return blowfish.encrypted(password.toUtf8()).toHex();
}

QString Account::decryptPassword(const QString &crypted)
{
    QBlowfish blowfish(jid.toUtf8());
    blowfish.setPaddingEnabled(true);
    return QString::fromUtf8(blowfish.decrypted(QByteArray::fromHex(crypted.toLatin1())));
}
