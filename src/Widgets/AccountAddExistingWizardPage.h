#ifndef JUZEN_ACCOUNTADDEXISTINGWIZARDPAGE_H
#define JUZEN_ACCOUNTADDEXISTINGWIZARDPAGE_H

#include "AccountManager.h"
#include "Widgets/AccountAddWizard.h"
#include <QtWidgets/QWizardPage>

class AccountAddExistingWizardPage: public QWizardPage
{
    Q_OBJECT

public:
    AccountAddExistingWizardPage(QWidget *parent = Q_NULLPTR);
    virtual ~AccountAddExistingWizardPage();

    bool isComplete() const;
    bool validatePage();

protected:
    AccountAddWizard *wizard() const;

private:
    QString m_jid;
    QString m_password;
    int m_port;
};

#endif //JUZEN_ACCOUNTADDEXISTINGWIZARDPAGE_H
