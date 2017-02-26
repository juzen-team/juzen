#ifndef JUZEN_ICONFACTORY_H
#define JUZEN_ICONFACTORY_H

#include <jreen/presence.h> 

class QPixmap;
class QString;
class IconFactory
{
public:
    static QPixmap activityIcon(const QString &general, const QString &specific = QString());
    static QPixmap moodIcon(const QString &mood);
    static QPixmap presenceIcon(const Jreen::Presence::Type &type);
};

#endif //JUZEN_ICONFACTORY_H
