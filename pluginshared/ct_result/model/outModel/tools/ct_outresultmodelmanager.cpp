#include "ct_outresultmodelmanager.h"

CT_OutResultModelManager::CT_OutResultModelManager()
{
}

CT_OutResultModelManager::~CT_OutResultModelManager()
{
    clearAllResultModels();
}

bool CT_OutResultModelManager::addResultModel(CT_OutAbstractResultModel *model)
{
    if(existInList(model))
    {
        delete model;
        return false;
    }

    _models.append(model);
    return true;
}

void CT_OutResultModelManager::clearAllResultModels()
{
    qDeleteAll(_models.begin(), _models.end());
    _models.clear();
}

bool CT_OutResultModelManager::isEmpty() const
{
    return _models.isEmpty();
}

const QList<CT_OutAbstractResultModel*>& CT_OutResultModelManager::models() const
{
    return _models;
}

QList<CT_OutAbstractResultModel *> CT_OutResultModelManager::takeModels()
{
    QList<CT_OutAbstractResultModel *> ret;

    while(!_models.isEmpty())
        ret.append(_models.takeFirst());

    return ret;
}

// PRIVATE //

bool CT_OutResultModelManager::existInList(CT_OutAbstractResultModel *model) const
{
    QListIterator<CT_OutAbstractResultModel*> it(_models);

    while(it.hasNext())
    {
        if(it.next()->uniqueName() == model->uniqueName())
            return true;
    }

    return false;
}
