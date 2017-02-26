#include "Resources/IconFactory.h"
#include <QtGui/QPixmap>

QPixmap IconFactory::activityIcon(const QString &general, const QString &specific)
{
    auto name = QString(":/activities/%1%2%3.png").arg(general);
    if (specific.isEmpty() || specific == "other") {
        name = name.arg(QString(), QString());
    } else {
        name = name.arg("_", specific);
    }

    QPixmap ret = QPixmap(name);
    if (ret.isNull()) {
        ret = QPixmap(":/activities/unknown.png");
    }

    return ret;
}

QPixmap IconFactory::moodIcon(const QString &mood)
{
    auto name = QString(":/moods/%1.png").arg(mood);

    QPixmap ret = QPixmap(name);
    if (ret.isNull()) {
        ret = QPixmap(":/moods/undefined.png");
    }

    return ret;
}

QPixmap IconFactory::presenceIcon(const Jreen::Presence::Type &type)
{
    switch (type) {
        case Jreen::Presence::Available:
            return QPixmap(":/presences/available.png");

        case Jreen::Presence::Chat:
            return QPixmap(":/presences/chat.png");

        case Jreen::Presence::Away:
            return QPixmap(":/presences/away.png");

        case Jreen::Presence::DND:
            return QPixmap(":/presences/dnd.png");

        case Jreen::Presence::XA:
            return QPixmap(":/presences/xa.png");

        default:
            return QPixmap(":/presences/unavailable.png");
    }
}
