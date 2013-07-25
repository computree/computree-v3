#ifndef INRESULTWIDGET_H
#define INRESULTWIDGET_H

#include "widgets/abstractcopywidget.h"

namespace Ui {
    class COPYResultWidget;
}

class COPYResultWidget : public AbstractCopyWidget
{
    Q_OBJECT

public:

    explicit COPYResultWidget(AbstractCopyModel* model, QWidget *parent = 0);
    ~COPYResultWidget();

    bool isvalid();
    QString getAlias();
    QString getDisplayableName();
    QString getDescription();

private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::COPYResultWidget *ui;
};

#endif // INRESULTWIDGET_H
