#include "models/abstractmodel.h"

AbstractModel::AbstractModel() : QStandardItem()
{
}

AbstractModel::~AbstractModel()
{
    delete _widget;
}
AbstractWidget* AbstractModel::getWidget()
{
    return _widget;
}


QString AbstractModel::getDef()
{
    return QString("DEF_%1").arg(getName());
}

QString AbstractModel::getAlias()
{
    return _widget->getAlias();
}

bool AbstractModel::isValid()
{
    for (int i = 0 ; i < rowCount() ; i++)
    {
        AbstractModel* item = (AbstractModel*) child(i);
        if (!item->isValid()) {return false;}
    }
    return getWidget()->isvalid();
}


void AbstractModel::getChildrenInModelsDefinitions(QString &result)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractModel* item = (AbstractModel*) child(i);
        result.append("\n");
        result.append("\n");
        result.append(item->getInModelsDefinition());
    }
}

void AbstractModel::getChildrenInComputeContent(QString &result)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractModel* item = (AbstractModel*) child(i);
        result.append("\n");
        result.append("\n");
        result.append(item->getInComputeContent());
    }
}

void AbstractModel::onAliasChange()
{
    setText(getName());
}
