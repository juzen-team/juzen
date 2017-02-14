#include "Widgets/AccountRegisterFormWizardPage.h"

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollArea>

#include <QtCore/QDebug>

AccountRegisterFormWizardPage::AccountRegisterFormWizardPage(QWidget *parent) : QWizardPage(parent)
{
    setCommitPage(true);

    setTitle("Register new account");
    setSubTitle("Please, select a server where you want to register account.");

    auto layout = new QVBoxLayout(this);
    setLayout(layout);
}

AccountRegisterFormWizardPage::~AccountRegisterFormWizardPage()
{
}

void AccountRegisterFormWizardPage::initializePage()
{
    static bool initialized = false;
    if (!initialized) {
        connect(wizard()->getAccountManager(), &AccountManager::registerFormReceived,
                [this](const Jreen::RegistrationData &data) {
                    setSubTitle(data.form()->instructions());

                    dfw = new DataFormWidget(data.form(), data.bitsOfBinaries(), this);

                    clearLayout();
                    layout()->addWidget(dfw);
                }
        );
        initialized = true;
    }

    layout()->addWidget(new QLabel("Please, wait while register form loading...", this));
    wizard()->getAccountManager()->getRegisterForm(wizard()->property("server").toString());
}

void AccountRegisterFormWizardPage::cleanupPage()
{
    clearLayout();
}

bool AccountRegisterFormWizardPage::validatePage()
{
    wizard()->getAccountManager()->submitRegisterForm(dfw->getDataForm());
    return false;
}

AccountAddWizard *AccountRegisterFormWizardPage::wizard() const
{
    return qobject_cast<AccountAddWizard *>(QWizardPage::wizard());
}

void AccountRegisterFormWizardPage::clearLayout()
{
    auto item = layout()->takeAt(0);
    if (item) {
        delete item->widget();
        delete item;
    }
}
