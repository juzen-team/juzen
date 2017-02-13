#include "Widgets/AccountAddWizard.h"
#include "Widgets/AccountAddExistingWizardPage.h"
#include "Widgets/WizardStartPage.h"

#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QLayout>

AccountAddWizard::AccountAddWizard(AccountManager *am, QWidget *parent) : QWizard(parent), am(am)
{
	setWindowTitle("Add new account");
	setOptions(QWizard::DisabledBackButtonOnLastPage | QWizard::NoCancelButtonOnLastPage);

	selectionPageId = addPage(createSelectionPage());
	registerPageId = addPage(createRegisterPage());
	existingPageId = addPage(createExistingPage());
	finishPageId = addPage(createFinishPage());
}

AccountAddWizard::~AccountAddWizard()
{
}

int AccountAddWizard::nextId() const
{
	if (currentId() == selectionPageId) {
		return nextPageId;
	}
	if (currentId() == registerPageId || currentId() == existingPageId) {
		return finishPageId;
	}

	return -1;
}

AccountManager *AccountAddWizard::getAccountManager()
{
    return am;
}

QWizardPage *AccountAddWizard::createSelectionPage()
{
	auto page = new WizardStartPage(this);

	page->setTitle("Add new account");
	page->setSubTitle("You haven't any account yet. Do you want to register new account or use existing one?");

	auto registerButton = new QCommandLinkButton("Register new account", "Register new account on selected server.", this);
	connect(registerButton, &QCommandLinkButton::clicked, [&](bool /*checked*/)
	{
		nextPageId = registerPageId;
		next();
	});
	auto existingButton = new QCommandLinkButton("Use existing account", this);
	connect(existingButton, &QCommandLinkButton::clicked, [&](bool /*checked*/)
	{
		nextPageId = existingPageId;
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
	auto page = new QWizardPage(this);

	page->setTitle("Register new account");
	page->setSubTitle("Register new account on selected server. Please fill all fields.");

	auto layout = new QVBoxLayout(this);
	page->setLayout(layout);

	return page;
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
