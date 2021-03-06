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

    bool addExistingAccount(const QString &jid, const QString &password, int port = -1);

    void getRegisterForm(const QString &server);
    void submitRegisterForm(const Jreen::DataForm::Ptr &form);

    Account *activeAccount();

signals:
    void registrationFormReceived(const Jreen::RegistrationData &data);
    void registrationSuccess();
    void registrationError(const QString &errorStr);
    void registrationUnsupported();

private:
    QStringList findAllAccounts() const;
    QString findActiveAccount() const;

    void createRegistrationObjects(const QString &server);
    void deleteRegistrationObjects();
    
    Account m_account;

    Jreen::Client *m_registrationClient = nullptr;
    Jreen::RegistrationManager *m_registrationManager = nullptr;
};

#endif //JUZEN_ACCOUNTMANAGER_H
