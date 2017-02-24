#include "AccountManager.h"
#include "System/AppInfo.h"
#include "Widgets/AccountAddWizard.h"

#include <QtCore/QDir>

AccountManager::AccountManager()
{
    QString activeAccount = findActiveAccount();
    if (activeAccount.isEmpty()) {
        AccountAddWizard wizard(this);
        int result = wizard.exec();
        if (!result) {
            return;
        }
    }

    activeAccount = findActiveAccount();
    if (activeAccount.isEmpty()) {
        return;
    }

    m_account.loadAccount(activeAccount);
}

bool AccountManager::addExistingAccount(const QString &jid, const QString &password, int port)
{
    QString accountDirPath = QString("%1/accounts/%2").arg(AppInfo::dataDir(), jid);

    QDir accountDir;
    if (!accountDir.mkpath(accountDirPath)) {
        return false;
    }

    Account newAccount;
    newAccount.createAccount(jid, password, port);

    Settings settings("config");
    settings.set("active_account", jid);

    return true;
}

void AccountManager::getRegisterForm(const QString &server)
{
    createRegistrationObjects(server);
    m_registrationManager->registerAtServer();
}

void AccountManager::submitRegisterForm(const Jreen::DataForm::Ptr &form)
{
    Jreen::RegistrationData data;
    data.setForm(form);
    m_registrationManager->send(data);
}

Account *AccountManager::activeAccount()
{
    if (m_account.isNull()) {
        return nullptr;
    }
    return &m_account;
}

QStringList AccountManager::findAllAccounts() const
{
    QDir accountsDir(AppInfo::dataDir());
    if (!accountsDir.exists() || !accountsDir.cd("accounts")) {
        return QStringList();
    }

    return accountsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
}

QString AccountManager::findActiveAccount() const
{
    QStringList accounts = findAllAccounts();
    
    Settings settings("config");
    QString activeAccount = settings.get<QString>("active_account");
    if (accounts.contains(activeAccount)) {
        return activeAccount;
    }

    if (!accounts.empty()) {
        return accounts[0];
    }

    return QString();
}

void AccountManager::createRegistrationObjects(const QString &server)
{
    deleteRegistrationObjects();

    m_registrationClient = new Jreen::Client();
    m_registrationManager = new Jreen::RegistrationManager(server, m_registrationClient);

    connect(m_registrationManager, &Jreen::RegistrationManager::formReceived, [this](const Jreen::RegistrationData &data)
    {
        if (data.hasForm()) {
            emit registrationFormReceived(data);
        } else {
            Q_ASSERT(!"No form received");
        }
    });

    connect(m_registrationManager, &Jreen::RegistrationManager::success, [this]()
    {
        emit registrationSuccess();
    });

    connect(m_registrationManager, &Jreen::RegistrationManager::error, [this](const Jreen::Error::Ptr &error)
    {
        QString errorStr = (error ? error->text() : "Unknown error.");
        if (errorStr.isEmpty()) {
            switch (error->condition()) {
                case Jreen::Error::Conflict:
                    errorStr = "The desired name is already in use by another user.";
                    break;

                default:
                    errorStr = "Unknown error.";
            }
        }
        emit registrationError(errorStr);
    });

    connect(m_registrationManager, &Jreen::RegistrationManager::unsupported, [this]()
    {
        emit registrationUnsupported();
    });
}

void AccountManager::deleteRegistrationObjects()
{
    m_registrationClient->deleteLater();
    m_registrationManager->deleteLater();
}
