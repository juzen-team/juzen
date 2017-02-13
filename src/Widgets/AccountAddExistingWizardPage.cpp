#include "Widgets/AccountAddExistingWizardPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLineEdit>

AccountAddExistingWizardPage::AccountAddExistingWizardPage(AccountManager *am, QWidget *parent) : am(am), QWizardPage(parent)
{
	setTitle("Use existing account");

	auto jidEdit = new QLineEdit(this);
	jidEdit->setPlaceholderText("jid@example.com[:port]");
	connect(jidEdit, &QLineEdit::textChanged, [&](const QString &text)
	{
		jid = text.section(':', 0, 0);
		QString portStr = text.section(':', 1, 1);
		port = (portStr.isEmpty() ? -1 : portStr.toInt());
		emit completeChanged();
	});
	
	auto passwordEdit = new QLineEdit(this);
	passwordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
	connect(passwordEdit, &QLineEdit::textChanged, [&](const QString &text)
	{
		password = text;
		emit completeChanged();
	});

	auto layout = new QVBoxLayout(this);
	layout->addWidget(new QLabel("Enter your JID:"));
	layout->addWidget(jidEdit);
	layout->addWidget(new QLabel("Enter your password:"));
	layout->addWidget(passwordEdit);
	
	setLayout(layout);
}

AccountAddExistingWizardPage::~AccountAddExistingWizardPage()
{
}

bool AccountAddExistingWizardPage::isComplete() const
{
	return !jid.isEmpty() && !password.isEmpty();
}

bool AccountAddExistingWizardPage::validatePage()
{
	if (jid.count('@') != 1) {
		setSubTitle("Please enter jid in following format: jid@example.com[:port]");
		return false;
	}

	return am->addExistingAccount(jid, password, port);
}
