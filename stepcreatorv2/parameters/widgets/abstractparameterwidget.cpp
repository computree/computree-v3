#include "abstractparameterwidget.h"

AbstractParameterWidget::AbstractParameterWidget(AbstractParameter *model, QWidget *parent) : QWidget(parent)
{
    _model = model;
    setFocusPolicy(Qt::StrongFocus);
}
