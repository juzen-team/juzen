#include "Account.h"
#include "Crypto/QBlowfish.h"
#include <jreen/capabilities.h>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>

Account::Account(const QString &jid) : m_roster(this)
{
    connect(&m_client, &Jreen::Client::connected, this, &Account::onConnect);
    connect(&m_client, &Jreen::Client::disconnected, this, &Account::onDisconnect);
    connect(&m_roster, &Roster::loaded, this, &Account::onRosterLoaded);

    auto caps = m_client.presence().payload<Jreen::Capabilities>();
    caps->setNode("https://juzen.im/");

    auto disco = m_client.disco();
    disco->setSoftwareVersion("Juzen", "0.1");
    disco->addIdentity(Jreen::Disco::Identity("client", "type", "Juzen"));

    loadAccount(jid);
}

Account::~Account()
{
}

void Account::createAccount(const QString &jid, const QString &password, int port)
{
    m_jid = jid;

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
    m_jid = jid;

    m_client.setJID(jid);
    m_client.setPassword(decryptPassword(password));
    if (port != -1) {
        m_client.setPort(port);
    }   
}

bool Account::isNull() const
{
    return m_jid.isEmpty();
}

void Account::connectToServer()
{
    m_client.connectToServer();
}

Jreen::Client *Account::client()
{
    return &m_client;
}

Roster *Account::roster()
{
    return &m_roster;
}

void Account::onConnect()
{
    m_roster.load();
}

void Account::onDisconnect(Jreen::Client::DisconnectReason reason)
{
    switch (reason) {
        case Jreen::Client::User:
            break;

        case Jreen::Client::AuthorizationError:
            QMessageBox::critical(nullptr, "Connection failed", "Wrong username or password.");
            break;

        case Jreen::Client::HostUnknown:
        case Jreen::Client::ItemNotFound:
        case Jreen::Client::SystemShutdown:
        case Jreen::Client::NoSupportedFeature:
        case Jreen::Client::NoAuthorizationSupport:
        case Jreen::Client::NoEncryptionSupport:
        case Jreen::Client::NoCompressionSupport:
        case Jreen::Client::RemoteStreamError:
        case Jreen::Client::RemoteConnectionFailed:
        case Jreen::Client::InternalServerError:
        case Jreen::Client::Conflict:
        case Jreen::Client::Unknown:
            QMessageBox::critical(nullptr, "Connection failed", "Unknown connection error.");
            break;
    }
}

void Account::onRosterLoaded()
{
    m_client.setPresence(Jreen::Presence::Available);
}

QString Account::encryptPassword(const QString &password)
{
    QBlowfish blowfish(m_jid.toUtf8());
    blowfish.setPaddingEnabled(true);
    return blowfish.encrypted(password.toUtf8()).toHex();
}

QString Account::decryptPassword(const QString &crypted)
{
    QBlowfish blowfish(m_jid.toUtf8());
    blowfish.setPaddingEnabled(true);
    return QString::fromUtf8(blowfish.decrypted(QByteArray::fromHex(crypted.toLatin1())));
}
