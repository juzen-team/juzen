#ifndef JUZEN_ACCOUNTMANAGER_H
#define JUZEN_ACCOUNTMANAGER_H

#include "Account.h"
#include <jreen/registrationmanager.h>
#include <QtCore/QObject>

class QStringList;
class AccountManager: public QObject
{
    Q_OBJECT

public:
    AccountManager();

	bool addExistingAccount(const QString &jid, const QString &password, int port);

	void getRegisterForm(const QString &server);
    void submitRegisterForm(const Jreen::DataForm::Ptr &form);

    Account *getActiveAccount();

signals:
	void registerFormReceived(const Jreen::RegistrationData &data);
	void registerAccountStateChanged(bool result, const QString &resultStr = QString());

private:
	QStringList findAllAccounts() const;
    QString findActiveAccount() const;

    void createRegistrationObjects(const QString &server);
    void deleteRegistrationObjects();
    
    Account account;

    QString registrationServer;
    Jreen::Client *registrationClient = nullptr;
    Jreen::RegistrationManager *registrationManager = nullptr;
};

#endif //JUZEN_ACCOUNTMANAGER_H
