#ifndef JUZEN_ACCOUNTADDWIZARD_H
#define JUZEN_ACCOUNTADDWIZARD_H

#include "AccountManager.h"
#include <QtWidgets/QWizard>

class AccountAddWizard: public QWizard
{
    Q_OBJECT

public:
	AccountAddWizard(AccountManager *am, QWidget *parent = Q_NULLPTR);
    virtual ~AccountAddWizard();

	int nextId() const;

	AccountManager *getAccountManager();

private:
	AccountManager *am;

	QWizardPage *createSelectionPage();
	QWizardPage *createRegisterPage();
	QWizardPage *createExistingPage();
	QWizardPage *createFinishPage();

	int selectionPageId;
	int registerPageId;
	int existingPageId;
	int finishPageId;

	int nextPageId = 0;
};

#endif //JUZEN_ACCOUNTADDWIZARD_H
