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
    double getMin();
    double getMax();
    int getDec();
    double getMult();
    double getDefaultValue();
    QString getDescription();

private slots:
    void on_alias_textChanged(const QString &arg1);

    void on_nbDec_valueChanged(int arg1);

    void on_min_valueChanged(double arg1);

    void on_max_valueChanged(double arg1);

private:
    Ui::WidgetDouble *ui;
};

#endif // WIDGETDOUBLE_H
