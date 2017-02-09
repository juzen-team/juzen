#include <jreen/client.h>
#include <jreen/disco.h>
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

    QObject::connect(&client, &Client::connected, []() {
        cout << "connected" << endl;
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

