#include "AccountManager.h"
#include "MainWindow.h"
#include "System/AppInfo.h"

#include <QtCore/QTime>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(AppInfo::appName());
    app.setApplicationVersion(AppInfo::appVersion());

    auto palette = app.palette();
    palette.setColor(QPalette::Background, Qt::white);
    app.setPalette(palette);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    AccountManager am;
    if (!am.activeAccount()) {
        return 0;
    }

    MainWindow mw(am.activeAccount());
    mw.show();

    return app.exec();
}
