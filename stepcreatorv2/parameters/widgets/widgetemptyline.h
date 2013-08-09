#ifndef WIDGETEMPTYLINE_H
#define WIDGETEMPTYLINE_H

#include "parameters/widgets/abstractparameterwidget.h"

namespace Ui {
    class WidgetEmptyLine;
}

class WidgetEmptyLine : public AbstractParameterWidget
{
    Q_OBJECT

public:

    explicit WidgetEmptyLine(AbstractParameter* model, QWidget *parent = 0);
    ~WidgetEmptyLine();

    bool isvalid();
    QString getAlias();

private:
    Ui::WidgetEmptyLine *ui;
};

#endif // WIDGETEMPTYLINE_H
