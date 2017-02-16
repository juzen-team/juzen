#ifndef JUZEN_CONTACTLISTVIEW_H
#define JUZEN_CONTACTLISTVIEW_H

#include "Roster/Roster.h"
#include <QtWidgets/QListView>

class ContactListModel;
class ContactListView: public QListView
{
    Q_OBJECT

public:
    ContactListView(Roster *roster, QWidget *parent = Q_NULLPTR);
    virtual ~ContactListView();

    ContactListModel *model();

private:
    Roster *roster;
};

#endif //JUZEN_CONTACTLISTVIEW_H
