#include "models/abstractcopymodel.h"

AbstractCopyModel::AbstractCopyModel() : QStandardItem()
{
    _status = AbstractCopyModel::S_Copy;
}

AbstractCopyModel::~AbstractCopyModel()
{
    delete _widget;
}
AbstractCopyWidget* AbstractCopyModel::getWidget()
{
    return _widget;
}


QString AbstractCopyModel::getDef()
{
    return QString("DEF_%1").arg(getName());
}

QString AbstractCopyModel::getAlias()
{
    return _widget->getAlias();
}

bool AbstractCopyModel::isValid()
{
    for (int i = 0 ; i < rowCount() ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        if (!item->isValid()) {return false;}
    }
    return getWidget()->isvalid();
}

QString AbstractCopyModel::getCopyModelsDefines()
{
    QString result = "#define ";
    result.append(getDef());
    result.append(QString(" \"%1\"\n").arg(getAlias()));

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        result.append(item->getCopyModelsDefines());
    }
    return result;
}


// static
QString AbstractCopyModel::getQStringListConcat(QSet<QString> &list)
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


void AbstractCopyModel::getChildrenCopyModelsDefinitions(QString &result)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        result.append("\n");
        result.append(item->getCopyModelsDefinition());
    }
}

void AbstractCopyModel::getChildrenCopyModelsHierachy(QString &result)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        result.append(item->getCopyModelsHierachy());
    }
}

void AbstractCopyModel::getChildrenCopyComputeBeginning(QString &result, QString resultName)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        result.append(item->getCopyComputeBeginning(i, resultName));
    }
}

void AbstractCopyModel::onAliasChange()
{
    setText(getName());
}
