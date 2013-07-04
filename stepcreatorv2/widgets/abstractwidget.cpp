#include "widgets/abstractwidget.h"
#include "models/abtractmodel.h"

AbstractWidget::AbstractWidget(AbstractModel *model, QWidget *parent) : QWidget(parent)
{
    connect(this, SIGNAL(aliasChanged()), model, SLOT(onAliasChange()));
}
