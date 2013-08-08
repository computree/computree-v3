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
    QString getName();

private:
    Ui::WidgetBool *ui;
};

#endif // WIDGETBOOL_H
