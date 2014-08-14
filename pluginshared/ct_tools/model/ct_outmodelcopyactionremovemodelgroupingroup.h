#ifndef CT_OUTMODELCOPYACTIONREMOVEMODELGROUPINGROUP_H
#define CT_OUTMODELCOPYACTIONREMOVEMODELGROUPINGROUP_H

#include "ct_tools/model/abstract/ct_abstractoutmodelcopyaction.h"
#include "ct_itemdrawable/model/outModel/abstract/def_ct_abstractgroupmodelout.h"

/** \class  CT_OutModelCopyActionRemoveModelGroupInGroup
    \brief  Class of an action that REMOVES a GROUP in one or more GROUP that will be found in the outModel copy by using the in group model name.

    \example createInResultModelListProtected :
             ----------------------------------

    CT_InStandardGroupModel *group = new CT_InStandardGroupModel(DEF_SearchInGroup);
    CT_InStandardGroupModel *groupToRemove = new CT_InStandardGroupModel(DEF_SearchInGroupToRemove);

    group->addGroup(groupToRemove);

    addInResultModel(new CT_InResultModelGroupToCopy(DEF_SearchInResult, group));

            createOutResultModelListProtected :
            -----------------------------------

    QList<CT_AbstractOutModelCopyAction*> actions;  // create a liste of actions

    // add the new action
    actions << new CT_OutModelCopyActionRemoveModelGroupInGroup(DEF_SearchInGroupToRemove);

    // create one or more copy and execute all actions that be in the list
    addOutResultModelCopy( getInResultModel(DEF_SearchInResult), actions);

*/
class PLUGINSHAREDSHARED_EXPORT CT_OutModelCopyActionRemoveModelGroupInGroup : public CT_AbstractOutModelCopyAction
{
public:
    /*!
    *  \brief Constructor
    *
    *  Constructor of the class
    *
    *  \param inGroupModelNameToSearchAndRemove : model name of the in group model to remove.
    */
    CT_OutModelCopyActionRemoveModelGroupInGroup(const QString &inGroupModelNameToSearchAndRemove);

    bool execute(const CT_VirtualAbstractStep *step, CT_OutResultModelGroupToCopyPossibility *modelCopy);

 private:
    QString         _inGroupModelNameToSearchAndRemove;

    void recursiveRemoveAllChildsGroupFromList(DEF_CT_AbstractGroupModelOut *g, QList<DEF_CT_AbstractGroupModelOut*> &gList);
};

#endif // CT_OUTMODELCOPYACTIONREMOVEMODELGROUPINGROUP_H
