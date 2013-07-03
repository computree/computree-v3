#include "initemmodel.h"

INItemModel::INItemModel()
{
    _widget = new INItemWidget();
    setText("Item");
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
    QString result = "Modèle Item";

    getChildrenInModelsDefinitions(result);
    return result;
}

QString INItemModel::getInComputeContent()
{
    QString result = "Compute Item";

    getChildrenInComputeContent(result);
    return result;
}
