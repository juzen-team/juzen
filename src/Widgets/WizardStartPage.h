#ifndef JUZEN_WIZARDSTARTPAGE_H
#define JUZEN_WIZARDSTARTPAGE_H

#include <QtWidgets/QWizardPage>

class WizardStartPage: public QWizardPage
{
    Q_OBJECT

public:
    WizardStartPage(QWidget *parent = Q_NULLPTR);
    virtual ~WizardStartPage();

    bool isComplete() const;
};

#endif //JUZEN_WIZARDSTARTPAGE_H
