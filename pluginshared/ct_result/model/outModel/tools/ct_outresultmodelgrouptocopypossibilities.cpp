#include "ct_outresultmodelgrouptocopypossibilities.h"

#include "ct_tools/model/ct_outmodelcopyactionaddmodelgroupingroup.h"
#include "ct_tools/model/ct_outmodelcopyactionaddmodelitemingroup.h"
#include "ct_tools/model/ct_outmodelcopyactionremovemodelgroupingroup.h"
#include "ct_tools/model/ct_outmodelcopyactionremovemodelitemingroup.h"

#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"

CT_OutResultModelGroupToCopyPossibilities::CT_OutResultModelGroupToCopyPossibilities()
{
}

bool CT_OutResultModelGroupToCopyPossibilities::addGroupModel(const QString &inParentGroupModelName,
                                                              CT_AutoRenameModels &autoOutModelName,
                                                              CT_AbstractItemGroup *group,
                                                              const QString &displayableName,
                                                              const QString &description)
{
    QListIterator<CT_OutResultModelGroupCopy*> it(m_models);

    while(it.hasNext())
    {
        CT_OutResultModelGroupCopy *model = it.next();

        CT_OutResultModelGroupToCopyPossibility *poss = model->outResultModelCopy();
        CT_VirtualAbstractStep *step = model->step();

        if((step == NULL) || (poss == NULL))
            return false;

        CT_OutStdGroupModel *groupModel = new CT_OutStdGroupModel("", (CT_AbstractItemGroup*)group->copy(NULL, NULL, CT_ResultCopyModeList()), displayableName, description);

        CT_OutModelCopyActionAddModelGroupInGroup action(inParentGroupModelName,
                                                         groupModel,
                                                         autoOutModelName);

        if(!action.execute(step, poss))
        {
            delete group;
            return false;
        }
    }

    delete group;

    return true;
}

bool CT_OutResultModelGroupToCopyPossibilities::addGroupModel(const CT_AutoRenameModels &autoOutParentGroupModelName,
                                                              CT_AutoRenameModels &autoOutModelName,
                                                              CT_AbstractItemGroup *group,
                                                              const QString &displayableName,
                                                              const QString &description)
{
    QListIterator<CT_OutResultModelGroupCopy*> it(m_models);

    while(it.hasNext())
    {
        CT_OutResultModelGroupCopy *model = it.next();

        CT_OutResultModelGroupToCopyPossibility *poss = model->outResultModelCopy();
        CT_VirtualAbstractStep *step = model->step();

        if((step == NULL) || (poss == NULL))
            return false;

        CT_OutStdGroupModel *groupModel = new CT_OutStdGroupModel("", (CT_AbstractItemGroup*)group->copy(NULL, NULL, CT_ResultCopyModeList()), displayableName, description);

        CT_OutModelCopyActionAddModelGroupInGroup action(autoOutParentGroupModelName,
                                                         groupModel,
                                                         autoOutModelName);

        if(!action.execute(step, poss))
        {
            delete group;
            return false;
        }
    }

    delete group;

    return true;
}

bool CT_OutResultModelGroupToCopyPossibilities::removeGroupModel(const QString &inGroupModelName)
{
    QListIterator<CT_OutResultModelGroupCopy*> it(m_models);

    while(it.hasNext())
    {
        CT_OutResultModelGroupCopy *model = it.next();

        CT_OutResultModelGroupToCopyPossibility *poss = model->outResultModelCopy();
        CT_VirtualAbstractStep *step = model->step();

        if((step == NULL) || (poss == NULL))
            return false;

        CT_OutModelCopyActionRemoveModelGroupInGroup action(inGroupModelName);

        if(!action.execute(step, poss))
            return false;
    }

    return true;
}

bool CT_OutResultModelGroupToCopyPossibilities::addItemModel(const QString &inParentGroupModelName,
                                                             CT_AutoRenameModels &autoOutModelName,
                                                             CT_AbstractSingularItemDrawable *item,
                                                             const QString &displayableName,
                                                             const QString &description)
{
    QListIterator<CT_OutResultModelGroupCopy*> it(m_models);

    while(it.hasNext())
    {
        CT_OutResultModelGroupCopy *model = it.next();

        CT_OutResultModelGroupToCopyPossibility *poss = model->outResultModelCopy();
        CT_VirtualAbstractStep *step = model->step();

        if((step == NULL) || (poss == NULL))
            return false;

        CT_OutStdSingularItemModel *itemModel = new CT_OutStdSingularItemModel("", (CT_AbstractSingularItemDrawable*)item->copy(NULL, NULL, CT_ResultCopyModeList()), displayableName, description);

        CT_OutModelCopyActionAddModelItemInGroup action(inParentGroupModelName,
                                                        itemModel,
                                                        autoOutModelName);

        if(!action.execute(step, poss))
        {
            delete item;
            return false;
        }
    }

    delete item;

    return true;
}

bool CT_OutResultModelGroupToCopyPossibilities::addItemModel(const CT_AutoRenameModels &autoOutParentGroupModelName,
                                                             CT_AutoRenameModels &autoOutModelName,
                                                             CT_AbstractSingularItemDrawable *item,
                                                             const QString &displayableName,
                                                             const QString &description)
{
    QListIterator<CT_OutResultModelGroupCopy*> it(m_models);

    while(it.hasNext())
    {
        CT_OutResultModelGroupCopy *model = it.next();

        CT_OutResultModelGroupToCopyPossibility *poss = model->outResultModelCopy();
        CT_VirtualAbstractStep *step = model->step();

        if((step == NULL) || (poss == NULL))
            return false;

        CT_OutStdSingularItemModel *itemModel = new CT_OutStdSingularItemModel("", (CT_AbstractSingularItemDrawable*)item->copy(NULL, NULL, CT_ResultCopyModeList()), displayableName, description);

        CT_OutModelCopyActionAddModelItemInGroup action(autoOutParentGroupModelName,
                                                        itemModel,
                                                        autoOutModelName);

        if(!action.execute(step, poss))
        {
            delete item;
            return false;
        }
    }

    delete item;

    return true;
}

bool CT_OutResultModelGroupToCopyPossibilities::removeItemModel(const QString &inItemModelName)
{
    QListIterator<CT_OutResultModelGroupCopy*> it(m_models);

    while(it.hasNext())
    {
        CT_OutResultModelGroupCopy *model = it.next();

        CT_OutResultModelGroupToCopyPossibility *poss = model->outResultModelCopy();
        CT_VirtualAbstractStep *step = model->step();

        if((step == NULL) || (poss == NULL))
            return false;

        CT_OutModelCopyActionRemoveModelItemInGroup action(inItemModelName);

        if(!action.execute(step, poss))
            return false;
    }

    return true;
}

void CT_OutResultModelGroupToCopyPossibilities::addResulModel(CT_OutResultModelGroupCopy *rModel)
{
    m_models.append(rModel);
}
