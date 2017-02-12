#ifndef JUZEN_CONTACTLISTVIEW_H
#define JUZEN_CONTACTLISTVIEW_H

#include <QtWidgets/QListView>

class ContactListModel;
class ContactListView: public QListView
{
    Q_OBJECT

public:
    ContactListView(QWidget *parent = Q_NULLPTR);
    virtual ~ContactListView();

    ContactListModel *model();
};

#endif //JUZEN_CONTACTLISTVIEW_H
