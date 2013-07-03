#include "ingroupmodel.h"

INGroupModel::INGroupModel()
{
    _widget = new INGroupWidget();
    setText("Group");
}

INGroupModel::~INGroupModel()
{
    delete _widget;
}


QString INGroupModel::getName()
{
    return QString("group_%1").arg(getAlias());
}


QString INGroupModel::getInModelsDefinition()
{
    QString result = "Modèle Groupe";

    getChildrenInModelsDefinitions(result);
    return result;
}

QString INGroupModel::getInComputeContent()
{
    QString result = "Compute Groupe";

    getChildrenInComputeContent(result);
    return result;
}
