#include "System/Settings.h"
#include <QtCore/QJsonDocument>

Settings::Settings(const QString &path) : configFile(path)
{
    readSettings();
}

void Settings::setPath(const QString &path)
{
    configFile.setFileName(path);
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
