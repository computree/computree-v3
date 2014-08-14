#ifndef CT_COMBOBOX_H
#define CT_COMBOBOX_H

#include "ct_widgetwithvaluereferenceinterface.h"

#include <QComboBox>

class PLUGINSHAREDSHARED_EXPORT CT_ComboBox : public CT_WidgetWithValueReferenceInterface
{
private:

    struct ComboBoxData
    {
        QStringList  _valuesList;
        QString     *_value;
    };

public:
    CT_ComboBox(QStringList valuesList, QString &value, QString description = "");

    QString type() const;
    QList<SettingsNodeGroup*> getAllValues() const;
    bool setAllValues(const QList<SettingsNodeGroup*> &list);

    QWidget* createWidget(QWidget &parent);

    void updateValue();

    bool isValueAndWidgetValueDifferent() const;

    QVariant getValue() const;

    bool setWidgetValue(QVariant val);

protected:

    ComboBoxData   _data;
    QComboBox      *_comboBoxCreated;
    QString         _description;
};

#endif // CT_COMBOBOX_H
