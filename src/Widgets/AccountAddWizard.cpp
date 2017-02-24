#include "Widgets/AccountAddWizard.h"
#include "Widgets/AccountAddExistingWizardPage.h"
#include "Widgets/AccountRegisterWizardPage.h"
#include "Widgets/AccountRegisterFormWizardPage.h"
#include "Widgets/WizardStartPage.h"

#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QLayout>

AccountAddWizard::AccountAddWizard(AccountManager *accountManager, QWidget *parent) : QWizard(parent), m_accountManager(accountManager)
{
    setWindowTitle("Add new account");
    setOptions(QWizard::DisabledBackButtonOnLastPage | QWizard::NoCancelButtonOnLastPage);

    m_selectionPageId = addPage(createSelectionPage());
    m_registerPageId = addPage(createRegisterPage());
    m_registerFormPageId = addPage(createRegisterFormPage());
    m_existingPageId = addPage(createExistingPage());
    m_finishPageId = addPage(createFinishPage());
}

AccountAddWizard::~AccountAddWizard()
{
}

int AccountAddWizard::nextId() const
{
    if (currentId() == m_selectionPageId) {
        return m_nextPageId;
    }
    if (currentId() == m_registerPageId) {
        return m_registerFormPageId;
    }
    if (currentId() == m_registerFormPageId) {
        if (property("name").toString().isEmpty()) {
            return m_existingPageId;
        }
        return m_finishPageId;
    }
    if (currentId() == m_existingPageId) {
        return m_finishPageId;
    }

    return -1;
}

AccountManager *AccountAddWizard::accountManager()
{
    return m_accountManager;
}

QWizardPage *AccountAddWizard::createSelectionPage()
{
    auto page = new WizardStartPage(this);

    page->setTitle("Add new account");
    page->setSubTitle("You haven't any account yet. Do you want to register new account or use existing one?");

    auto registerButton = new QCommandLinkButton("Register new account", "Register new account on selected server.", this);
    connect(registerButton, &QCommandLinkButton::clicked, [&](bool /*checked*/)
    {
        m_nextPageId = m_registerPageId;
        next();
    });
    auto existingButton = new QCommandLinkButton("Use existing account", this);
    connect(existingButton, &QCommandLinkButton::clicked, [&](bool /*checked*/)
    {
        m_nextPageId = m_existingPageId;
        next();
    });

    auto layout = new QVBoxLayout(this);
    layout->addWidget(registerButton);
    layout->addWidget(existingButton);
    page->setLayout(layout);

    return page;
}

QWizardPage *AccountAddWizard::createRegisterPage()
{
    return new AccountRegisterWizardPage(this);
}

QWizardPage *AccountAddWizard::createRegisterFormPage()
{
    return new AccountRegisterFormWizardPage(this);
}

QWizardPage *AccountAddWizard::createExistingPage()
{
    return new AccountAddExistingWizardPage(this);
}

QWizardPage *AccountAddWizard::createFinishPage()
{
    auto page = new QWizardPage(this);

    page->setTitle("Account added");
    page->setSubTitle("Now you can connect to server.");

    auto layout = new QVBoxLayout(this);
    page->setLayout(layout);

    return page;
}
