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
    double getMin();
    double getMax();
    double getDefaultValue();
    QString getDescription();

private slots:
    void on_alias_textChanged(const QString &arg1);

    void on_min_valueChanged(int arg1);

    void on_max_valueChanged(int arg1);

private:
    Ui::WidgetInt *ui;
};

#endif // WIDGETINT_Hs
