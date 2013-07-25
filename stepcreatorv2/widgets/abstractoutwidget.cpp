#include "widgets/abstractoutwidget.h"

AbstractOutWidget::AbstractOutWidget(AbstractOutModel *model, QWidget *parent) : QWidget(parent)
{
    _model = model;
    setFocusPolicy(Qt::StrongFocus);
}
