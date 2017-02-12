#include "AccountManager.h"
#include "Widgets/ContactListView.h"
#include "Widgets/ContactListModel.h"

#include <jreen/vcardmanager.h>

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Juzen");

    ContactListView clv;
    
    AccountManager am;

    auto vcm = new Jreen::VCardManager(am.getActiveAccount()->getClient());
    auto roster = am.getActiveAccount()->getRoster();

    QObject::connect(roster, &Roster::itemAdded, [&](QSharedPointer<Jreen::RosterItem> item)
    {
        vcm->fetch(item->jid());

    });
    QObject::connect(vcm, &Jreen::VCardManager::vCardFetched, [&](const Jreen::VCard::Ptr &vcard, const Jreen::JID &jid)
    {
        clv.model()->add(vcard->formattedName(), roster->getPresenceText(jid.bare()), vcard->photo().data(), vcard->photo().mimeType());
    });

    clv.show();

    return app.exec();
}

