#include "AccountManager.h"
#include "Account.h"
#include "System/Settings.h"

#include <QtCore/QString>

AccountManager::AccountManager() : account(findWorkingAccount())
{
}

QString AccountManager::findWorkingAccount()
{
    Settings settings("config");
    return settings.get<QString>("working_account");
}
