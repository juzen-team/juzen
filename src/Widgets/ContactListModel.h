#ifndef JUZEN_CONTACTLISTMODEL_H
#define JUZEN_CONTACTLISTMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QVector>

class ContactListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ContactListModel(QObject *parent = Q_NULLPTR);
    ~ContactListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::UserRole) const override;

    void add(const QString &name, const QString &status, const QByteArray &photo, const QString &mimeType);

private:
    struct Contact {
        QString name;
        QString status;
        QByteArray photo;
        QString mimeType;
    };

    QVector<Contact> items;
};

#endif //JUZEN_CONTACTLISTMODEL_H
