#ifndef JUZEN_DATAFORMFIELDWIDGET_H
#define JUZEN_DATAFORMFIELDWIDGET_H

#include <jreen/bitsofbinary.h>
#include <jreen/dataform.h>
#include <QtWidgets/QWidget>

class QVBoxLayout;
class DataFormFieldWidget : public QWidget
{
    Q_OBJECT

public:
    using QMapBobs = QMap<QString, QByteArray>;

    DataFormFieldWidget(const Jreen::DataFormField &field, const QMapBobs &bobsMap, QWidget *parent = Q_NULLPTR);
    ~DataFormFieldWidget();

private:
    bool fieldIsLink() const;
    void generateText(const QVariant &value, QVBoxLayout *layout);
    void generateMedia(QVBoxLayout *layout);

    Jreen::DataFormField field;
    QMapBobs bobsMap;
};

#endif //JUZEN_DATAFORMFIELDWIDGET_H
