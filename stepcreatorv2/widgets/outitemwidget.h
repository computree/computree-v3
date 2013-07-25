#ifndef INITEMWIDGET_H
#define INITEMWIDGET_H


#include "widgets/abstractoutwidget.h"

namespace Ui {
    class OUTItemWidget;
}

class OUTItemWidget : public AbstractOutWidget
{
    Q_OBJECT

public:

    enum ChoiceMode
    {
        C_OneIfMultiple,
        C_MultipleIfMultiple
    };

    enum FinderMode
    {
        F_Obligatory,
        F_Optional
    };


    explicit OUTItemWidget(AbstractOutModel* model, QWidget *parent = 0);
    ~OUTItemWidget();

    bool isvalid();
    QString getItemType();
    QString getAlias();
    QString getDisplayableName();
    QString getDescription();

    OUTItemWidget::FinderMode getFinderMode();
    OUTItemWidget::ChoiceMode getChoiceMode();

private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::OUTItemWidget *ui;
};

#endif // INITEMWIDGET_H
