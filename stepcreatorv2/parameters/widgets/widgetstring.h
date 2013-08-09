#ifndef WIDGETSTRING_H
#define WIDGETSTRING_H

#include "parameters/widgets/abstractparameterwidget.h"

namespace Ui {
    class WidgetString;
}

class WidgetString : public AbstractParameterWidget
{
    Q_OBJECT

public:

    explicit WidgetString(AbstractParameter* model, QWidget *parent = 0);
    ~WidgetString();

    bool isvalid();
    QString getAlias();
    QString getBeforeText();
    QString getAfterText();
    QString getDefaultValue();
    QString getDescription();

private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::WidgetString *ui;
};

#endif // WIDGETSTRING_H
