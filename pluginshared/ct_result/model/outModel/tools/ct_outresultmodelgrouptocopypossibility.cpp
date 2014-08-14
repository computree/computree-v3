#include "ct_outresultmodelgrouptocopypossibility.h"

#include "ct_outresultmodelgroup.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

CT_OutResultModelGroupToCopyPossibility::CT_OutResultModelGroupToCopyPossibility()
{
    _inModel = NULL;
    _possibility = NULL;
    _possibilityForSearch = NULL;
}

CT_OutResultModelGroupToCopyPossibility::~CT_OutResultModelGroupToCopyPossibility()
{
    delete _possibility;
}

CT_OutAbstractResultModelGroup* CT_OutResultModelGroupToCopyPossibility::outModelForModification() const
{
    if(_possibility == NULL)
        return NULL;

    return dynamic_cast<CT_OutAbstractResultModelGroup*>(_possibility->outModel());
}

const CT_OutAbstractResultModelGroup* CT_OutResultModelGroupToCopyPossibility::outModelForSearch() const
{
    if(_possibilityForSearch == NULL)
        return NULL;

    return dynamic_cast<const CT_OutAbstractResultModelGroup*>(_possibilityForSearch->outModel());
}

bool CT_OutResultModelGroupToCopyPossibility::setInModelAndPossibility(const CT_InResultModelGroupToCopy *inModel,
                                                                       CT_InStdModelPossibility *possibility,
                                                                       const CT_InStdModelPossibility *possibilityForSearch)
{
    _inModel = (CT_InResultModelGroupToCopy*)inModel;
    _possibility = possibility;
    _possibilityForSearch = (CT_InStdModelPossibility*)possibilityForSearch;

    return true;
}

CT_InResultModelGroupToCopy* CT_OutResultModelGroupToCopyPossibility::inModel() const
{
    return _inModel;
}

CT_InStdModelPossibility* CT_OutResultModelGroupToCopyPossibility::possibility() const
{
    return _possibility;
}

CT_InStdModelPossibility* CT_OutResultModelGroupToCopyPossibility::possibilityForSearch() const
{
    return _possibilityForSearch;
}

CT_OutResultModelGroupToCopyPossibility* CT_OutResultModelGroupToCopyPossibility::copy() const
{
    CT_OutResultModelGroupToCopyPossibility *cpy = new CT_OutResultModelGroupToCopyPossibility();

    CT_InStdModelPossibility *pCpy = _possibility->copy();
    pCpy->setOutModel(*(pCpy->outModel()->copy()));
    pCpy->setAutoDeleteOutModel(true);

    cpy->setInModelAndPossibility(_inModel, pCpy, _possibilityForSearch);

    return cpy;
}
