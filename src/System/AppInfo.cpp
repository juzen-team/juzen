#include "System/AppInfo.h"

#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QApplication>

QString AppInfo::executableDir()
{
    return qApp->applicationDirPath();
}

QString AppInfo::dataDir()
{
    QDir dir;

    // Check if that portable version
    dir.setPath(AppInfo::executableDir());
    if (dir.exists()) {
        if (dir.cd("Juzen") || dir.cd("juzen")) {
            return dir.path();
        }
    }
    
    // Return path to default config dir
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}

QString AppInfo::accountDataDir(const QString &jid)
{
    return QString("%1/accounts/%2").arg(dataDir(), jid);
}

QString AppInfo::appName()
{
    return "juzen";
}

QString AppInfo::appFullName()
{
    return QString("%1 v%2").arg(AppInfo::appName(), AppInfo::appVersion());
}

QString AppInfo::appVersion()
{
    return "0.1";
}
