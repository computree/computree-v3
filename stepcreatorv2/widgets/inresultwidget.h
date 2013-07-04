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

    explicit INResultWidget(AbstractModel* model, QWidget *parent = 0);
    ~INResultWidget();

    bool isvalid();
    QString getAlias();
    QString getDisplayableName();
    QString getDescription();
    bool getRecursive();

    INResultWidget::ResultType getResultType();

private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::INResultWidget *ui;
};

#endif // INRESULTWIDGET_H
