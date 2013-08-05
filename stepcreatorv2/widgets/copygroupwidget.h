#ifndef INGROUPWIDGET_H
#define INGROUPWIDGET_H


#include "widgets/abstractcopywidget.h"

namespace Ui {
    class COPYGroupWidget;
}

class COPYGroupWidget : public AbstractCopyWidget
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

    explicit COPYGroupWidget(AbstractCopyModel* model, QWidget *parent = 0);
    explicit COPYGroupWidget(AbstractCopyModel* model, QString alias, QString name, QString desc, QWidget *parent = 0);
    ~COPYGroupWidget();

    bool isvalid();
    QString getAlias();
    QString getDisplayableName();
    QString getDescription();


private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::COPYGroupWidget *ui;
};

#endif // INRESULTWIDGET_H
