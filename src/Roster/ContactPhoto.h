#ifndef JUZEN_CONTACTPHOTO_H
#define JUZEN_CONTACTPHOTO_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

class Contact;
class ContactPhoto : public QObject
{
    Q_OBJECT

public:
    ContactPhoto(Contact *contact);
    virtual ~ContactPhoto();
    
    QPixmap photo() const;
    void generatePhoto();
    bool setPhoto(const QString &hash);
    void setPhoto(const QByteArray &data, const QString &type);

private:
    QPixmap generateNoPhotoFiller(const QString &name) const;

    Contact *m_contact;
    QPixmap m_photo;
};

#endif //JUZEN_CONTACTPHOTO_H
