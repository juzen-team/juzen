#ifndef JUZEN_ACCOUNTMANAGER_H
#define JUZEN_ACCOUNTMANAGER_H

#include "Account.h"
#include <QtCore/QObject>

class QString;
class AccountManager: public QObject
{
    Q_OBJECT

public:
    AccountManager();

    Account *getActiveAccount();

private:
    QString findActiveAccount();
    
    Account account;
};

#endif //JUZEN_ACCOUNTMANAGER_H
