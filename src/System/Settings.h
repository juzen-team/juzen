#ifndef JUZEN_SETTINGS_H
#define JUZEN_SETTINGS_H

#include <QtCore/QFile>
#include <QtCore/QVariant>

class Settings : public QObject
{
	Q_OBJECT

public:
    Settings(const QString &path = QString(), bool relative = true);
    
    void setPath(const QString &path);
    void setRelativeConfigPath(const QString &path);
    
    template<typename T>
    T get(const QString &key, const T &defaultValue = T())
    {
        if (config.type() == QVariant::List) {
            auto list = config.toList();
            int index = key.toInt();
            
            if (index >= list.count()) {
                return defaultValue;
            }
            
            return list.at(index).template value<T>();
        }
        
        if (config.type() == QVariant::Map) {
            auto map = config.toMap();
            return map.value(key, defaultValue).template value<T>();
        }
        
        return defaultValue;
    }

    template<typename T>
    void set(const QString &key, const T &value)
    {
        if (config.type() == QVariant::List) {
            auto list = config.toList();
            int index = key.toInt();
            
            if (index >= list.count()) {
                list.append(value);
            } else {
                list[index] = value;
            }
            
            config = list;
        } else {
			QMap<QString, QVariant> map;
			if (config.type() == QVariant::Map) {
				map = config.toMap();
			}
            map[key] = value;
            config = map;
        }
        
        writeSettings();
        emit settingsChanged(key);
    }
    
signals:
    void settingsChanged(const QString &key);
    
private:
    void readSettings();
    void writeSettings();
    
    QFile configFile;
    QVariant config;
};

#endif //JUZEN_SETTINGS_H
