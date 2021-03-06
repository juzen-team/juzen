#include "Widgets/AccountAddExistingWizardPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>

AccountAddExistingWizardPage::AccountAddExistingWizardPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Use existing account");

    auto jidEdit = new QLineEdit(this);
    jidEdit->setPlaceholderText("jid@example.com[:port]");
    connect(jidEdit, &QLineEdit::textChanged, [&](const QString &text)
    {
        m_jid = text.section(':', 0, 0);
        QString portStr = text.section(':', 1, 1);
        m_port = (portStr.isEmpty() ? -1 : portStr.toInt());
        emit completeChanged();
    });
    
    auto passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    connect(passwordEdit, &QLineEdit::textChanged, [&](const QString &text)
    {
        m_password = text;
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
    return !m_jid.isEmpty() && !m_password.isEmpty();
}

bool AccountAddExistingWizardPage::validatePage()
{
    if (m_jid.count('@') != 1) {
        QMessageBox::critical(this, "Error", "Please enter jid in following format: jid@example.com[:port].");
        return false;
    }

    return wizard()->accountManager()->addExistingAccount(m_jid, m_password, m_port);
}

AccountAddWizard *AccountAddExistingWizardPage::wizard() const
{
    return qobject_cast<AccountAddWizard *>(QWizardPage::wizard());
}
