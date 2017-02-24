#include "Widgets/DataFormFieldWidget.h"

#include <QtCore/QUrl>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

DataFormFieldWidget::DataFormFieldWidget(const Jreen::DataFormField &field, const QMapBobs &bobsMap, QWidget *parent) :
        QWidget(parent), m_field(field), m_bobsMap(bobsMap)
{
    if (field.type() == Jreen::DataFormField::Hidden) {
        return;
    }

    QVariant value;
    switch (field.type()) {
        case Jreen::DataFormField::Boolean:
            value = field.cast<Jreen::DataFormFieldBoolean>().value();
            break;

        case Jreen::DataFormField::ListMulti:
            value = field.cast<Jreen::DataFormFieldListMulti>().values();
            break;

        default:
            value = field.value();
    }

    auto layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    if (!fieldIsLink() && !field.label().isEmpty()) {
        QString label = field.label();
        if (field.isRequired()) {
            label += " [required]";
        }
        layout->addWidget(new QLabel(label, this));
    }
    switch (field.type()) {
        case Jreen::DataFormField::Boolean:
            Q_ASSERT(!"unsupported field");
            break;

        case Jreen::DataFormField::Fixed:
            if (!value.toString().isEmpty()) {
                layout->addWidget(new QLabel(value.toString(), this));
            }
            break;

        case Jreen::DataFormField::JidMulti:
        case Jreen::DataFormField::JidSingle:
        case Jreen::DataFormField::ListMulti:
        case Jreen::DataFormField::ListSingle:
            Q_ASSERT(!"unsupported field");
            break;

        case Jreen::DataFormField::TextMulti:
        case Jreen::DataFormField::TextPrivate:
        case Jreen::DataFormField::TextSingle:
        case Jreen::DataFormField::None:
            generateText(value, layout);

        default:
            break;
    }

    setLayout(layout);
}

DataFormFieldWidget::~DataFormFieldWidget()
{
}

bool DataFormFieldWidget::fieldIsLink() const
{
    return (m_field.type() == Jreen::DataFormField::TextSingle || m_field.type() == Jreen::DataFormField::None)
           && m_field.var() == "url";
}

void DataFormFieldWidget::generateText(const QVariant &value, QVBoxLayout *layout)
{
    if (fieldIsLink()) {
        auto label = new QLabel(QString("<a href=\"%1\">%2</a>").arg(value.toString(), m_field.label()), this);
        label->setTextInteractionFlags(Qt::TextBrowserInteraction);
        label->setOpenExternalLinks(true);
        layout->addWidget(label);

        return;
    }

    generateMedia(layout);

    auto lineEdit = new QLineEdit(value.toString(), this);
    connect(lineEdit, &QLineEdit::textChanged, [this](const QString &text)
    {
        m_field.setValue(text);
    });
    layout->addWidget(lineEdit);
}

void DataFormFieldWidget::generateMedia(QVBoxLayout *layout)
{
    if (!m_field.media()) {
        return;
    }

    Jreen::DataFormMedia::Ptr media = m_field.media();
    QByteArray data;
    QString type;

    for (auto &uri : media->uries()) {
        const QUrl url = uri.url();
        if (url.scheme() == "cid") {
            data = m_bobsMap.value(uri.url().toString(QUrl::RemoveScheme));
            if (!data.isEmpty()) {
                type = uri.type();
                break;
            }
        }
    }

    if (!data.isEmpty() && type.startsWith(QLatin1String("image/"))) {
        QPixmap pixmap;
        pixmap.loadFromData(data, type.toLatin1().data());

        auto label = new QLabel(this);
        label->setPixmap(pixmap);

        layout->addWidget(label);
    }
}
