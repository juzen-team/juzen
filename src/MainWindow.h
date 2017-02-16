#ifndef JUZEN_MAINWINDOW_H
#define JUZEN_MAINWINDOW_H

#include "Account.h"
#include "Widgets/ContactListView.h"
#include <QtWidgets/QWidget>

class QGridLayout;
class MainWindow: public QWidget
{
    Q_OBJECT

public:
    MainWindow(Account *account, QWidget *parent = Q_NULLPTR);
    virtual ~MainWindow();

    QGridLayout *layout() const;

private:
    void initWindow();

    Account *account;

    ContactListView *contactListView;
    //ContactListView *recentListView;
};

#endif //JUZEN_MAINWINDOW_H
