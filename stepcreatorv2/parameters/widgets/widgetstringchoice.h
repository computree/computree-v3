#ifndef WIDGETSTRINGCHOICE_H
#define WIDGETSTRINGCHOICE_H

#include "parameters/widgets/abstractparameterwidget.h"

namespace Ui {
    class WidgetStringChoice;
}

class WidgetStringChoice : public AbstractParameterWidget
{
    Q_OBJECT

public:

    explicit WidgetStringChoice(AbstractParameter* model, QWidget *parent = 0);
    ~WidgetStringChoice();

    bool isvalid();
    QString getAlias();
    QString getBeforeText();
    QString getAfterText();
    QString getDefaultValue();
    QStringList getPossibleValues();
    QString getDescription();

private slots:
    void on_alias_textChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::WidgetStringChoice *ui;
};

#endif // WIDGETSTRINGCHOICE_H
