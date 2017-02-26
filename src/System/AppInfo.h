#ifndef JUZEN_APPINFO_H
#define JUZEN_APPINFO_H

class QString;
class AppInfo
{
public:
    static QString executableDir();
    static QString dataDir();
    static QString accountDataDir(const QString &jid);

    static QString appName();
    static QString appFullName();
    static QString appVersion();
};

#endif //JUZEN_APPINFO_H
