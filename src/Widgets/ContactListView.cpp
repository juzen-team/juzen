#include "Widgets/ContactListView.h"
#include "Widgets/ContactListDelegate.h"
#include "Widgets/ContactListModel.h"

ContactListView::ContactListView(Roster *roster, QWidget *parent) : QListView(parent), roster(roster)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto oldModel = model();
    setModel(new ContactListModel(this));
    delete oldModel;

    auto oldDelegate = itemDelegate();
    setItemDelegate(new ContactListDelegate(this));
    delete oldDelegate;

    initRosterConnections();
}

ContactListView::~ContactListView()
{
}

ContactListModel *ContactListView::model()
{
    return qobject_cast<ContactListModel *>(QListView::model());
}

void ContactListView::initRosterConnections()
{
    connect(roster, &Roster::contactAdded,
        [&](Contact::Ptr contact)
        {
            addContact(contact);
        }
    );
}

void ContactListView::addContact(Contact::Ptr contact)
{
    model()->add(contact);
}
