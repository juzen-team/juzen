#ifndef JUZEN_ACCOUNTADDWIZARD_H
#define JUZEN_ACCOUNTADDWIZARD_H

#include "AccountManager.h"
#include <QtWidgets/QWizard>

class AccountAddWizard: public QWizard
{
    Q_OBJECT

public:
    AccountAddWizard(AccountManager *accountManager, QWidget *parent = Q_NULLPTR);
    virtual ~AccountAddWizard();

    int nextId() const;

    AccountManager *accountManager();

private:
    QWizardPage *createSelectionPage();
    QWizardPage *createRegisterPage();
    QWizardPage *createRegisterFormPage();
    QWizardPage *createExistingPage();
    QWizardPage *createFinishPage();

    AccountManager *m_accountManager;

    int m_selectionPageId;
    int m_registerPageId;
    int m_registerFormPageId;
    int m_existingPageId;
    int m_finishPageId;

    int m_nextPageId = 0;
};

#endif //JUZEN_ACCOUNTADDWIZARD_H
