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

    // TODO: read servers from config file?
    servers << qMakePair<QString, QString>("Select server", "Please, select any server from list.");
    servers << qMakePair<QString, QString>("totallynota.valdikss.org.ru", "Valdikss's server for tests.\nProsody 0.10 nightly build 345 (2017-02-12, 74187ee6ed55).");
    servers << qMakePair<QString, QString>("jabber.ru", "The largest jabber-server in Russia.\nejabberd 3.2.x.");
    
    for (auto &server : servers) {
        serverBox->addItem(server.first);
    }

    auto serverLabel = new QLabel(servers[0].second, this);
    connect(serverBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        [this, serverBox, serverLabel](int index)
        {
            wizard()->setProperty("server", servers[index].first);
            serverLabel->setText(servers[index].second);
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
