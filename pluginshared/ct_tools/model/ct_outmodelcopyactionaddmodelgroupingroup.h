#ifndef CT_OUTMODELCOPYACTIONADDMODELGROUPINGROUP_H
#define CT_OUTMODELCOPYACTIONADDMODELGROUPINGROUP_H

#include "ct_tools/model/abstract/ct_abstractoutmodelcopyaction.h"

#include "ct_itemdrawable/model/outModel/abstract/def_ct_abstractgroupmodelout.h"

/** \class  CT_OutModelCopyActionAddModelGroupInGroup
    \brief  Class of an action that ADDS a GROUP in one or more GROUP that will be found in the outModel copy by using the in group model name.

    \example createInResultModelListProtected :
             ----------------------------------

    CT_InStandardGroupModel *group = new CT_InStandardGroupModel(DEF_SearchInGroup);

    addInResultModel(new CT_InResultModelGroupToCopy(DEF_SearchInResult, group));

            createOutResultModelListProtected :
            -----------------------------------

    CT_OutStandardGroupModel *newGroup = CT_OutStandardGroupModel("");  // create a new Group

    CT_OutStandardItemDrawableModel *newItem = new CT_OutStandardItemDrawableModel("", new CT_Circle());    // create a new Item (is a Circle)

    newGroup->addItem(newItem, _autoRenameNewCircle);   // _autoRenameNewCircle is a CT_AutoRenameModels. It is obligatory to use this when you
                                                        // add a model in a copy.

    QList<CT_AbstractOutModelCopyAction*> actions;  // create a liste of actions

    // add the new action
    actions << new CT_OutModelCopyActionAddModelGroupInGroup(DEF_SearchInGroup, newGroup, _autoRenameNewGroup);  // _autoRenameNewGroup is a CT_AutoRenameModels. It is obligatory to use this when you
                                                                                                                 // add a model in a copy.

    // create one or more copy and execute all actions that be in the list
    addOutResultModelCopy( getInResultModel(DEF_SearchInResult), actions);

*/
class PLUGINSHAREDSHARED_EXPORT CT_OutModelCopyActionAddModelGroupInGroup : public CT_AbstractOutModelCopyAction
{
public:

    /*!
    *  \brief Constructor
    *
    *  Constructor of the class
    *
    *  \param inGroupModelNameToSearch : model name of the in group model that be created in "createInResultModelListProtected" method.
    *  \param groupModelToAdd : new group model to add
    *  \param autoRenameGroupModelToAdd : autoRename object that will be used to named the new group when he will be added to the copy of the result model.
    */
    CT_OutModelCopyActionAddModelGroupInGroup(const QString &inGroupModelNameToSearch,
                                              DEF_CT_AbstractGroupModelOut *groupModelToAdd,
                                              CT_AutoRenameModels &autoRenameGroupModelToAdd);

    /*!
    *  \brief Constructor
    *
    *  Constructor of the class. You can use this version of constructor if you want to add a new group to a previous new group added with a CT_OutModelCopyActionAddModelGroupInGroup object.
    *
    *  \param otherNewGroupModelNameToSearch : autoRename object that will be used to search the group. (autorename object used for the previous group added)
    *  \param groupModelToAdd : new group model to add
    *  \param autoRenameGroupModelToAdd : autoRename object that will be used to named the new group when he will be added to the copy of the result model.
    *
    *  \example actions << new CT_OutModelCopyActionAddModelGroupInGroup(DEF_SearchInGroup,
    *                                                                    myNewGroupToAdd,
    *                                                                    _autoRenameObjectForFirstNewGroup);
    *
    *           actions << new CT_OutModelCopyActionAddModelGroupInGroup(_autoRenameObjectForFirstNewGroup,
    *                                                                    mySecondNewGroupToAddInFirstNewGroup,
    *                                                                    _autoRenameObjectForSecondNewGroup);
    */
    CT_OutModelCopyActionAddModelGroupInGroup(const CT_AutoRenameModels &otherNewGroupModelNameToSearch,
                                              DEF_CT_AbstractGroupModelOut *groupModelToAdd,
                                              CT_AutoRenameModels &autoRenameGroupModelToAdd);

    ~CT_OutModelCopyActionAddModelGroupInGroup();

    bool execute(const CT_VirtualAbstractStep *step, CT_OutResultModelGroupToCopyPossibility *modelCopy);

private:
    QString                         _inGroupModelNameToSearch;
    DEF_CT_AbstractGroupModelOut    *_groupModelToAdd;
    CT_AutoRenameModels             *_autoRenameGroupModelToAdd;
    CT_AutoRenameModels             *_autoRenameGroupModelToSearch;
};

#endif // CT_OUTMODELCOPYACTIONADDMODELGROUPINGROUP_H
