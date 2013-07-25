#ifndef INRESULTWIDGET_H
#define INRESULTWIDGET_H

#include "widgets/abstractoutwidget.h"

namespace Ui {
    class OUTResultWidget;
}

class OUTResultWidget : public AbstractOutWidget
{
    Q_OBJECT

public:

    enum ResultType
    {
        R_StandardResult,
        R_CopyResult
    };

    explicit OUTResultWidget(AbstractOutModel* model, QWidget *parent = 0);
    ~OUTResultWidget();

    bool isvalid();
    QString getAlias();
    QString getDisplayableName();
    QString getDescription();
    bool getRecursive();

    OUTResultWidget::ResultType getResultType();

private slots:
    void on_alias_textChanged(const QString &arg1);

private:
    Ui::OUTResultWidget *ui;
};

#endif // INRESULTWIDGET_H
