#ifndef WIDGETINT_H
#define WIDGETINT_H

#include "parameters/widgets/abstractparameterwidget.h"

namespace Ui {
    class WidgetInt;
}

class WidgetInt : public AbstractParameterWidget
{
    Q_OBJECT

public:

    explicit WidgetInt(AbstractParameter* model, QWidget *parent = 0);
    ~WidgetInt();

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
    Ui::WidgetInt *ui;
};

#endif // WIDGETINT_H
