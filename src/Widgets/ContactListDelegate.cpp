#include "Resources/IconFactory.h"
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

    auto defaultFont = painter->font();

    Contact::Ptr contact = index.data(Qt::UserRole).value<Contact::Ptr>();
    ContactResource::Ptr resource = contact->mainResource();

    // Name
    {
        auto font = defaultFont;
        font.setBold(true);

        auto rect = option.rect;
        rect.setLeft(rect.left() + 55);
        rect.setTop(rect.top() + 5);

        painter->setFont(font);
        painter->drawText(rect, contact->name());
    }

    // Status
    {
        auto font = defaultFont;
        font.setItalic(true);

        auto rect = option.rect;
        rect.setLeft(rect.left() + 55);
        rect.setTop(rect.top() + 25);

        painter->setFont(font);
        if (resource.isNull()) {
            painter->drawText(rect, ContactResource::presenceText(Jreen::Presence::Unavailable));
        } else {
            painter->drawText(rect, resource->presenceText());
        }
    }

    // Photo
    {
        constexpr int photoSize = 40;
        auto photoRect = option.rect;
        photoRect.setLeft(photoRect.left() + 5);
        photoRect.setTop(photoRect.top() + 5);
        photoRect.setRight(photoRect.left() + photoSize);
        photoRect.setBottom(photoRect.top() + photoSize);

        constexpr int moodRingSize = 16;
        auto moodRingRect = option.rect;
        moodRingRect.setLeft(moodRingRect.left() + 5);
        moodRingRect.setTop(moodRingRect.top() + 32);
        moodRingRect.setRight(moodRingRect.left() + moodRingSize);
        moodRingRect.setBottom(moodRingRect.top() + moodRingSize);

        constexpr int moodSize = 10;
        auto moodRect = option.rect;
        moodRect.setLeft(moodRect.left() + 8);
        moodRect.setTop(moodRect.top() + 35);
        moodRect.setRight(moodRect.left() + moodSize);
        moodRect.setBottom(moodRect.top() + moodSize);

        constexpr int activityRingSize = 16;
        auto activityRingRect = option.rect;
        activityRingRect.setLeft(activityRingRect.left() + 18);
        activityRingRect.setTop(activityRingRect.top() + 32);
        activityRingRect.setRight(activityRingRect.left() + activityRingSize);
        activityRingRect.setBottom(activityRingRect.top() + activityRingSize);

        constexpr int activitySize = 10;
        auto activityRect = option.rect;
        activityRect.setLeft(activityRect.left() + 21);
        activityRect.setTop(activityRect.top() + 35);
        activityRect.setRight(activityRect.left() + activitySize);
        activityRect.setBottom(activityRect.top() + activitySize);

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

        QPainterPath path;
        path.addEllipse(photoRect);
        if (resource && resource->isOnline()) {
            // Mood
            if (contact->mood()) {
                QPainterPath excludePath;
                excludePath.addEllipse(moodRingRect);
                path = path.subtracted(excludePath);
            }

            // Activity
            if (contact->activity()) {
                QPainterPath excludePath;
                excludePath.addEllipse(activityRingRect);
                path = path.subtracted(excludePath);
            }

            QPainterPath excludePath;
            excludePath.addEllipse(presenceRingRect);
            path = path.subtracted(excludePath);
        }
        painter->setClipPath(path);

        QPixmap photo = contact->photo();
        photo = photo.scaledToHeight(photoSize, Qt::SmoothTransformation);
        painter->drawPixmap(photoRect, photo);

        painter->setClipping(false);

        
        if (resource && resource->isOnline()) {
            // Mood
            if (contact->mood()) {
                auto moodIcon = IconFactory::moodIcon(contact->mood()->typeName());
                moodIcon = moodIcon.scaledToHeight(moodSize, Qt::SmoothTransformation);
                painter->drawPixmap(moodRect, moodIcon);
            }

            // Activity
            if (contact->activity()) {
                auto activityIcon = IconFactory::activityIcon(contact->activity()->generalName(), contact->activity()->specificName());
                activityIcon = activityIcon.scaledToHeight(activitySize, Qt::SmoothTransformation);
                painter->drawPixmap(activityRect, activityIcon);
            }

            auto presenceIcon = IconFactory::presenceIcon(resource->presenceType());
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
