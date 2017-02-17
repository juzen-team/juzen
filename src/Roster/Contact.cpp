#include "Contact.h"
#include <QtGui/QColor>
#include <QtGui/QPainter>

#include <QtCore/QDebug>

Contact::Contact(Jreen::RosterItem::Ptr &rosterItem, QObject *parent) : QObject(parent), rosterItem(rosterItem)
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
    return "Offline";
}

QPixmap Contact::getPhoto() const
{
    return photo;
}

void Contact::vCardFetched(const Jreen::VCard::Ptr &vcard)
{
    name = vcard->formattedName();
    photo.loadFromData(vcard->photo().data(), vcard->photo().mimeType().toLatin1().data());
    emit contactChanged(getJid());
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
