#ifndef INRESULTWIDGET_H
#define INRESULTWIDGET_H

#include "widgets/abstractwidget.h"

namespace Ui {
    class INResultWidget;
}

class INResultWidget : public AbstractWidget
{
    Q_OBJECT

public:

    enum ResultType
    {
        R_StandardResult,
        R_CopyResult
    };

    explicit INResultWidget(QWidget *parent = 0);
    ~INResultWidget();

    bool isvalid();
    QString getAlias();
    QString getDisplayableName();
    QString getDescription();

    INResultWidget::ResultType getResultType();

private:
    Ui::INResultWidget *ui;
};

#endif // INRESULTWIDGET_H
