#include "ct_outresultmodelgroupcopy.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_inresultmodelgrouptocopy.h"
#include "ct_resultgroup.h"

CT_OutResultModelGroupCopy::CT_OutResultModelGroupCopy(QString uniqueName,
                                                       CT_OutResultModelGroupToCopyPossibility *copy,
                                                       QString resultName,
                                                       QString displayableName,
                                                       QString description) : CT_OutAbstractResultModelGroup(uniqueName,
                                                                                                             description,
                                                                                                             displayableName.isEmpty() ? copy->outModelForSearch()->displayableName() : displayableName,
                                                                                                             resultName)
{
    _outModelPossibility = copy;
    _inModelCopy = NULL;
}

QString CT_OutResultModelGroupCopy::modelTypeDisplayable() const
{
    return QString("CT_OutResultModelGroupCopy");
}

CT_OutResultModelGroupCopy::~CT_OutResultModelGroupCopy()
{
    delete _outModelPossibility;
}

void CT_OutResultModelGroupCopy::setRootGroup(DEF_CT_AbstractGroupModelOut *rootGroup)
{
    _outModelPossibility->outModelForModification()->setRootGroup(rootGroup);
}

DEF_CT_AbstractGroupModelOut* CT_OutResultModelGroupCopy::rootGroup() const
{
    return _outModelPossibility->outModelForModification()->rootGroup();
}

CT_AbstractResult* CT_OutResultModelGroupCopy::createResult(CT_ResultInitializeData &data) const
{
    // on récupère l'étape qui doit créer ce résultat
    CT_VirtualAbstractStep *step = _outModelPossibility->outModelForModification()->step();

    // on recherche le résultat dans cette étape
    CT_AbstractResult* res = _outModelPossibility->inModel()->searchResultsInStepForThisPossibility(step,
                                                                                                    _outModelPossibility->possibilityForSearch());

    // si il y a eu une erreur !
    if(res == NULL)
        return NULL;

    // sinon on retourne une copie
    res = res->createNewInstance((CT_OutAbstractResultModel*)this,
                                 data,
                                 *this->step());

    if(!resultName().isEmpty())
        res->setName(resultName());
    else if(!res->getName().contains(" (COPY)"))
        res->setName(res->getName() + " (COPY)");
    else
        res->setName(res->getName());

    return res;
}

CT_OutAbstractModel* CT_OutResultModelGroupCopy::originalModel() const
{
    // on récupère l'étape qui doit créer ce résultat
    CT_VirtualAbstractStep *step = _outModelPossibility->outModelForModification()->step();

    return _outModelPossibility->inModel()->searchResultsInStepForThisPossibility(step,
                                                                                  _outModelPossibility->possibilityForSearch())->model()->originalModel();
}

CT_OutAbstractResultModel* CT_OutResultModelGroupCopy::copy() const
{
    CT_OutResultModelGroupCopy *cpy = new CT_OutResultModelGroupCopy(uniqueName(),
                                                                     _outModelPossibility->copy(),
                                                                     resultName(),
                                                                     displayableName(),
                                                                     description());

    if(turnIndex() >= 0)
        cpy->setTurnIndex(turnIndex());

    cpy->setStep(step());

    return cpy;
}

CT_OutResultModelGroupToCopyPossibility* CT_OutResultModelGroupCopy::outResultModelCopy() const
{
    return _outModelPossibility;
}

const CT_InResultModelGroupToCopy* CT_OutResultModelGroupCopy::inResultModelCopy() const
{
    return _inModelCopy;
}

void CT_OutResultModelGroupCopy::setInResultCopyModel(const CT_InResultModelGroupToCopy *model)
{
    _inModelCopy = (CT_InResultModelGroupToCopy*)model;
}
