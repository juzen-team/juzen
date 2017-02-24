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

        connect(wizard()->accountManager(), &AccountManager::registrationFormReceived,
            [this](const Jreen::RegistrationData &data) {
                auto oldDfw = m_dataFormWidget;
                m_dataFormWidget = new DataFormWidget(data.form(), data.bitsOfBinaries(), oldDfw, this);

                clearLayout();
                layout()->addWidget(m_dataFormWidget);
                setSubTitle(data.form()->instructions());
            }
        );

        connect(wizard()->accountManager(), &AccountManager::registrationSuccess,
            [this]() {
                if (!wizard()->property("name").toString().isEmpty()) {
                    QString jid = wizard()->property("name").toString();
                    if (!jid.contains('@')) {
                        jid += QString("@%1").arg(wizard()->property("server").toString());
                    }
                    wizard()->accountManager()->addExistingAccount(jid, wizard()->property("password").toString());
                }
                m_registrationSuccess = true;
                wizard()->next();
            }
        );

        connect(wizard()->accountManager(), &AccountManager::registrationError,
            [this](const QString &errorStr) {
                QMessageBox::critical(this, "Registration failed", errorStr);
                m_dataFormWidget->hide();
                performRequest();
            }
        );

        connect(wizard()->accountManager(), &AccountManager::registrationUnsupported,
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
    m_dataFormWidget = nullptr;
}

bool AccountRegisterFormWizardPage::isComplete() const
{
    return m_pageComplete;
}

bool AccountRegisterFormWizardPage::validatePage()
{
    if (m_registrationSuccess) {
        return true;
    }

    if (!m_dataFormWidget->allRequiredFieldsFilled()) {
        QMessageBox::critical(this, "Error", "Please fill all required fields.");
        return false;
    }

    setPageComplete(false);
    wizard()->setProperty("name", m_dataFormWidget->dataForm()->field("username").value());
    wizard()->setProperty("password", m_dataFormWidget->dataForm()->field("password").value());
    wizard()->accountManager()->submitRegisterForm(m_dataFormWidget->dataForm());

    return false;
}

AccountAddWizard *AccountRegisterFormWizardPage::wizard() const
{
    return qobject_cast<AccountAddWizard *>(QWizardPage::wizard());
}

void AccountRegisterFormWizardPage::clearLayout()
{
    setSubTitle(QString());
    setPageComplete();

    QLayoutItem *item;
    while ((item = layout()->takeAt(0))) {
        delete item->widget();
        delete item;
    }
}

void AccountRegisterFormWizardPage::performRequest()
{
    setPageComplete(false);
    layout()->addWidget(new QLabel("Please, wait while register form loading...", this));
    wizard()->accountManager()->getRegisterForm(wizard()->property("server").toString());
}

void AccountRegisterFormWizardPage::setPageComplete(bool complete)
{
    m_pageComplete = complete;
    emit completeChanged();
}
