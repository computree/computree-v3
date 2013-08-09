#ifndef WIDGETEXCLUDEVALUE_H
#define WIDGETEXCLUDEVALUE_H

#include "parameters/widgets/abstractparameterwidget.h"

namespace Ui {
    class WidgetExcludeValue;
}

class WidgetExcludeValue : public AbstractParameterWidget
{
    Q_OBJECT

public:

    explicit WidgetExcludeValue(AbstractParameter* model, QWidget *parent = 0);
    ~WidgetExcludeValue();

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
    Ui::WidgetExcludeValue *ui;
};

#endif // WIDGETEXCLUDEVALUE_H
