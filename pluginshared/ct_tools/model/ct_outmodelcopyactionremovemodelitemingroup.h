#ifndef CT_OUTMODELCOPYACTIONREMOVEMODELITEMINGROUP_H
#define CT_OUTMODELCOPYACTIONREMOVEMODELITEMINGROUP_H

#include "ct_tools/model/abstract/ct_abstractoutmodelcopyaction.h"

/** \class  CT_OutModelCopyActionRemoveModelItemInGroup
    \brief  Class of an action that REMOVES an ITEM in one or more GROUP that will be found in the outModel copy by using the in group model name.

    \example createInResultModelListProtected :
             ----------------------------------

    CT_InStandardGroupModel *group = new CT_InStandardGroupModel(DEF_SearchInGroup);
    CT_InStdSingularItemModel *item = new CT_InStdSingularItemModel(DEF_SearchInItemDrawableToRemove,
                                                                              CT_AbstractItemDrawable::staticGetType());

    group->addItem(item);

    addInResultModel(new CT_InResultModelGroupToCopy(DEF_SearchInResult, group));

            createOutResultModelListProtected :
            -----------------------------------

    QList<CT_AbstractOutModelCopyAction*> actions;  // create a liste of actions

    // add the new action
    actions << new CT_OutModelCopyActionRemoveModelItemInGroup(DEF_SearchInItemDrawableToRemove);

    // create one or more copy and execute all actions that be in the list
    addOutResultModelCopy( getInResultModel(DEF_SearchInResult), actions);

*/
class PLUGINSHAREDSHARED_EXPORT CT_OutModelCopyActionRemoveModelItemInGroup : public CT_AbstractOutModelCopyAction
{
public:
    /*!
    *  \brief Constructor
    *
    *  Constructor of the class
    *
    *  \param inItemModelNameToSearchAndRemove : model name of the in item model to remove.
    */
    CT_OutModelCopyActionRemoveModelItemInGroup(const QString &inItemModelNameToSearchAndRemove);

    bool execute(const CT_VirtualAbstractStep *step, CT_OutResultModelGroupToCopyPossibility *modelCopy);

 private:
    QString         _inItemModelNameToSearchAndRemove;
};

#endif // CT_OUTMODELCOPYACTIONREMOVEMODELITEMINGROUP_H
