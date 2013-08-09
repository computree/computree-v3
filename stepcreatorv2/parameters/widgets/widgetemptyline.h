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
    QString getBeforeText();
    QString getAfterText();
    QString getCheckboxText();
    bool getDefaultValue();
    QString getDescription();

private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::WidgetEmptyLine *ui;
};

#endif // WIDGETEMPTYLINE_H
