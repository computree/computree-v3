#include "ct_inresultmodelmanager.h"

#include "ct_virtualabstractstep.h"
#include "ct_result/model/outModel/abstract/ct_outabstractresultmodelgroup.h"

#include <QDebug>
#include "ct_inresultmodelgroup.h"

CT_InResultModelManager::CT_InResultModelManager()
{
}

CT_InResultModelManager::~CT_InResultModelManager()
{
    clearAllResultModels();
}

bool CT_InResultModelManager::addResultModel(CT_InAbstractResultModel *model)
{
    if(existInList(model))
    {
        delete model;
        return false;
    }

    _models.append(model);
    return true;
}

void CT_InResultModelManager::clearAllResultModels()
{
    qDeleteAll(_models.begin(), _models.end());
    _models.clear();
}

bool CT_InResultModelManager::isAllOutputModelFoundFromStep(const CT_VirtualAbstractStep *step)
{
    return searchOneOrSearchMultipleAndSaveAllOutputModelFromStep(step, false, true);
}

bool CT_InResultModelManager::findAllOutputModelFromStepAndSavePossibilities(const CT_VirtualAbstractStep *step)
{
    return searchOneOrSearchMultipleAndSaveAllOutputModelFromStep(step, true, true);
}

bool CT_InResultModelManager::isEmpty() const
{
    return _models.isEmpty();
}

bool CT_InResultModelManager::needInputResults() const
{
    QListIterator<CT_InAbstractResultModel*> it(_models);

    while(it.hasNext())
    {
        if(it.next()->needOutputModel())
            return true;
    }

    return false;
}

const QList<CT_InAbstractResultModel*>& CT_InResultModelManager::models() const
{
    return _models;
}

// PRIVATE //

bool CT_InResultModelManager::existInList(const CT_InAbstractResultModel *model) const
{
    QListIterator<CT_InAbstractResultModel*> it(_models);

    while(it.hasNext())
    {
        if(it.next()->uniqueName() == model->uniqueName())
            return true;
    }

    return false;
}

bool CT_InResultModelManager::searchOneOrSearchMultipleAndSaveAllOutputModelFromStep(const CT_VirtualAbstractStep *step,
                                                                                     const bool &searchAndSave,
                                                                                     const bool &returnTrueIfNotFoundAndMinimumPossibilitiesEqualsZero)
{
    if(_models.isEmpty())
        return false;

    bool found = true;

    QListIterator<CT_InAbstractResultModel*> inIt(_models);

    // if we must save possibilities
    if(searchAndSave)
    {
        // we clear all possibilities of models
        while(inIt.hasNext())
            inIt.next()->clearPossibilitiesSaved();

        inIt.toFront();
    }

    // for each models and while we found olbigatory elements
    while(inIt.hasNext()
          && found)
    {
        CT_InAbstractResultModel *model = inIt.next();

        if(model->needOutputModel())
        {
            if(model->isRecursive())
                found = recursiveFindOutputModelFromStepForModel(step, model, searchAndSave, searchAndSave);
            else
                found = findOutputModelFromStepForModel(step, model, searchAndSave, searchAndSave);

            // si on a pas trouvé de possibilité
            // et qu'on veut quand même renvoyer true
            // si le modèle est optionnel
            if(!found
                    && returnTrueIfNotFoundAndMinimumPossibilitiesEqualsZero
                    && (model->minimumNumberOfPossibilityThatMustBeSelectedForOneTurn() == 0))
                found = true;
        }
    }

    return found;
}

bool CT_InResultModelManager::recursiveFindOutputModelFromStepForModel(const CT_VirtualAbstractStep *step,
                                                                       CT_InAbstractResultModel *inModel,
                                                                       const bool &savePossibilities,
                                                                       const bool &multiple)
{
    bool found = findOutputModelFromStepForModel(step,
                                                 inModel,
                                                 savePossibilities,
                                                 multiple);

    if((!found || multiple)
            && (step != NULL))
    {
        CT_VirtualAbstractStep *parent = step->parentStep();

        if(parent != NULL)
        {
            if(recursiveFindOutputModelFromStepForModel(parent,
                                                        inModel,
                                                        savePossibilities,
                                                        multiple))
                found = true;
        }
    }

    return found;
}

bool CT_InResultModelManager::findOutputModelFromStepForModel(const CT_VirtualAbstractStep *step,
                                                              CT_InAbstractResultModel *inModel,
                                                              const bool &savePossibilities,
                                                              const bool &multiple)
{
    if(step == NULL)
        return false;

    bool found = false;

    // get all OUTPUT models (sorted by turn) of this step that represent results
    QList< QList<CT_OutAbstractResultModelGroup*> > outModelTurnList = step->getAllOutResultModels();

    QListIterator< QList<CT_OutAbstractResultModelGroup*> > itTurn(outModelTurnList);

    // for each turn and while we have not found obligatory elements
    while(itTurn.hasNext()
          && (!found || multiple))
    {
        QListIterator<CT_OutAbstractResultModelGroup*> outIt(itTurn.next());

        // for each models of this turn and while we have not found obligatory elements
        while(outIt.hasNext()
              && (!found || multiple))
        {
            if(multiple)
            {
                if(inModel->recursiveFindAllPossibilitiesInModel(*outIt.next(), savePossibilities))
                    found = true;
            }
            else
            {
                if(inModel->recursiveFindOnePossibilityInModel(*outIt.next(), savePossibilities))
                    found = true;
            }
        }
    }

    return found;
}
