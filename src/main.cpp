#include "AccountManager.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    auto app = new QApplication(argc, argv);
    app->setApplicationName("Juzen");
    
    AccountManager am;

    return app->exec();
}

