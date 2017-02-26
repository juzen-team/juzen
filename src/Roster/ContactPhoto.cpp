#include "Account.h"
#include "Roster/ContactPhoto.h"
#include "Roster/Contact.h"
#include "Roster/Roster.h"
#include "System/AppInfo.h"
#include <QtCore/QBuffer>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#include <QtGui/QColor>
#include <QtGui/QPainter>

ContactPhoto::ContactPhoto(Contact *contact) : QObject(contact), m_contact(contact)
{
}

ContactPhoto::~ContactPhoto()
{
}

QPixmap ContactPhoto::photo() const
{
    return m_photo;
}

void ContactPhoto::generatePhoto()
{
    m_photo = generateNoPhotoFiller(m_contact->name());
}

bool ContactPhoto::setPhoto(const QString &hash)
{
    auto photoPath = QString("%1/photos/%2").arg(AppInfo::accountDataDir(m_contact->roster()->account()->client()->jid().bare()), hash);
    
    QPixmap photo;
    if (photo.load(photoPath)) {
        m_photo = photo;
        return true;
    }

    return false;
}

void ContactPhoto::setPhoto(const QByteArray &data, const QString &type)
{
    m_photo.loadFromData(data, type.toLatin1().data());

    auto photosDirpath = QString("%1/photos/").arg(AppInfo::accountDataDir(m_contact->roster()->account()->client()->jid().bare()));
    QDir photosDir;
    if (!photosDir.exists(photosDirpath)) {
        photosDir.mkpath(photosDirpath);
    }

    auto hash = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();
    m_photo.save(photosDirpath + hash, "PNG");
}

QPixmap ContactPhoto::generateNoPhotoFiller(const QString &name) const
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

    painter.drawText(rect, Qt::AlignCenter, QString(name[0]));

    painter.end();

    return replacementPhoto;
}
