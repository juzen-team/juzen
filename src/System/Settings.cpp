#include "System/Settings.h"
#include "System/AppInfo.h"
#include <QtCore/QJsonDocument>

Settings::Settings(const QString &path)
{
    m_configFile.setFileName(path);
    readSettings();
}

Settings::Settings(const Settings &other) : m_config(other.m_config)
{
    m_configFile.setFileName(other.m_configFile.fileName());
}

Settings Settings::config()
{
    return Settings(QString("%1/config.json").arg(AppInfo::dataDir()));
}

Settings Settings::accountConfig(const QString &account)
{
    return Settings(QString("%1/config.json").arg(AppInfo::accountDataDir(account)));
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
