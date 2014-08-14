#include "ct_autorenamemodels.h"

#include "ct_model/abstract/ct_abstractmodel.h"

CT_AutoRenameModels::CT_AutoRenameModels()
{
    _name = "";
    _extra = -1;
}

CT_AutoRenameModels::~CT_AutoRenameModels()
{
    while(!_models.isEmpty())
        _models.takeFirst()->setAutoRenameObject(NULL);
}

QString CT_AutoRenameModels::name() const
{
    return _name;
}

int CT_AutoRenameModels::extra() const
{
    return _extra;
}

QString CT_AutoRenameModels::completeName() const
{
    if(_extra >= 0)
        return _name + QString().setNum(_extra);

    return name();
}

bool CT_AutoRenameModels::hasBeenModified() const
{
    return !_name.isEmpty();
}

void CT_AutoRenameModels::setName(QString name)
{
    _name = name;
}

void CT_AutoRenameModels::setExtra(int extra)
{
    _extra = extra;
}

bool CT_AutoRenameModels::renameAllModels()
{
    QString cName = completeName();

    QListIterator<CT_AbstractModel*> it(_models);

    while(it.hasNext())
        if(!it.next()->canSetUniqueName(cName))
            return false;

    it.toFront();

    while(it.hasNext())
        it.next()->setUniqueName(cName);

    return true;
}

void CT_AutoRenameModels::addModel(const CT_AbstractModel *model)
{
    _models.append((CT_AbstractModel*)model);
    ((CT_AbstractModel*)model)->setAutoRenameObject(this);
}

bool CT_AutoRenameModels::removeModel(CT_AbstractModel *model)
{
    bool ret = _models.removeOne(model);

    if(ret && _models.isEmpty())
        clear();

    return ret;
}

const QList<CT_AbstractModel*>& CT_AutoRenameModels::models() const
{
    return _models;
}

void CT_AutoRenameModels::clear()
{
    _models.clear();

    _name = "";
    _extra = -1;
}
