#ifndef WIDGETBOOL_H
#define WIDGETBOOL_H

#include "parameters/widgets/abstractparameterwidget.h"

namespace Ui {
    class WidgetBool;
}

class WidgetBool : public AbstractParameterWidget
{
    Q_OBJECT

public:

    explicit WidgetBool(AbstractParameter* model, QWidget *parent = 0);
    ~WidgetBool();

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
    Ui::WidgetBool *ui;
};

#endif // WIDGETBOOL_H
