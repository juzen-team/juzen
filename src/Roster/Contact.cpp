#include "Contact.h"
#include "Roster.h"
#include <QtCore/QSharedPointer>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <algorithm>

#include <QtCore/QDebug>

Contact::Contact(Jreen::RosterItem::Ptr &rosterItem, QObject *parent) : QObject(parent),
                                                                        m_rosterItem(rosterItem)
{
    contactPhoto = generateNoPhotoFiller();
}

Contact::~Contact()
{
}

QString Contact::jid() const
{
    return m_rosterItem->jid();
}

QString Contact::name() const
{
    if (m_vcard) {
        return m_vcard->formattedName();
    }

    if (m_rosterItem) {
        return m_rosterItem->name();
    }

    return QString();    
}

QPixmap Contact::photo() const
{
    return contactPhoto;
}

ContactResource::Ptr Contact::mainResource() const
{
    if (m_resources.size() == 0) {
        return QSharedPointer<ContactResource>();
    }
    return m_resources[0];
}

QVector<ContactResource::Ptr> Contact::allResources() const
{
    return m_resources;
}

void Contact::presenceReceived(const Jreen::Presence &presence)
{
    if (presence.subtype() == Jreen::Presence::Unavailable) {
        if (m_resources.size() > 0) {
            removeResource(presence.from().resource());
        }
    } else {
        addOrChangeResource(presence);
    }
    std::sort(m_resources.rbegin(), m_resources.rend());
    emit contactChanged(jid());
}

void Contact::vCardFetched(const Jreen::VCard::Ptr &vcard)
{
    contactPhoto.loadFromData(vcard->photo().data(), vcard->photo().mimeType().toLatin1().data());
    this->m_vcard = vcard;
    emit contactChanged(jid());
}

void Contact::addOrChangeResource(const Jreen::Presence &presence)
{
    auto r = std::find_if(m_resources.begin(), m_resources.end(),
        [&presence](auto r)
        {
            return r->resource() == presence.from().resource();
        }
    );
    if (r != m_resources.end()) {
        (*r)->setPresence(presence);
    } else {
        auto resource = QSharedPointer<ContactResource>::create(presence, this);
        m_resources.push_back(resource);
    }
}

void Contact::removeResource(const QString &resource)
{
    m_resources.erase(std::remove_if(m_resources.begin(), m_resources.end(),
        [&resource](auto r)
        {
            return r->resource() == resource;
        }
    ));
}

QPixmap Contact::generateNoPhotoFiller() const
{
    QPixmap replacementPhoto(40, 40);
    QPainter painter(&replacementPhoto);

    auto colorNames = QColor::colorNames();
    colorNames.removeAll("black");
    colorNames.removeAll("white");
    auto randColorName = colorNames[qrand() % colorNames.size()];

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
