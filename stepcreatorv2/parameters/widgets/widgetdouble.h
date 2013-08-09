#ifndef WIDGETDOUBLE_H
#define WIDGETDOUBLE_H

#include "parameters/widgets/abstractparameterwidget.h"

namespace Ui {
    class WidgetDouble;
}

class WidgetDouble : public AbstractParameterWidget
{
    Q_OBJECT

public:

    explicit WidgetDouble(AbstractParameter* model, QWidget *parent = 0);
    ~WidgetDouble();

    bool isvalid();
    QString getAlias();
    QString getBeforeText();
    QString getAfterText();
    QString getCheckboxText();
    bool getDefaultValue();
    QString getDescription();

private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::WidgetDouble *ui;
};

#endif // WIDGETDOUBLE_H
