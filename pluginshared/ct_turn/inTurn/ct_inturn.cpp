#include "ct_inturn.h"

#include "ct_virtualabstractstep.h"
#include "ct_abstractresult.h"
#include "ct_outabstractresultmodel.h"

#include "interfaces.h"

CT_InTurn::CT_InTurn()
{
}

CT_InTurn::~CT_InTurn()
{
    qDeleteAll(_models.begin(), _models.end());
}

bool CT_InTurn::addResultModel(CT_InAbstractResultModel *model)
{
    if(existInList(model))
        return false;

    _models.append(model);
    return true;
}

const QList<CT_InAbstractResultModel*>& CT_InTurn::models() const
{
    return _models;
}

bool CT_InTurn::isValid() const
{
    QListIterator<CT_InAbstractResultModel*> it(_models);

    while(it.hasNext())
    {
        CT_InAbstractResultModel *model = it.next();

        if(model->getPossibilitiesSavedSelected().size() < model->minimumNumberOfPossibilityThatMustBeSelectedForOneTurn())
            return false;

        if((model->minimumNumberOfPossibilityThatMustBeSelectedForOneTurn() > 0)
                && !model->recursiveIsAtLeastOnePossibilitySelectedIfItDoes())
            return false;
    }

    return true;
}

QList<CT_InResultsByModel*> CT_InTurn::getResultsFromStep(CT_VirtualAbstractStep *step) const
{
    QList<CT_InResultsByModel*> list;

    if(!isValid())
        return list;

    QListIterator<CT_InAbstractResultModel*> it(_models);

    while(it.hasNext())
    {
        CT_InAbstractResultModel *model = it.next();

        CT_InResultsByModel *resByModel = new CT_InResultsByModel(model);
        list.append(resByModel);

        if(model->needOutputModel()
                &&  !model->getPossibilitiesSavedSelected().isEmpty())
        {
            QList<CT_AbstractResult*> lResult = model->searchResultsInStepForPossibilitiesSelected(step);

            if(lResult.isEmpty())
            {
                qDeleteAll(list.begin(), list.end());
                list.clear();

                return list;
            }

            while(!lResult.isEmpty())
            {
                resByModel->addResult(lResult.takeFirst());
            }
        }
    }

    return list;
}

QList<SettingsNodeGroup*> CT_InTurn::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *root = new SettingsNodeGroup("Values");
    root->addValue(new SettingsNodeValue("Version", 1));
    retList.append(root);

    QListIterator<CT_InAbstractResultModel*> it(_models);

    while(it.hasNext())
    {
        SettingsNodeGroup *resModel = new SettingsNodeGroup("ResultModel");
        QList<SettingsNodeGroup*> groups = it.next()->getAllValues();

        while(!groups.isEmpty())
            resModel->addGroup(groups.takeFirst());

        root->addGroup(resModel);
    }

    return retList;
}

bool CT_InTurn::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if(list.isEmpty()
            || (list.first()->name() != "Values"))
        return false;

    SettingsNodeGroup *root = list.first();

    // les infos des modèles
    QList<SettingsNodeGroup*> groups = root->groupsByTagName("ResultModel");

    if(groups.size() != _models.size())
        return false;

    int i=0;
    QListIterator<CT_InAbstractResultModel*> it(_models);

    while(it.hasNext())
    {
        CT_InAbstractResultModel *resModel = it.next();

        if(!resModel->setAllValues(groups.at(i)->groups()))
            return false;

        ++i;
    }

    return true;
}

// PRIVATE //

bool CT_InTurn::existInList(CT_InAbstractResultModel *model) const
{
    QListIterator<CT_InAbstractResultModel*> it(_models);

    while(it.hasNext())
    {
        if(it.next()->uniqueName() == model->uniqueName())
            return true;
    }

    return false;
}
