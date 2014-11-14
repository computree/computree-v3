#ifndef CT_OUTMODELCOPYACTIONADDMODELITEMATTRIBUTEINITEM_H
#define CT_OUTMODELCOPYACTIONADDMODELITEMATTRIBUTEINITEM_H

#include "ct_tools/model/abstract/ct_abstractoutmodelcopyaction.h"

#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

/** \class  CT_OutModelCopyActionAddModelItemAttributeInItem
    \brief  Class of an action that ADDS a ITEM ATTRIBUTE in one or more ITEM that will be found in the outModel copy by using the in item model name.

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

    CT_OutStdItemAttributeModel *newItemAttribute = new CT_OutStdItemAttributeModel("", new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_NUMBER));    // create a new Item (is a Circle)

    QList<CT_AbstractOutModelCopyAction*> actions;  // create a liste of actions

    // add the new action
    actions << new CT_OutModelCopyActionAddModelItemAttributeInItem(DEF_SearchInItemDrawable, newItemAttribute, _autoRenameNewItemAttribute);  // _autoRenameNewItemAttribute is a CT_AutoRenameModels. It is obligatory to use this when you
                                                                                                                                               // add a model in a copy.

    // create one or more copy and execute all actions that be in the list
    addOutResultModelCopy( getInResultModel(DEF_SearchInResult), actions);

*/
class PLUGINSHAREDSHARED_EXPORT CT_OutModelCopyActionAddModelItemAttributeInItem : public CT_AbstractOutModelCopyAction
{
public:
    /**
     * @brief Constuctor
     *
     * @param inItemModelNameToSearch : model name of the in item model that be created in "createInResultModelListProtected" method.
     * @param itemAttributeModelToAdd : new item attribute model to add
     * @param autoRenameItemAttributeModelToAdd : autoRename object that will be used to named the new item attribute when he will be added to the copy of the result model.
     */
    CT_OutModelCopyActionAddModelItemAttributeInItem(const QString &inItemModelNameToSearch,
                                                     CT_OutAbstractItemAttributeModel *itemAttributeModelToAdd,
                                                     CT_AutoRenameModels &autoRenameItemAttributeModelToAdd);

    /**
     * @brief Constuctor with autoRename object
     *
     * @param inItemModelNameToSearch : autoRename object that will be used to search the item.
     * @param itemAttributeModelToAdd : new item attribute model to add
     * @param autoRenameItemAttributeModelToAdd : autoRename object that will be used to named the new item attribute when he will be added to the copy of the result model.
     */
    CT_OutModelCopyActionAddModelItemAttributeInItem(const CT_AutoRenameModels &inItemModelNameToSearch,
                                                     CT_OutAbstractItemAttributeModel *itemAttributeModelToAdd,
                                                     CT_AutoRenameModels &autoRenameItemAttributeModelToAdd);

    ~CT_OutModelCopyActionAddModelItemAttributeInItem();

    bool execute(const CT_VirtualAbstractStep *step, CT_OutResultModelGroupToCopyPossibility *modelCopy);

private:
   QString                              m_inItemModelNameToSearch;
   CT_OutAbstractItemAttributeModel     *m_itemAttributeModelToAdd;
   CT_AutoRenameModels                  *m_autoRenameItemAttributeModelToAdd;
   CT_AutoRenameModels                  *m_autoRenameItemModelNameToSearch;
};

#endif // CT_OUTMODELCOPYACTIONADDMODELITEMATTRIBUTEINITEM_H
