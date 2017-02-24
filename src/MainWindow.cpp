#include "MainWindow.h"
#include "System/AppInfo.h"
#include "Widgets/ContactListModel.h"

#include <jreen/vcardmanager.h>

#include <QtWidgets/QGridLayout>

MainWindow::MainWindow(Account *m_account, QWidget *parent) : QWidget(parent), m_account(m_account)
{
    //setAttribute(Qt::WA_QuitOnClose, false);
    setWindowTitle(AppInfo::appFullName());

    auto layout = new QGridLayout(this);
    initWindow();
    setLayout(layout);

    m_account->connectToServer();
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
    m_contactListView = new ContactListView(m_account->roster(), this);
    layout()->addWidget(m_contactListView, 0, 0);
}
