#include "abtractmodel.h"
#include "abstractwidget.h"

AbtractModel::AbtractModel() : QStandardItem()
{
}

QString AbtractModel::getDef()
{
    return QString("DEF_%1").arg(getName());
}

bool AbtractModel::isValid()
{
    for (int i = 0 ; i < rowCount() ; i++)
    {
        AbtractModel* item = (AbtractModel*) child(i);
        if (!item->isValid()) {return false;}
    }
    return getWidget()->isvalid();
}


void AbtractModel::getChildrenInModelsDefinitions(QString &result)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbtractModel* item = (AbtractModel*) child(i);
        result.append("\n");
        result.append("\n");
        result.append(item->getInModelsDefinition());
    }
}

void AbtractModel::getChildrenInComputeContent(QString &result)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbtractModel* item = (AbtractModel*) child(i);
        result.append("\n");
        result.append("\n");
        result.append(item->getInComputeContent());
    }
}

void AbtractModel::onAliasChange()
{
    setText(getName());
}
