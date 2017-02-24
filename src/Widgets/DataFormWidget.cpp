#include "Widgets/DataFormWidget.h"
#include "Widgets/DataFormFieldWidget.h"

#include <QtCore/QUrl>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QBoxLayout>

#include <QtCore/QDebug>

DataFormWidget::DataFormWidget(const Jreen::DataForm::Ptr &form, QWidget *parent) :
    DataFormWidget(form, QList<Jreen::BitsOfBinary::Ptr>(), nullptr, parent)
{
}

DataFormWidget::DataFormWidget(const Jreen::DataForm::Ptr &form, DataFormWidget *other, QWidget *parent) :
    DataFormWidget(form, QList<Jreen::BitsOfBinary::Ptr>(), other, parent)
{
}

DataFormWidget::DataFormWidget(const Jreen::DataForm::Ptr &form, const QList<Jreen::BitsOfBinary::Ptr> &bobs, QWidget *parent) :
    DataFormWidget(form, bobs, nullptr, parent)
{
}

DataFormWidget::DataFormWidget(const Jreen::DataForm::Ptr &form, const QList<Jreen::BitsOfBinary::Ptr> &bobs, DataFormWidget *other, QWidget *parent) :
        QWidget(parent), m_form(form)
{
    DataFormFieldWidget::QMapBobs bobsMap;
    for (const Jreen::BitsOfBinary::Ptr &bob : bobs) {
        bobsMap.insert(bob->cid().toString(), bob->data());
    }

    auto scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto innerWidget = new QWidget(scrollArea);
    auto innerLayout = new QVBoxLayout(innerWidget);
    innerLayout->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < form->fieldsCount(); i++) {
        Jreen::DataFormField field = form->field(i);
        if (other) {
            if (field.type() != Jreen::DataFormField::Invalid &&
                field.type() != Jreen::DataFormField::Fixed &&
                field.type() != Jreen::DataFormField::Hidden) {
                field.setValues(other->dataForm()->field(i).values());
            }
        }
        innerLayout->addWidget(new DataFormFieldWidget(field, bobsMap, innerWidget));
    }

    innerWidget->setLayout(innerLayout);
    scrollArea->setWidget(innerWidget);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(scrollArea);
    setLayout(layout);
}

DataFormWidget::~DataFormWidget()
{
}

bool DataFormWidget::allRequiredFieldsFilled() const
{
    bool ok = true;
    for (int i = 0; i < m_form->fieldsCount(); i++) {
        Jreen::DataFormField field = m_form->field(i);
        if (field.isRequired() && field.value().isEmpty()) {
            ok = false;
            break;
        }
    }
    return ok;
}

Jreen::DataForm::Ptr DataFormWidget::dataForm()
{
    m_form->setType(Jreen::DataForm::Submit);
    return m_form;
}
