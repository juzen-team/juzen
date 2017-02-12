#include "Widgets/ContactListView.h"
#include "Widgets/ContactListDelegate.h"
#include "Widgets/ContactListModel.h"

ContactListView::ContactListView(QWidget *parent) : QListView(parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto oldModel = model();
    setModel(new ContactListModel(this));
    delete oldModel;

    auto oldDelegate = itemDelegate();
    setItemDelegate(new ContactListDelegate(this));
    delete oldDelegate;
}

ContactListView::~ContactListView()
{
}

ContactListModel *ContactListView::model()
{
    return qobject_cast<ContactListModel *>(QListView::model());
}
