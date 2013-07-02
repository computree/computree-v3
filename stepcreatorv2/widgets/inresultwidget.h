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
    explicit INResultWidget(QWidget *parent = 0);
    ~INResultWidget();

private:
    Ui::INResultWidget *ui;
};

#endif // INRESULTWIDGET_H
