#ifndef JUZEN_ACCOUNTADDEXISTINGWIZARDPAGE_H
#define JUZEN_ACCOUNTADDEXISTINGWIZARDPAGE_H

#include "AccountManager.h"
#include <QtWidgets/QWizardPage>

class AccountAddExistingWizardPage: public QWizardPage
{
    Q_OBJECT

public:
	AccountAddExistingWizardPage(AccountManager *am, QWidget *parent = Q_NULLPTR);
    virtual ~AccountAddExistingWizardPage();

	bool isComplete() const;
	bool validatePage();

private:
	AccountManager *am;

	QString jid;
	QString password;
	int port;
};

#endif //JUZEN_ACCOUNTADDEXISTINGWIZARDPAGE_H
