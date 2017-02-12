#ifndef JUZEN_CONTACTLISTDELEGATE_H
#define JUZEN_CONTACTLISTDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class ContactListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ContactListDelegate(QObject *parent = Q_NULLPTR);
    ~ContactListDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif //JUZEN_CONTACTLISTDELEGATE_H
