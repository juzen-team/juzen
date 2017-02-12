#include "AccountManager.h"
#include "Account.h"
#include "System/Settings.h"

#include <QtCore/QString>

AccountManager::AccountManager() : account(findActiveAccount())
{
}

Account *AccountManager::getActiveAccount()
{
    return &account;
}

QString AccountManager::findActiveAccount()
{
    Settings settings("config");
    return settings.get<QString>("active_account");
}
