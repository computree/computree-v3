#ifndef INGROUPWIDGET_H
#define INGROUPWIDGET_H


#include "widgets/abstractwidget.h"

namespace Ui {
    class INGroupWidget;
}

class INGroupWidget : public AbstractWidget
{
    Q_OBJECT

public:
    enum GroupType
    {
        G_ZeroOrMore,
        G_OneOrMore,
        G_Sandard
    };

    enum ChoiceMode
    {
        C_OneIfMultiple,
        C_MultipleIfMultiple,
        C_DontChoose
    };

    enum FinderMode
    {
        F_Obligatory,
        F_Optional
    };

    explicit INGroupWidget(AbstractModel* model, QWidget *parent = 0);
    ~INGroupWidget();

    bool isvalid();
    QString getAlias();
    QString getDisplayableName();
    QString getDescription();

    INGroupWidget::GroupType getResultType();
    INGroupWidget::FinderMode getFinderMode();
    INGroupWidget::ChoiceMode getChoiceMode();



private slots:
    void on_rb_standard_toggled(bool checked);

    void on_alias_textChanged(const QString &arg1);

private:
    Ui::INGroupWidget *ui;
};

#endif // INRESULTWIDGET_H
