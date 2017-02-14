#ifndef JUZEN_ACCOUNTREGISTERWIZARDPAGE_H
#define JUZEN_ACCOUNTREGISTERWIZARDPAGE_H

#include "Widgets/AccountAddWizard.h"
#include <QtWidgets/QWizardPage>

class AccountRegisterWizardPage: public QWizardPage
{
    Q_OBJECT

public:
	AccountRegisterWizardPage(QWidget *parent = Q_NULLPTR);
    virtual ~AccountRegisterWizardPage();

private:
	QStringList servers;
};

#endif //JUZEN_ACCOUNTREGISTERWIZARDPAGE_H
