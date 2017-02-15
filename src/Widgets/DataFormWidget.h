#ifndef JUZEN_DATAFORMWIDGET_H
#define JUZEN_DATAFORMWIDGET_H

#include <jreen/bitsofbinary.h>
#include <jreen/dataform.h>
#include <QtWidgets/QWidget>

class DataFormWidget : public QWidget
{
    Q_OBJECT

public:
    DataFormWidget(const Jreen::DataForm::Ptr &form, QWidget *parent = Q_NULLPTR);
    DataFormWidget(const Jreen::DataForm::Ptr &form, DataFormWidget *other, QWidget *parent = Q_NULLPTR);
    DataFormWidget(const Jreen::DataForm::Ptr &form, const QList<Jreen::BitsOfBinary::Ptr> &bobs, QWidget *parent = Q_NULLPTR);
    DataFormWidget(const Jreen::DataForm::Ptr &form, const QList<Jreen::BitsOfBinary::Ptr> &bobs, DataFormWidget *other, QWidget *parent = Q_NULLPTR);
    ~DataFormWidget();

    Jreen::DataForm::Ptr getDataForm();

private:
    Jreen::DataForm::Ptr form;
};

#endif //JUZEN_DATAFORMWIDGET_H
