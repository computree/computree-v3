#include "widgets/abstractwidget.h"

AbstractWidget::AbstractWidget(AbstractModel *model, QWidget *parent) : QWidget(parent)
{
    _model = model;
}
