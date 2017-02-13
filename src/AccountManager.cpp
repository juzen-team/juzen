#include "AccountManager.h"
#include "Account.h"
#include "System/AppInfo.h"
#include "System/Settings.h"
#include "Widgets/AccountAddWizard.h"

#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtWidgets/QApplication>

AccountManager::AccountManager()
{
	QString activeAccount = findActiveAccount();
	if (activeAccount.isEmpty()) {
		AccountAddWizard wizard(this);
		int result = wizard.exec();
		if (result) {
			activeAccount = findActiveAccount();
		}
	}

	if (activeAccount.isEmpty()) {
		return;
	}

	account.setJid(activeAccount);
	account.connectToServer();
}

bool AccountManager::addExistingAccount(const QString &jid, const QString &password, int port)
{
	QString accountDirPath = QString("%1/accounts/%2").arg(AppInfo::getDataDir(), jid);

	QDir accountDir;
	if (!accountDir.mkpath(accountDirPath)) {
		return false;
	}

	Settings accountSettings(QString("accounts/%1/config").arg(jid));
	accountSettings.set("password", password);
	accountSettings.set("port", port);

	Settings settings("config");
	settings.set("active_account", jid);

	return true;
}

bool AccountManager::registerAccount()
{
	return false;
}

Account *AccountManager::getActiveAccount()
{
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
