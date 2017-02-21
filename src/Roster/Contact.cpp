#include "Contact.h"
#include "Roster.h"
#include <QtCore/QSharedPointer>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <algorithm>

#include <QtCore/QDebug>

Contact::Contact(Jreen::RosterItem::Ptr &rosterItem, QObject *parent) : QObject(parent),
                                                                        rosterItem(rosterItem)
{
}

Contact::~Contact()
{
}

QString Contact::jid() const
{
    return rosterItem->jid();
}

QString Contact::name() const
{
    if (vcard.isNull()) {
        return rosterItem->name();
    }

    return "name";
    //return vcard->formattedName();
}

QPixmap Contact::photo() const
{
    return contactPhoto;
}

ContactResource::Ptr Contact::mainResource() const
{
    if (resources.size() == 0) {
        return QSharedPointer<ContactResource>();
    }
    return resources[0];
}

void Contact::presenceReceived(const Jreen::Presence &presence)
{
    if (presence.subtype() == Jreen::Presence::Unavailable) {
        removeResource(presence.from().resource());
    } else {
        addOrChangeResource(presence);
    }
    std::sort(resources.rbegin(), resources.rend());
    emit contactChanged(jid());
}

void Contact::vCardFetched(const Jreen::VCard::Ptr &vcard)
{
    //contactPhoto.loadFromData(vcard->photo().data(), vcard->photo().mimeType().toLatin1().data());
    emit contactChanged(jid());
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

QPixmap Contact::generateNoPhotoReplacement() const
{
    QPixmap replacementPhoto(40, 40);
    QPainter painter(&replacementPhoto);

    auto colorNames = QColor::colorNames();
    colorNames.removeAll("black");
    colorNames.removeAll("white");
    auto randColorName = colorNames[qrand() % (colorNames.size() + 1)];

    QRect rect(0, 0, replacementPhoto.width(), replacementPhoto.height());
    painter.fillRect(rect, QBrush(QColor(randColorName)));

    QFont font;
    font.setPointSize(30);
    font.setStyleStrategy(static_cast<QFont::StyleStrategy>(QFont::PreferAntialias | QFont::PreferQuality));
    painter.setFont(font);

    painter.drawText(rect, Qt::AlignCenter, QString(name()[0]));

    painter.end();

    return replacementPhoto;
}
