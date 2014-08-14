#ifndef CT_OUTRESULTMODELGROUPTOCOPYPOSSIBILITIES_H
#define CT_OUTRESULTMODELGROUPTOCOPYPOSSIBILITIES_H

#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibility.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

#include <QList>

/**
 * @brief Class used to add/remove elements to possibilities selected
 *        by the user of a INPUT model that represent a copy of a result.
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutResultModelGroupToCopyPossibilities
{
public:
    CT_OutResultModelGroupToCopyPossibilities();

    /**
     * @brief Add a new group model to another group model in the result.
     *
     * @param inParentGroupModelName : the unique name of the model of the parent group (typically DEF_....)
     * @param autoOutModelName : object that automatically renames the new group model to avoid the same modelName exists in the hierarchy
     * @param group : the ItemDrawable that represent the group (typically a CT_StandardItemGroup, example : new CT_TTreeGroup())
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the group model
     *
     * @return true if the group was added to the parent group. false if the unique name already exist in the hierarchy or if the parent group was not found.
     */
    bool addGroupModel(const QString &inParentGroupModelName,
                       CT_AutoRenameModels &autoOutModelName,
                       CT_AbstractItemGroup *group = new CT_StandardItemGroup(),
                       const QString &displayableName = "",
                       const QString &description = "");

    /**
     * @brief Add a new group model to another group model in the result. The parent group model must obligatory have been added by one of the method
     *        called addGroupModel(...).
     *
     * @param autoOutParentGroupModelName : object that know the name of the parent group.
     * @param autoOutModelName : object that automatically renames the new group model to avoid the same modelName exists in the hierarchy
     * @param group : the ItemDrawable that represent the group (typically a CT_StandardItemGroup, example : new CT_TTreeGroup())
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the group model
     *
     * @return true if the group was added to the parent group. false if the unique name already exist in the hierarchy or if the parent group was not found.
     */
    bool addGroupModel(const CT_AutoRenameModels &autoOutParentGroupModelName,
                       CT_AutoRenameModels &autoOutModelName,
                       CT_AbstractItemGroup *group = new CT_StandardItemGroup(),
                       const QString &displayableName = "",
                       const QString &description = "");

    /**
     * @brief Remove the group model that correspond to the IN model in the result.
     *
     * @param inGroupModelName : the model name of the IN group model (typically DEF_....)
     *
     * @return true if the group was removed.
     */
    bool removeGroupModel(const QString &inGroupModelName);

    /**
     * @brief Add a new ItemDrawable model to a group model in the result.
     *
     * @param parentGroupModelName : the unique name of the IN model of the parent group (typically DEF_....)
     * @param autoOutModelName : object that automatically renames the new item model to avoid the same modelName exists in the hierarchy
     * @param item : the ItemDrawable that represent the item (example : new CT_Scene())
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the item model
     *
     * @return true if the item was added to the parent group. false if the unique name already exist in the hierarchy or if the parent group was not found.
     */
    bool addItemModel(const QString &inParentGroupModelName,
                      CT_AutoRenameModels &autoOutModelName,
                      CT_AbstractSingularItemDrawable *item,
                      const QString &displayableName = "",
                      const QString &description = "");

    /**
     * @brief Add a new ItemDrawable model to a group model in the result. The parent group model must obligatory have been added by one of the method
     *        called addGroupModel(...).
     *
     * @param autoOutParentGroupModelName : object that know the name of the parent group.
     * @param autoOutModelName : object that automatically renames the new item model to avoid the same modelName exists in the hierarchy
     * @param item : the ItemDrawable that represent the item (example : new CT_Scene())
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the item model
     *
     * @return true if the item was added to the parent group. false if the unique name already exist in the hierarchy or if the parent group was not found.
     */
    bool addItemModel(const CT_AutoRenameModels &autoOutParentGroupModelName,
                      CT_AutoRenameModels &autoOutModelName,
                      CT_AbstractSingularItemDrawable *item,
                      const QString &displayableName = "",
                      const QString &description = "");

    /**
     * @brief Remove the ItemDrawable model that correspond to the IN model in the result.
     *
     * @param inItemModelName : the model name of the IN itemdrawable model (typically DEF_....)
     *
     * @return true if the item was removed.
     */
    bool removeItemModel(const QString &inItemModelName);

protected:

    friend class CT_VirtualAbstractStep;

    /**
     * @brief called by CT_VirtualAbstractStep to add a new model
     *        that represent a possibility of a INPUT model that represent a result
     */
    void addResulModel(CT_OutResultModelGroupCopy *rModel);

private:
    QList<CT_OutResultModelGroupCopy*>  m_models;
};

#endif // CT_OUTRESULTMODELGROUPTOCOPYPOSSIBILITIES_H
