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
    explicit INGroupWidget(QWidget *parent = 0);
    ~INGroupWidget();

private slots:
    void on_rb_standard_toggled(bool checked);

private:
    Ui::INGroupWidget *ui;
};

#endif // INRESULTWIDGET_H
