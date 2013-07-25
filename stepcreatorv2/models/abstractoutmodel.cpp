#include "models/abstractoutmodel.h"

AbstractOutModel::AbstractOutModel() : QStandardItem()
{
}

AbstractOutModel::~AbstractOutModel()
{
    delete _widget;
}
AbstractOutWidget* AbstractOutModel::getWidget()
{
    return _widget;
}


QString AbstractOutModel::getDef()
{
    return QString("DEF_%1").arg(getName());
}

QString AbstractOutModel::getAlias()
{
    return _widget->getAlias();
}

bool AbstractOutModel::isValid()
{
    for (int i = 0 ; i < rowCount() ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) child(i);
        if (!item->isValid()) {return false;}
    }
    return getWidget()->isvalid();
}

QString AbstractOutModel::getOutModelsDefines()
{
    QString result = "#define ";
    result.append(getDef());
    result.append(QString(" \"%1\"\n").arg(getAlias()));

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) child(i);
        result.append(item->getOutModelsDefines());
    }
    return result;
}


// static
QString AbstractOutModel::getQStringListConcat(QSet<QString> &list)
{
    QStringList list2(list.toList());
    list2.sort();

    QString result = "";

    foreach (const QString &value, list2)
    {
        result += value;
        result += "\n";
    }
    return result;
}


void AbstractOutModel::getChildrenOutModelsDefinitions(QString &result)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) child(i);
        result.append("\n");
        result.append(item->getOutModelsDefinition());
    }
}

void AbstractOutModel::getChildrenOutModelsHierachy(QString &result)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) child(i);
        result.append(item->getOutModelsHierachy());
    }
}

void AbstractOutModel::getChildrenOutComputeBeginning(QString &result, QString resultDef, QString useCopy)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) child(i);
        result.append(item->getOutComputeBeginning(resultDef, useCopy));
    }
}

void AbstractOutModel::onAliasChange()
{
    setText(getName());
}
