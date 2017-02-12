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

    auto photoRect = option.rect;
    photoRect.setLeft(photoRect.left() + 5);
    photoRect.setTop(photoRect.top() + 5);
    photoRect.setRight(photoRect.left() + 40);
    photoRect.setBottom(photoRect.top() + 40);

    painter->setFont(nameFont);
    painter->drawText(nameRect, index.data(Qt::UserRole).toString());

    painter->setFont(statusFont);
    painter->drawText(statusRect, index.data(Qt::UserRole + 1).toString());

    QPainterPath path;
    path.addEllipse(photoRect);
    painter->setClipPath(path);

    QPixmap photo;
    photo.loadFromData(index.data(Qt::UserRole + 2).toByteArray(), index.data(Qt::UserRole + 3).toString().toLatin1().data());
    photo = photo.scaledToHeight(40, Qt::SmoothTransformation);
    painter->drawPixmap(photoRect, photo);

    painter->restore();
}

QSize ContactListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(50);
    return size;
}
