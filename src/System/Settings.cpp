#include "System/Settings.h"
#include "System/AppInfo.h"
#include <QtCore/QJsonDocument>

Settings::Settings(const QString &path, bool relative)
{
    if (relative) {
        setRelativeConfigPath(path);
    } else {
        setPath(path);
    }
}

void Settings::setPath(const QString &path)
{
    configFile.setFileName(path);
    readSettings();
}

void Settings::setRelativeConfigPath(const QString &path)
{
    configFile.setFileName(QString("%1/%2.json").arg(AppInfo::getDataDir(), path));
    readSettings();
}

void Settings::readSettings()
{
    if (!configFile.open(QIODevice::ReadOnly)) {
        return;
    }
    
    auto json = QJsonDocument::fromJson(configFile.readAll());
    configFile.close();
    
    config = json.toVariant();
}

void Settings::writeSettings()
{
    if (!configFile.open(QIODevice::WriteOnly)) {
        return;
    }
    
    auto json = QJsonDocument::fromVariant(config);
    configFile.write(json.toJson());
    configFile.close();
}
