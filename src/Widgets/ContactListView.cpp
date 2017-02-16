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

    QObject::connect(roster, &Roster::itemAdded,
        [&](QSharedPointer<Jreen::RosterItem> item)
        {
            model()->add(item->name(), roster->getPresenceText(item->jid()), QByteArray(), QString());
        }
    );
}

ContactListView::~ContactListView()
{
}

ContactListModel *ContactListView::model()
{
    return qobject_cast<ContactListModel *>(QListView::model());
}
