#include "System/AppInfo.h"

#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QApplication>

QString AppInfo::getExecutableDir()
{
    return qApp->applicationDirPath();
}

QString AppInfo::getDataDir()
{
    QDir dir;

    // Check if that portable version
    dir.setPath(AppInfo::getExecutableDir());
    if (dir.exists()) {
        if (dir.cd("Juzen") || dir.cd("juzen")) {
            return dir.path();
        }
    }
    
    // Return path to default config dir
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}

QString AppInfo::getAppName()
{
    return "juzen";
}

QString AppInfo::getAppFullName()
{
    return QString("%1 v%2").arg(AppInfo::getAppName(), AppInfo::getAppVersion());
}

QString AppInfo::getAppVersion()
{
    return "0.1";
}
