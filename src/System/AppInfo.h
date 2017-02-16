#ifndef JUZEN_APPINFO_H
#define JUZEN_APPINFO_H

class QString;
class AppInfo
{
public:
    static QString getExecutableDir();
    static QString getDataDir();

    static QString getAppName();
    static QString getAppFullName();
    static QString getAppVersion();
};

#endif //JUZEN_APPINFO_H
