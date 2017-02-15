#include "Widgets/AccountRegisterFormWizardPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QMessageBox>

AccountRegisterFormWizardPage::AccountRegisterFormWizardPage(QWidget *parent) : QWizardPage(parent)
{
    setCommitPage(true);

    setTitle("Register new account");

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
        initialized = true;

        connect(wizard()->getAccountManager(), &AccountManager::registrationFormReceived,
            [this](const Jreen::RegistrationData &data) {
				auto oldDfw = dfw;
                dfw = new DataFormWidget(data.form(), data.bitsOfBinaries(), oldDfw, this);

                clearLayout();
                layout()->addWidget(dfw);
                setSubTitle(data.form()->instructions());
            }
        );

        connect(wizard()->getAccountManager(), &AccountManager::registrationSuccess,
            [this]() {
                if (!wizard()->property("name").toString().isEmpty()) {
                    QString jid = wizard()->property("name").toString();
                    if (!jid.contains('@')) {
                        jid += QString("@%1").arg(wizard()->property("server").toString());
                    }
                    wizard()->getAccountManager()->addExistingAccount(jid, wizard()->property("password").toString());
                }
                registrationSuccess = true;
                wizard()->next();
            }
        );

        connect(wizard()->getAccountManager(), &AccountManager::registrationError,
            [this](const QString &errorStr) {
                QMessageBox::critical(this, "Registration failed", errorStr);
                dfw->hide();
                performRequest();
            }
        );

        connect(wizard()->getAccountManager(), &AccountManager::registrationUnsupported,
            [this]() {
                clearLayout();
                layout()->addWidget(new QLabel("Sorry, this server doesn't support in-band registeration.", this));
            }
        );
    }

    clearLayout();
    performRequest();
}

void AccountRegisterFormWizardPage::cleanupPage()
{
    clearLayout();
	dfw = nullptr;
}

bool AccountRegisterFormWizardPage::validatePage()
{
    if (registrationSuccess) {
        return true;
    }

    wizard()->setProperty("name", dfw->getDataForm()->field("username").value());
    wizard()->setProperty("password", dfw->getDataForm()->field("password").value());
    wizard()->getAccountManager()->submitRegisterForm(dfw->getDataForm());

    return false;
}

AccountAddWizard *AccountRegisterFormWizardPage::wizard() const
{
    return qobject_cast<AccountAddWizard *>(QWizardPage::wizard());
}

void AccountRegisterFormWizardPage::clearLayout()
{
    setSubTitle(QString());
    QLayoutItem *item;
    while ((item = layout()->takeAt(0))) {
        delete item->widget();
        delete item;
    }
}

void AccountRegisterFormWizardPage::performRequest()
{
    layout()->addWidget(new QLabel("Please, wait while register form loading...", this));
    wizard()->getAccountManager()->getRegisterForm(wizard()->property("server").toString());
}
