#ifndef JUZEN_APPINFO_H
#define JUZEN_APPINFO_H

class QString;
class AppInfo
{
public:
    static QString getExecutableDir();
    static QString getDataDir();
};

#endif //JUZEN_APPINFO_H
