#ifndef JUZEN_ACCOUNTMANAGER_H
#define JUZEN_ACCOUNTMANAGER_H

#include "Account.h"
#include <QtCore/QObject>

class QStringList;
class AccountManager: public QObject
{
    Q_OBJECT

public:
    AccountManager();

	bool addExistingAccount(const QString &jid, const QString &password, int port);
	bool registerAccount();

    Account *getActiveAccount();

private:
	QStringList findAllAccounts() const;
    QString findActiveAccount() const;
    
    Account account;
};

#endif //JUZEN_ACCOUNTMANAGER_H
