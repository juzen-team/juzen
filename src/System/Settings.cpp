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
    m_configFile.setFileName(path);
    readSettings();
}

void Settings::setRelativeConfigPath(const QString &path)
{
    m_configFile.setFileName(QString("%1/%2.json").arg(AppInfo::dataDir(), path));
    readSettings();
}

void Settings::readSettings()
{
    if (!m_configFile.open(QIODevice::ReadOnly)) {
        return;
    }
    
    auto json = QJsonDocument::fromJson(m_configFile.readAll());
    m_configFile.close();
    
    m_config = json.toVariant();
}

void Settings::writeSettings()
{
    if (!m_configFile.open(QIODevice::WriteOnly)) {
        return;
    }
    
    auto json = QJsonDocument::fromVariant(m_config);
    m_configFile.write(json.toJson());
    m_configFile.close();
}
