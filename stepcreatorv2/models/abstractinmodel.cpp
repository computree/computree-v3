#include "models/abstractinmodel.h"

AbstractInModel::AbstractInModel() : QStandardItem()
{
}

AbstractInModel::~AbstractInModel()
{
    delete _widget;
}
AbstractInWidget* AbstractInModel::getWidget()
{
    return _widget;
}


QString AbstractInModel::getDef()
{
    return QString("DEF_%1").arg(getName());
}

QString AbstractInModel::getAlias()
{
    return _widget->getAlias();
}

QString AbstractInModel::getDescription()
{
    return _widget->getDescription();
}

QString AbstractInModel::getDisplayableName()
{
    return _widget->getDisplayableName();
}


bool AbstractInModel::isValid()
{
    for (int i = 0 ; i < rowCount() ; i++)
    {
        AbstractInModel* item = (AbstractInModel*) child(i);
        if (!item->isValid()) {return false;}
    }
    return getWidget()->isvalid();
}

QString AbstractInModel::getInModelsDefines()
{
    QString result = "#define ";
    result.append(getDef());
    result.append(QString(" \"%1\"\n").arg(getAlias()));

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractInModel* item = (AbstractInModel*) child(i);
        result.append(item->getInModelsDefines());
    }
    return result;
}

void AbstractInModel::getChildrenInModelsDefinitions(QString &result)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractInModel* item = (AbstractInModel*) child(i);
        result.append("\n");
        result.append(item->getInModelsDefinition());
    }
}

void AbstractInModel::getChildrenInModelsHierachy(QString &result)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractInModel* item = (AbstractInModel*) child(i);
        result.append(item->getInModelsHierachy());
    }
}

void AbstractInModel::getChildrenInComputeBeginning(QString &result, QString resultDef, QString useCopy)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractInModel* item = (AbstractInModel*) child(i);
        result.append(item->getInComputeBeginning(resultDef, useCopy));
    }
}

void AbstractInModel::onAliasChange()
{
    setText(getName());
}
