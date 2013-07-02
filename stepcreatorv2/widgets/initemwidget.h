#ifndef INITEMWIDGET_H
#define INITEMWIDGET_H


#include "widgets/abstractwidget.h"

namespace Ui {
    class INItemWidget;
}

class INItemWidget : public AbstractWidget
{
    Q_OBJECT

public:
    explicit INItemWidget(QWidget *parent = 0);
    ~INItemWidget();

private:
    Ui::INItemWidget *ui;
};

#endif // INITEMWIDGET_H
