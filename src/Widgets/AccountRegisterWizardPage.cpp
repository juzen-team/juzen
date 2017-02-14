#include "Widgets/AccountRegisterWizardPage.h"

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>

AccountRegisterWizardPage::AccountRegisterWizardPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle("Register new account");
    setSubTitle("Please, select a server where you want to register account.");

    auto serverBox = new QComboBox(this);
    registerField("server*", serverBox);

    servers << "Select server" << "totallynota.valdikss.org.ru" << "jabber.ru";
    serverBox->addItems(servers);

    auto serverLabel = new QLabel(this);

    connect(serverBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [this, serverBox, serverLabel](int index)
            {
                wizard()->setProperty("server", serverBox->itemText(index));
                serverLabel->setText(QString("Label for server %1").arg(serverBox->itemText(index)));
            }
    );

	auto layout = new QVBoxLayout(this);
	layout->addWidget(new QLabel("Select server:"));
    layout->addWidget(serverBox);
    layout->addWidget(serverLabel);
	
	setLayout(layout);
}

AccountRegisterWizardPage::~AccountRegisterWizardPage()
{
}
