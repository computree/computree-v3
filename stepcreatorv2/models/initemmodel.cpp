#include "initemmodel.h"
#include "widgets/initemwidget.h"

INItemModel::INItemModel() : AbstractModel()
{
    _widget = new INItemWidget(this);
    setText(getName());
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
