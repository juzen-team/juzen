#ifndef JUZEN_ACCOUNTREGISTERWIZARDPAGE_H
#define JUZEN_ACCOUNTREGISTERWIZARDPAGE_H

#include "Widgets/AccountAddWizard.h"
#include <QtCore/QPair>
#include <QtWidgets/QWizardPage>

class AccountRegisterWizardPage: public QWizardPage
{
    Q_OBJECT

public:
    AccountRegisterWizardPage(QWidget *parent = Q_NULLPTR);
    virtual ~AccountRegisterWizardPage();

private:
    QVector<QPair<QString, QString>> servers;
};

#endif //JUZEN_ACCOUNTREGISTERWIZARDPAGE_H
