#include "initemmodel.h"

INItemModel::INItemModel() : AbstractModel()
{
    _widget = new INItemWidget(this);
    setText(getName());
}

INItemModel::~INItemModel()
{
    delete _widget;
}

QString INItemModel::getName()
{
    return QString("item_%1").arg(getAlias());
}

QString INItemModel::getInModelsDefinition()
{
    QString result = "Mod�le Item";

    getChildrenInModelsDefinitions(result);
    return result;
}

QString INItemModel::getInComputeContent()
{
    QString result = "Compute Item";

    getChildrenInComputeContent(result);
    return result;
}
