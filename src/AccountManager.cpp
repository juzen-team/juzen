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

    account.loadAccount(activeAccount);
}

bool AccountManager::addExistingAccount(const QString &jid, const QString &password, int port)
{
    QString accountDirPath = QString("%1/accounts/%2").arg(AppInfo::getDataDir(), jid);

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
    registrationManager->registerAtServer();
}

void AccountManager::submitRegisterForm(const Jreen::DataForm::Ptr &form)
{
    Jreen::RegistrationData data;
    data.setForm(form);
    registrationManager->send(data);
}

Account *AccountManager::getActiveAccount()
{
    if (account.isNull()) {
        return nullptr;
    }
    return &account;
}

QStringList AccountManager::findAllAccounts() const
{
    QDir accountsDir(AppInfo::getDataDir());
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

    registrationClient = new Jreen::Client();
    registrationManager = new Jreen::RegistrationManager(server, registrationClient);

    connect(registrationManager, &Jreen::RegistrationManager::formReceived, [this](const Jreen::RegistrationData &data)
    {
        if (data.hasForm()) {
            emit registrationFormReceived(data);
        } else {
            Q_ASSERT(!"No form received");
        }
    });

    connect(registrationManager, &Jreen::RegistrationManager::success, [this]()
    {
        emit registrationSuccess();
    });

    connect(registrationManager, &Jreen::RegistrationManager::error, [this](const Jreen::Error::Ptr &error)
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

    connect(registrationManager, &Jreen::RegistrationManager::unsupported, [this]()
    {
        emit registrationUnsupported();
    });
}

void AccountManager::deleteRegistrationObjects()
{
    registrationClient->deleteLater();
    registrationManager->deleteLater();
}
