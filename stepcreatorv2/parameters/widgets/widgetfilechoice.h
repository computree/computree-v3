#ifndef WIDGETFILECHOICE_H
#define WIDGETFILECHOICE_H

#include "parameters/widgets/abstractparameterwidget.h"

namespace Ui {
    class WidgetFileChoice;
}

class WidgetFileChoice : public AbstractParameterWidget
{
    Q_OBJECT

public:

    explicit WidgetFileChoice(AbstractParameter* model, QWidget *parent = 0);
    ~WidgetFileChoice();

    bool isvalid();
    QString getAlias();
    QString getBeforeText();
    QString getNeededFileType();
    QString getAfterText();
    QString getDescription();

private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::WidgetFileChoice *ui;
};

#endif // WIDGETFILECHOICE_H
