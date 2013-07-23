#include "widgets/abstractinwidget.h"

AbstractInWidget::AbstractInWidget(AbstractInModel *model, QWidget *parent) : QWidget(parent)
{
    _model = model;
    setFocusPolicy(Qt::StrongFocus);
}
