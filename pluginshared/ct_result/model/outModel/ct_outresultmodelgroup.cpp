#include "ct_outresultmodelgroup.h"

#include "ct_result/ct_resultgroup.h"

#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"

CT_OutResultModelGroup::CT_OutResultModelGroup(const QString &uniqueName,
                                               DEF_CT_AbstractGroupModelOut *rootGroupModel,
                                               const QString &resultName,
                                               const QString &displayableName,
                                               const QString &description) : CT_OutAbstractResultModelGroup(uniqueName,
                                                                                                            description,
                                                                                                            displayableName,
                                                                                                            resultName)
{
    _group = NULL;
    setRootGroup(rootGroupModel);
}

QString CT_OutResultModelGroup::modelTypeDisplayable() const
{
    return QString("CT_OutResultModelGroup");
}

CT_OutResultModelGroup::~CT_OutResultModelGroup()
{
    delete _group;
}

DEF_CT_AbstractGroupModelOut* CT_OutResultModelGroup::rootGroup() const
{
    return _group;
}

void CT_OutResultModelGroup::setRootGroup(DEF_CT_AbstractGroupModelOut *rootGroup)
{
    delete _group;
    _group = rootGroup;

    staticSetParentModelToModel(_group, this);
}

void CT_OutResultModelGroup::setRootGroup(const QString &uniqueName,
                                          CT_AbstractItemGroup *group,
                                          const QString &displayableName,
                                          const QString &description)
{
    CT_OutStdGroupModel *model = new CT_OutStdGroupModel(uniqueName, group, displayableName, description);
    setRootGroup(model);
}


bool CT_OutResultModelGroup::addGroupModel(const QString &parentGroupUniquelName,
                                           const QString &uniqueName,
                                           CT_AbstractItemGroup *group,
                                           const QString &displayableName,
                                           const QString &description)
{
    return addGroupModelT<CT_OutStdGroupModel>(parentGroupUniquelName, uniqueName, group, displayableName, description);
}

bool CT_OutResultModelGroup::addGroupModel(const QString &parentGroupUniqueName,
                                           DEF_CT_AbstractGroupModelOut *groupModel)
{
    DEF_CT_AbstractGroupModelOut *rootModel = dynamic_cast<DEF_CT_AbstractGroupModelOut*>(this->findModelInTree(parentGroupUniqueName));

    if(rootModel == NULL || groupModel == NULL)
        return false;

    if(rootModel->addGroup(groupModel))
        return true;

    return false;
}

bool CT_OutResultModelGroup::addItemModel(const QString &parentGroupUniqueName,
                                          const QString &uniqueName,
                                          CT_AbstractSingularItemDrawable *item,
                                          const QString &displayableName,
                                          const QString &description)
{
    DEF_CT_AbstractGroupModelOut *rootModel = dynamic_cast<DEF_CT_AbstractGroupModelOut*>(findModelInTree(parentGroupUniqueName));

    if(rootModel == NULL)
        return false;

    CT_OutStdSingularItemModel *newModel = new CT_OutStdSingularItemModel(uniqueName, item, displayableName, description);

    return rootModel->addItem(newModel);
}

bool CT_OutResultModelGroup::addItemAttributeModel(const QString &parentItemUniqueName,
                                                   const QString &uniqueName,
                                                   CT_AbstractItemAttribute *ia,
                                                   const QString &displayableName,
                                                   const QString &description)
{
    DEF_CT_AbstractItemDrawableModelOut *parentModel = dynamic_cast<DEF_CT_AbstractItemDrawableModelOut*>(findModelInTree(parentItemUniqueName));

    if(parentModel == NULL)
        return false;

    CT_OutStdItemAttributeModel *newModel = new CT_OutStdItemAttributeModel(uniqueName, ia, displayableName, description);

    return parentModel->addItemAttribute(newModel);
}

CT_AbstractResult* CT_OutResultModelGroup::createResult(CT_ResultInitializeData &data) const
{
    if(step() == NULL)
        return NULL;

    CT_ResultGroup *res = new CT_ResultGroup(*this, data, *step());

    if(!resultName().isEmpty())
        res->setName(resultName());

    return res;
}

CT_OutAbstractModel* CT_OutResultModelGroup::copy() const
{
    CT_OutResultModelGroup *cpy = new CT_OutResultModelGroup(uniqueName(),
                                                             _group->copyGroup(),
                                                             resultName(),
                                                             displayableName(),
                                                             description());

    if(turnIndex() >= 0)
        cpy->setTurnIndex(turnIndex());

    cpy->setStep(step());
    cpy->setResult(result());
    cpy->setOriginalModel(this);

    return cpy;
}
