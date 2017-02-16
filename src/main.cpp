#include "AccountManager.h"
#include "MainWindow.h"
#include "System/AppInfo.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(AppInfo::getAppName());
    app.setApplicationVersion(AppInfo::getAppVersion());

    auto palette = app.palette();
    palette.setColor(QPalette::Background, Qt::white);
    app.setPalette(palette);

    AccountManager am;
    if (!am.getActiveAccount()) {
        return 0;
    }

    MainWindow mw(am.getActiveAccount());
    mw.show();

    return app.exec();
}
