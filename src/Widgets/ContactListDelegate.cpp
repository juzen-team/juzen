#include "Roster/Contact.h"
#include "Widgets/ContactListDelegate.h"
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

ContactListDelegate::ContactListDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

ContactListDelegate::~ContactListDelegate()
{
}

void ContactListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    auto nameFont = painter->font();
    nameFont.setBold(true);

    auto statusFont = painter->font();
    statusFont.setItalic(true);

    auto nameRect = option.rect;
    nameRect.setLeft(nameRect.left() + 55);
    nameRect.setTop(nameRect.top() + 5);

    auto statusRect = option.rect;
    statusRect.setLeft(statusRect.left() + 55);
    statusRect.setTop(statusRect.top() + 25);

    constexpr int photoSize = 40;
    auto photoRect = option.rect;
    photoRect.setLeft(photoRect.left() + 5);
    photoRect.setTop(photoRect.top() + 5);
    photoRect.setRight(photoRect.left() + photoSize);
    photoRect.setBottom(photoRect.top() + photoSize);

    constexpr int presenceRingSize = 16;
    auto presenceRingRect = option.rect;
    presenceRingRect.setLeft(presenceRingRect.left() + 32);
    presenceRingRect.setTop(presenceRingRect.top() + 32);
    presenceRingRect.setRight(presenceRingRect.left() + presenceRingSize);
    presenceRingRect.setBottom(presenceRingRect.top() + presenceRingSize);

    constexpr int presenceSize = 10;
    auto presenceRect = option.rect;
    presenceRect.setLeft(presenceRect.left() + 35);
    presenceRect.setTop(presenceRect.top() + 35);
    presenceRect.setRight(presenceRect.left() + presenceSize);
    presenceRect.setBottom(presenceRect.top() + presenceSize);

    Contact::Ptr contact = index.data(Qt::UserRole).value<Contact::Ptr>();
    ContactResource::Ptr resource = contact->mainResource();

    painter->setFont(nameFont);
    painter->drawText(nameRect, contact->name());

    painter->setFont(statusFont);
    if (resource.isNull()) {
        painter->drawText(statusRect, "Offline");
    } else {
        painter->drawText(statusRect, resource->presenceText());
    }

    QPainterPath path;
    path.addEllipse(photoRect);
    painter->setClipPath(path);

    QPixmap photo = contact->photo();
    photo = photo.scaledToHeight(photoSize, Qt::SmoothTransformation);
    painter->drawPixmap(photoRect, photo);

    painter->setClipping(false);

    if (!resource.isNull()) {
        QPixmap presenceIcon;
        switch (resource->presenceType()) {
            case Jreen::Presence::Available:
                presenceIcon = QPixmap(":/presences/available.png");
                break;

            case Jreen::Presence::Chat:
                presenceIcon = QPixmap(":/presences/chat.png");
                break;

            case Jreen::Presence::Away:
                presenceIcon = QPixmap(":/presences/away.png");
                break;

            case Jreen::Presence::DND:
                presenceIcon = QPixmap(":/presences/dnd.png");
                break;

            case Jreen::Presence::XA:
                presenceIcon = QPixmap(":/presences/xa.png");
                break;

            case Jreen::Presence::Unavailable:
            default:
                break;
        }

        if (!presenceIcon.isNull()) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush("white"));
            painter->drawEllipse(presenceRingRect);

            presenceIcon = presenceIcon.scaledToHeight(presenceSize, Qt::SmoothTransformation);
            painter->drawPixmap(presenceRect, presenceIcon);
        }
    }

    painter->restore();
}

QSize ContactListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(50);
    return size;
}
