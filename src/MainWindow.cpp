#include "MainWindow.h"
#include "System/AppInfo.h"
#include "Widgets/ContactListModel.h"

#include <jreen/vcardmanager.h>

#include <QtWidgets/QGridLayout>

MainWindow::MainWindow(Account *account, QWidget *parent) : QWidget(parent), account(account)
{
    //setAttribute(Qt::WA_QuitOnClose, false);
    setWindowTitle(AppInfo::getAppFullName());

    auto layout = new QGridLayout(this);
    initWindow();
    setLayout(layout);

    account->connectToServer();
}

MainWindow::~MainWindow()
{
}

QGridLayout *MainWindow::layout() const
{
    return qobject_cast<QGridLayout *>(QWidget::layout());
}

void MainWindow::initWindow()
{
    contactListView = new ContactListView(account->getRoster(), this);
    layout()->addWidget(contactListView, 0, 0);
}
