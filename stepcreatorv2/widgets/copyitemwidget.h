#ifndef INITEMWIDGET_H
#define INITEMWIDGET_H


#include "widgets/abstractcopywidget.h"

namespace Ui {
    class COPYItemWidget;
}

class COPYItemWidget : public AbstractCopyWidget
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


    explicit COPYItemWidget(AbstractCopyModel* model, QWidget *parent = 0);
    ~COPYItemWidget();

    bool isvalid();
    QString getItemType();
    QString getAlias();
    QString getDisplayableName();
    QString getDescription();

private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::COPYItemWidget *ui;
};

#endif // INITEMWIDGET_H
