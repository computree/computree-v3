#include "widgets/abstractcopywidget.h"

AbstractCopyWidget::AbstractCopyWidget(AbstractCopyModel *model, QWidget *parent) : QWidget(parent)
{
    _model = model;
    setFocusPolicy(Qt::StrongFocus);
}
