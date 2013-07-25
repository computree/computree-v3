#ifndef INGROUPWIDGET_H
#define INGROUPWIDGET_H


#include "widgets/abstractoutwidget.h"

namespace Ui {
    class OUTGroupWidget;
}

class OUTGroupWidget : public AbstractOutWidget
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

    explicit OUTGroupWidget(AbstractOutModel* model, QWidget *parent = 0);
    ~OUTGroupWidget();

    bool isvalid();
    QString getAlias();
    QString getDisplayableName();
    QString getDescription();

    OUTGroupWidget::GroupType getResultType();
    OUTGroupWidget::FinderMode getFinderMode();
    OUTGroupWidget::ChoiceMode getChoiceMode();



private slots:
    void on_rb_standard_toggled(bool checked);

    void on_alias_textChanged(const QString &arg1);

private:
    Ui::OUTGroupWidget *ui;
};

#endif // INRESULTWIDGET_H
