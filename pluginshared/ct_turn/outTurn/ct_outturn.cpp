#include "ct_outturn.h"

#include "ct_resultinitializedata.h"
#include "ct_abstractresult.h"

CT_OutTurn::CT_OutTurn()
{
}

CT_OutTurn::~CT_OutTurn()
{
    clearModels();
}

bool CT_OutTurn::addResultModel(CT_OutAbstractResultModel *model)
{
    if(existInList(model))
        return false;

    _models.append(model);

    return true;
}

const QList<CT_OutAbstractResultModel*>& CT_OutTurn::models() const
{
    return _models;
}

QList<CT_AbstractResult*> CT_OutTurn::createResults() const
{
    QList<CT_AbstractResult*> list;

    QListIterator<CT_OutAbstractResultModel*> it(_models);

    CT_ResultInitializeData *data;

    while(it.hasNext())
    {
        data = new CT_ResultInitializeData(list.size());

        CT_AbstractResult *res = it.next()->createResult(*data);

        if(res == NULL)
        {
            qDeleteAll(list.begin(), list.end());
            list.clear();

            return list;
        }

        list.append(res);
    }

    return list;
}

// PRIVATE //

bool CT_OutTurn::existInList(CT_OutAbstractResultModel *model) const
{
    QListIterator<CT_OutAbstractResultModel*> it(_models);

    while(it.hasNext())
    {
        if(it.next()->uniqueName() == model->uniqueName())
            return true;
    }

    return false;
}

void CT_OutTurn::clearModels()
{
    qDeleteAll(_models.begin(), _models.end());
    _models.clear();
}
