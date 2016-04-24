#ifndef CT_OUTRESULTMODELGROUP_H
#define CT_OUTRESULTMODELGROUP_H

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodelgroup.h"
#include "ct_itemdrawable/model/outModel/abstract/def_ct_abstractgroupmodelout.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

/**
 * @brief Use this class if you want to create a NEW result in your step.
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutResultModelGroup : public CT_OutAbstractResultModelGroup
{
    Q_OBJECT

public:
    /**
     * @brief Construct a OUTPUT model that represent a result
     * @param uniqueName : a unique name in all the hierarchy
     * @param rootGroupModel : the root model that represent a group
     * @param resultName: the name that will be set to the result. If empty use uniqueName. Empty by default.
     * @param displayableName : a displayable name (used in gui for user information). Empty by default.
     * @param description : a description of the result taht will be generated. Empty by default.
     */
    CT_OutResultModelGroup(const QString &uniqueName,
                           DEF_CT_AbstractGroupModelOut *rootGroupModel,
                           const QString &resultName = "",
                           const QString &displayableName = "",
                           const QString &description = "");
    ~CT_OutResultModelGroup();

    /**
     * @brief Returns the model type in a displayable QString (for gui)
     */
    virtual QString modelTypeDisplayable() const;

    /**
     * @brief Return the root model that represent a group
     */
    virtual DEF_CT_AbstractGroupModelOut* rootGroup() const;

    /**
     * @brief Set the root model that represent a group (delete the last root group from memory if exist).
     */
    virtual void setRootGroup(DEF_CT_AbstractGroupModelOut *rootGroup);

    /**
     * @brief Set the root group (delete the last root group from memory if exist).
     *
     * @param modelName : unique name of the new group model (typically DEF_....)
     * @param group : the ItemDrawable that represent the group (typically a CT_StandardItemGroup)
     * @param displayableName : displayable name that will be show in gui
     * @param description : description of the group model
     */
    virtual void setRootGroup(const QString &uniqueName,
                      CT_AbstractItemGroup *group = new CT_StandardItemGroup(),
                      const QString &displayableName = "",
                      const QString &description = "");

    /**
     * @brief Add a group model to another group model in the result.
     *
     * @param parentGroupUniquelName : the unique name of the model of the parent group (typically DEF_....)
     * @param uniqueName : unique name of the new group model (typically DEF_....)
     * @param group : the ItemDrawable that represent the group (typically a CT_StandardItemGroup, example : new CT_TTreeGroup())
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the group model
     *
     * @return true if the group was added to the parent group. false if the unique name already exist in the hierarchy or if the parent group was not found.
     */
    virtual bool addGroupModel(const QString &parentGroupUniquelName,
                       const QString &uniqueName,
                       CT_AbstractItemGroup *group = new CT_StandardItemGroup(),
                       const QString &displayableName = "",
                       const QString &description = "");

    virtual bool addGroupModel(const QString &parentGroupUniqueName,
                               DEF_CT_AbstractGroupModelOut *groupModel);


    /**
     * @brief Add a group model to another group model in the result. SPECIFY the type of model to use with a template
     *
     * @param parentGroupUniqueName : the unique name of the model of the parent group (typically DEF_....)
     * @param uniqueName : unique name of the new group model (typically DEF_....)
     * @param group : the ItemDrawable that represent the group (typically a CT_StandardItemGroup, example : new CT_TTreeGroup())
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the group model
     *
     * @return true if the group was added to the parent group. false if the unique name already exist in the hierarchy or if the parent group was not found.
     */
    template<typename GROUP_MODEL, typename GROUP>
    bool addGroupModelT(const QString &parentGroupUniqueName,
                        const QString &uniqueName,
                        GROUP *group = new GROUP(),
                        const QString &displayableName = "",
                        const QString &description = "")
    {
        DEF_CT_AbstractGroupModelOut *rootModel = dynamic_cast<DEF_CT_AbstractGroupModelOut*>(this->findModelInTree(parentGroupUniqueName));

        if(rootModel == NULL)
            return false;

        GROUP_MODEL *newModel = new GROUP_MODEL(uniqueName, group, displayableName, description);

        if(rootModel->addGroup(newModel))
            return true;

        return false;
    }

    /**
     * @brief Add a ItemDrawable model to a group model in the result.
     *
     * @param parentGroupUniquelName : the unique name of the model of the parent group (typically DEF_....)
     * @param uniqueName : unique name of the new group model (typically DEF_....)
     * @param item : the ItemDrawable that represent the item (example : new CT_Scene())
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the item model
     *
     * @return true if the item was added to the parent group. false if the unique name already exist in the hierarchy or if the parent group was not found.
     */
    virtual bool addItemModel(const QString &parentGroupUniqueName,
                      const QString &uniqueName,
                      CT_AbstractSingularItemDrawable *item,
                      const QString &displayableName = "",
                      const QString &description = "");

    /**
     * @brief Add a ItemAttribute model to a group model in the result.
     *
     * @param parentItemUniqueName : the unique name of the model of the parent group (typically DEF_....)
     * @param uniqueName : unique name of the new group model (typically DEF_....)
     * @param ia : the ItemAttribute that represent the item attribute (example : new CT_StdItemAttributeT<int>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(CT_AbstractCategory::DATA_ANGLE), NULL, 0))
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the item model
     *
     * @return true if the item attribute was added to the parent item. false if the unique name already exist in the hierarchy or if the parent item was not found.
     */
    virtual bool addItemAttributeModel(const QString &parentItemUniqueName,
                               const QString &uniqueName,
                               CT_AbstractItemAttribute *ia,
                               const QString &displayableName = "",
                               const QString &description = "");

    // INHERITED FROM CT_OutAbstractResultModel
    virtual CT_AbstractResult* createResult(CT_ResultInitializeData &data) const;

    /**
     * @brief Returns a copy of this model
     */
    virtual CT_OutAbstractModel* copy() const;

private:

    DEF_CT_AbstractGroupModelOut                *_group;
};

#endif // CT_OUTRESULTMODELGROUP_H
