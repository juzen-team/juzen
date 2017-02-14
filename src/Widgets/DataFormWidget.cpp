#include "Widgets/DataFormWidget.h"
#include "Widgets/DataFormFieldWidget.h"

#include <QtCore/QUrl>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QBoxLayout>

#include <QtCore/QDebug>

DataFormWidget::DataFormWidget(const Jreen::DataForm::Ptr &form, QWidget *parent) :
        DataFormWidget(form, QList<Jreen::BitsOfBinary::Ptr>(), parent)
{
}

DataFormWidget::DataFormWidget(const Jreen::DataForm::Ptr &form, const QList<Jreen::BitsOfBinary::Ptr> &bobs, QWidget *parent) :
        QWidget(parent), form(form)
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

Jreen::DataForm::Ptr DataFormWidget::getDataForm()
{
    form->setType(Jreen::DataForm::Submit);
    return form;
}
