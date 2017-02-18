#include "Contact.h"
#include "Roster.h"
#include <QtCore/QSharedPointer>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <algorithm>

#include <QtCore/QDebug>

Contact::Contact(Jreen::RosterItem::Ptr &rosterItem, Roster *roster) : QObject(roster),
                                                                       roster(roster),
                                                                       rosterItem(rosterItem)
{
    generatePhoto();
}

Contact::~Contact()
{
}

QString Contact::getJid() const
{
    return rosterItem->jid();
}

QString Contact::getName() const
{
    if (!name.isEmpty()) {
        return name;
    }
    return rosterItem->name();
}

QString Contact::getPresenceText() const
{
    if (resources.size() == 0) {
        return "Offline";
    }
    return resources[0]->presenceText();
}

QPixmap Contact::getPhoto() const
{
    return photo;
}

void Contact::presenceReceived(const Jreen::Presence &presence)
{
    if (presence.subtype() == Jreen::Presence::Unavailable) {
        removeResource(presence.from().resource());
    } else {
        addOrChangeResource(presence);
    }
    std::sort(resources.rbegin(), resources.rend());
    emit contactChanged(getJid());
}

void Contact::vCardFetched(const Jreen::VCard::Ptr &vcard)
{
    name = vcard->formattedName();
    photo.loadFromData(vcard->photo().data(), vcard->photo().mimeType().toLatin1().data());
    emit contactChanged(getJid());
}

void Contact::addOrChangeResource(const Jreen::Presence &presence)
{
    auto r = std::find_if(resources.begin(), resources.end(),
        [&presence](auto r)
        {
            return r->resource() == presence.from().resource();
        }
    );
    if (r != resources.end()) {
        (*r)->setPresence(presence);
    } else {
        auto resource = QSharedPointer<ContactResource>::create(presence, this);
        resources.push_back(resource);
    }
}

void Contact::removeResource(const QString &resource)
{
    resources.erase(std::remove_if(resources.begin(), resources.end(),
        [&resource](auto r)
        {
            return r->resource() == resource;
        }
    ));
}

void Contact::generatePhoto()
{
    QPixmap tmpPhoto(40, 40);
    QPainter painter(&tmpPhoto);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    auto colorNames = QColor::colorNames();
    colorNames.removeAll("black");
    colorNames.removeAll("white");
    auto randColorName = colorNames[qrand() % (colorNames.size() + 1)];

    QRect rect(0, 0, tmpPhoto.width(), tmpPhoto.height());
    painter.fillRect(rect, QBrush(QColor(randColorName)));

    QFont font;
    font.setPointSize(30);
    font.setStyleStrategy(static_cast<QFont::StyleStrategy>(QFont::PreferAntialias | QFont::PreferQuality));
    painter.setFont(font);

    painter.drawText(rect, Qt::AlignCenter, QString(getName()[0]));

    painter.end();
    photo = tmpPhoto;
}
