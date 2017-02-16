#ifndef JUZEN_ACCOUNTREGISTERFORMWIZARDPAGE_H
#define JUZEN_ACCOUNTREGISTERFORMWIZARDPAGE_H

#include "AccountManager.h"
#include "Widgets/AccountAddWizard.h"
#include "Widgets/DataFormWidget.h"
#include <QtWidgets/QWizardPage>

class AccountRegisterFormWizardPage: public QWizardPage
{
    Q_OBJECT

public:
    AccountRegisterFormWizardPage(QWidget *parent = Q_NULLPTR);
    virtual ~AccountRegisterFormWizardPage();

    void initializePage();
    void cleanupPage();
    bool isComplete() const;
    bool validatePage();

protected:
    AccountAddWizard *wizard() const;

private:
    void clearLayout();
    void performRequest();
    void setPageComplete(bool complete = true);

    DataFormWidget *dfw = nullptr;
    bool registrationSuccess = false;
    bool pageComplete = false;
};

#endif //JUZEN_ACCOUNTREGISTERFORMWIZARDPAGE_H
