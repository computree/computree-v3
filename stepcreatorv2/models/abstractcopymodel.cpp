#include "models/abstractcopymodel.h"
#include "tools.h"

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

QString AbstractCopyModel::getDisplayableName()
{
    return _widget->getDisplayableName();
}

QString AbstractCopyModel::getDef()
{
    return QString("DEF_%1").arg(getName());
}

QString AbstractCopyModel::getAlias()
{
    return _widget->getAlias();
}

QString AbstractCopyModel::getAutoRenameName()
{
    QString str = "_" + getName() + "_ModelName";
    return str;
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

bool AbstractCopyModel::copyIncludesNeeded()
{
    if (_status == AbstractCopyModel::S_Added) {return true;}

    for (int i = 0 ; i < rowCount() ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        if (item->copyIncludesNeeded()) {return true;}
    }

    return false;
}

QString AbstractCopyModel::getAutoRenamesDeclarations()
{
    QString result = "";

    if (getStatus()==AbstractCopyModel::S_Added)
    {
        result += Tools::getIndentation(1) + "CT_AutoRenameModels" + Tools::getIndentation(1) + getAutoRenameName() + ";\n";
    }

    int count = rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        result += item->getAutoRenamesDeclarations();
    }
    return result;
}


void AbstractCopyModel::setDeleted()
{
    _status = AbstractCopyModel::S_DeletedCopy;
    setData(QVariant(QColor(Qt::red)),Qt::ForegroundRole);
}

void AbstractCopyModel::setNotDeleted()
{
    _status = AbstractCopyModel::S_Copy;
    setData(QVariant(QColor(Qt::black)),Qt::ForegroundRole);
}


void AbstractCopyModel::onAliasChange()
{
    setText(getName());
}
