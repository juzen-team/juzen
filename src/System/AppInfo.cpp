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
        if (dir.cd("Juzen")) {
            return dir.path();
        }
    }
    
    // Return path to default config dir
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}
