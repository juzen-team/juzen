#include "Account.h"
#include "System/AppInfo.h"
#include "System/Settings.h"

#include <QtCore/QDebug>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    auto app = new QApplication(argc, argv);
    app->setApplicationName("Juzen");
    
    Settings s(AppInfo::getDataDir() + "/config.json");
    qDebug() << s.get<QString>("Key");
    qDebug() << s.get<QString>("Key2");
    qDebug() << s.get<QString>("Key3", "not found");
    
    s.set("Key2", "new value");

    //Account account("");

    return app->exec();
}

