#ifndef CT_OUTMODELCOPYACTIONADDMODELITEMINGROUP_H
#define CT_OUTMODELCOPYACTIONADDMODELITEMINGROUP_H

#include "ct_tools/model/abstract/ct_abstractoutmodelcopyaction.h"

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"

/** \class  CT_OutModelCopyActionAddModelItemInGroup
    \brief  Class of an action that ADDS a ITEM in one or more GROUP that will be found in the outModel copy by using the in group model name.

    \example createInResultModelListProtected :
             ----------------------------------

    CT_InZeroOrMoreGroupModel *rootG = new CT_InZeroOrMoreGroupModel();
    CT_InStandardGroupModel *group = new CT_InStandardGroupModel(DEF_SearchInGroup);
    CT_InStdSingularItemModel *item = new CT_InStdSingularItemModel(DEF_SearchInItemDrawable,
                                                                              CT_AbstractItemDrawable::staticGetType());

    rootG->addGroup(group);
    group->addItem(item);

    addInResultModel(new CT_InResultModelGroupToCopy(DEF_SearchInResult, rootG));

            createOutResultModelListProtected :
            -----------------------------------

    CT_OutStandardItemDrawableModel *newItem = new CT_OutStandardItemDrawableModel("", new CT_Circle());    // create a new Item (is a Circle)

    QList<CT_AbstractOutModelCopyAction*> actions;  // create a liste of actions

    // add the new action
    actions << new CT_OutModelCopyActionAddModelItemInGroup(DEF_SearchInGroup, newItem, _autoRenameNewItem);  // _autoRenameNewItem is a CT_AutoRenameModels. It is obligatory to use this when you
                                                                                                              // add a model in a copy.

    // create one or more copy and execute all actions that be in the list
    addOutResultModelCopy( getInResultModel(DEF_SearchInResult), actions);

*/
class PLUGINSHAREDSHARED_EXPORT CT_OutModelCopyActionAddModelItemInGroup : public CT_AbstractOutModelCopyAction
{
public:

    /*!
    *  \brief Constructor
    *
    *  Constructor of the class
    *
    *  \param inGroupModelNameToSearch : model name of the in group model that be created in "createInResultModelListProtected" method.
    *  \param itemModelToAdd : new item model to add
    *  \param autoRenameItemModelToAdd : autoRename object that will be used to named the new item when he will be added to the copy of the result model.
    */
    CT_OutModelCopyActionAddModelItemInGroup(const QString &inGroupModelNameToSearch,
                                             CT_OutAbstractSingularItemModel *itemModelToAdd,
                                             CT_AutoRenameModels &autoRenameItemModelToAdd);

    /*!
    *  \brief Constructor
    *
    *  Constructor of the class. You can use this version of constructor if you want to add a new item to a previous new group added with a CT_OutModelCopyActionAddModelGroupInGroup object.
    *
    *  \param otherNewGroupModelNameToSearch : autoRename object that will be used to search the group. (autorename object used for the previous group added)
    *  \param itemModelToAdd : new item model to add
    *  \param autoRenameItemModelToAdd : autoRename object that will be used to named the new item when he will be added to the copy of the result model.
    *
    *  \example actions << new CT_OutModelCopyActionAddModelGroupInGroup(DEF_SearchInGroup,
    *                                                                    myNewGroupToAdd,
    *                                                                    _autoRenameObjectForFirstNewGroup);
    *
    *           actions << new CT_OutModelCopyActionAddModelItemInGroup(_autoRenameObjectForFirstNewGroup,
    *                                                                    myNewItem,
    *                                                                    _autoRenameObjectForNewItem);
    */
    CT_OutModelCopyActionAddModelItemInGroup(const CT_AutoRenameModels &otherNewGroupModelNameToSearch,
                                             CT_OutAbstractSingularItemModel *itemModelToAdd,
                                             CT_AutoRenameModels &autoRenameItemModelToAdd);

    ~CT_OutModelCopyActionAddModelItemInGroup();

    bool execute(const CT_VirtualAbstractStep *step, CT_OutResultModelGroupToCopyPossibility *modelCopy);

private:
   QString                          _inGroupModelNameToSearch;
   CT_OutAbstractSingularItemModel  *_itemModelToAdd;
   CT_AutoRenameModels              *_autoRenameItemModelToAdd;
   CT_AutoRenameModels              *_autoRenameGroupModelToSearch;
};

#endif // CT_OUTMODELCOPYACTIONADDMODELITEMINGROUP_H
