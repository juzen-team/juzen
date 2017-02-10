#include <jreen/client.h>
#include <jreen/abstractroster.h>
#include <jreen/message.h>
#include <QtCore/QObject>
#include <QtWidgets/QApplication>
#include <iostream>
using namespace Jreen;
using namespace std;

int main(int argc, char *argv[])
{
    auto app = new QApplication(argc, argv);

    Client client(JID("jid"), "password");

    auto disco = client.disco();
    disco->setSoftwareVersion("Juzen", "0.1");
	disco->addIdentity(Jreen::Disco::Identity("client", "type", "juzen", "en"));

    SimpleRoster sr(&client);
    QObject::connect(&sr, &SimpleRoster::loaded, []() {
        cout << "Roster loaded" << endl;
    });
    QObject::connect(&sr, &SimpleRoster::itemAdded, [](const Jreen::RosterItem::Ptr &item) {
        cout << "itemAdded: " << item->jid().toStdString() << endl;
    });
    QObject::connect(&sr, &SimpleRoster::presenceReceived, [](const Jreen::RosterItem::Ptr &item, const Jreen::Presence &presence) {
        cout << "presenceReceived (from: " << presence.from().full().toStdString() << "): " << presence.subtype() << endl;
    });
    QObject::connect(&sr, &SimpleRoster::subscriptionReceived, [](const Jreen::RosterItem::Ptr &item, const Jreen::Presence &presence) {
        cout << "subscriptionReceived (from: " << presence.from().full().toStdString() << "): " << presence.subtype() << endl;
    });

    QObject::connect(&client, &Client::connected, [&]() {
        cout << "connected" << endl;
        client.setPresence(Presence::Available);
        sr.load();
    });
    QObject::connect(&client, &Client::disconnected, [](Client::DisconnectReason reason) {
        cout << "disconnected with reason " << reason << endl;
    });
    QObject::connect(&client, &Client::messageReceived, [](const Message &message) {
        cout << "message (from: " << message.from().full().toStdString() << "): " << message.body().toStdString() << endl;
    });


    client.connectToServer();
    return app->exec();
}

