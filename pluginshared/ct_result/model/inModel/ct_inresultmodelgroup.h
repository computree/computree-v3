#ifndef CT_INRESULTMODELGROUP_H
#define CT_INRESULTMODELGROUP_H

#include "ct_result/model/inModel/abstract/ct_inabstractresultmodel.h"
#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.h"
#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"

class CT_OutAbstractResultModel;

/**
 * @brief A model that represent a CT_ResultGroup that you want to search
 */
class PLUGINSHAREDSHARED_EXPORT CT_InResultModelGroup : public CT_InAbstractResultModel
{
public:
    /**
     * @brief Constructor
     * @param uniqueName : the unique name (in the hierarchy) of the model. Typically a DEF_...
     * @param rootGroupModel : the root model that represent a group.
     * @param displayableName : a displayable name for this model. If empty by default used the unique name. Empty by default.
     * @param description : a description of the result you search. If empty by default used the displayable name. Empty by default.
     * @param recursive : true if you want this model research recusively results in all step in the tree (go up in hierarchy) or false
     *                    if you want just research in results of the step passed in parameter of the method "searchResultsInStep...". False by default.
     */
    CT_InResultModelGroup(const QString &uniqueName,
                          CT_InAbstractGroupModel *rootGroupModel,
                          const QString &displayableName = "",
                          const QString &description = "",
                          bool recursive = false);
    ~CT_InResultModelGroup();

    /**
     * @brief Returns the model type in a displayable QString (for gui)
     */
    virtual QString modelTypeDisplayable() const;

    /**
     * @brief Returns the root model that represent a group
     */
    CT_InAbstractGroupModel* rootGroup() const;

    /**
     * @brief Define a custom root model (delete the last root model from memory if exist).
     * @return true if the group was set to the result.
     */
    virtual bool setRootGroup(CT_InAbstractGroupModel *rootGroup);

    /**
     * @brief Define a CT_InZeroOrMoreGroupModel to be the root model (delete the last root model from memory if exist).
     * @return true if the group was set to the result.
     */
    virtual bool setZeroOrMoreRootGroup();

    /**
     * @brief Define a CT_InStdGroupModel to be the root model (delete the last root model from memory if exist).
     * @param uniqueName : unique name of the model (typically DEF_....)
     * @param displayableName : displayable name that will be show in gui
     * @param description : description of the group you search
     * @param choiceMode : the choice mode for this group
     * @param finderMode : the finder mode for this group
     * @return true if the group was set to the result.
     */
    virtual bool setRootGroup(const QString &uniqueName,
                              const QString &displayableName = "Groupe",
                              const QString &description = "",
                              CT_InAbstractGroupModel::GroupChoiceMode choiceMode = CT_InAbstractGroupModel::CG_ChooseMultipleIfMultiple,
                              CT_InAbstractGroupModel::GroupFinderMode finderMode = CT_InAbstractGroupModel::FG_IsObligatory);

    /**
     * @brief Add a CT_InStdGroupModel to another model (that represent a group) in the result.
     *
     * @param parentGroupUniqueName : the unique name of the model of the parent group (typically DEF_....). Set empty if you want to add group to the rootGroup !
     * @param uniqueName : unique name of the new group model (typically DEF_....)
     * @param groupType : the type of the group (CT_GroupXXX::staticGetType). The group can be abstract if you want to be generic. MUST NOT BE EMPTY !
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the group model
     * @param choiceMode : user selection mode in the input result configuration dialog.
     * @param finderMode : if group is obligatory or not. By default it must.
     *
     * @return true if the group was added to the parent group. false if the unique name already exist in the hierarchy or if the parent group was not found.
     */
    virtual bool addGroupModel(const QString &parentGroupUniqueName,
                               const QString &uniqueName,
                               const QString &groupType = CT_AbstractItemGroup::staticGetType(),
                               const QString &displayableName = "Groupe",
                               const QString &description = "",
                               CT_InAbstractGroupModel::GroupChoiceMode choiceMode = CT_InAbstractGroupModel::CG_ChooseMultipleIfMultiple,
                               CT_InAbstractGroupModel::GroupFinderMode finderMode = CT_InAbstractGroupModel::FG_IsObligatory);

    /**
     * @brief Add a CT_InStdSingularItemModel to another model (that represent a group) in the result.
     *
     * @param parentGroupUniqueName : the unique name of the model of the parent group (typically DEF_....). Set empty if you want to add item to the rootGroup !
     * @param uniqueName : unique name of the new group model (typically DEF_....)
     * @param itemType : the result of the method staticGetType of the ItemDrawable you want to search (example : CT_Scene::staticGetType() or CT_AbstractSingularItemDrawable::staticGetType())
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the group model
     * @param choiceMode : user selection mode in the input result configuration dialog. By default the user can choose only one itemdrawable type if multiple was founded.
     * @param finderMode : if item is obligatory or not. By default it must.
     *
     * @return true if the item was added to the parent group. false if the unique name already exist in the hierarchy or if the parent group was not found.
     */
    virtual bool addItemModel(const QString &parentGroupUniqueName,
                              const QString &uniqueName,
                              const QString &itemType,
                              const QString &displayableName = "",
                              const QString &description = "",
                              CT_InAbstractModel::ChoiceMode choiceMode = CT_InAbstractModel::C_ChooseOneIfMultiple,
                              CT_InAbstractModel::FinderMode finderMode = CT_InAbstractModel::F_IsObligatory);

    /**
     * @brief Add a CT_InStdGroupModel to another model (that represent a group) in the result.
     *
     * @param parentGroupUniqueName : the unique name of the model of the parent group (typically DEF_....). Set empty if you want to add group to the rootGroup !
     * @param autoUniqueName : object that automatically renames the model to avoid the same uniqueName exists in the hierarchy
     * @param groupType : the type of the group (CT_GroupXXX::staticGetType). The group can be abstract if you want to be generic. MUST NOT BE EMPTY !
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the group model
     * @param choiceMode : user selection mode in the input result configuration dialog. By default we choose only itemdrawable so we doesn't choose group.
     * @param finderMode : if group is obligatory or not. By default it must.
     *
     * @return true if the group was added to the parent group. false if the unique name already exist in the hierarchy or if the parent group was not found.
     */
    bool addGroupModel(const QString &parentGroupUniqueName,
                       CT_AutoRenameModels &autoUniqueName,
                       const QString &groupType = CT_AbstractItemGroup::staticGetType(),
                       const QString &displayableName = "Groupe",
                       const QString &description = "",
                       CT_InAbstractGroupModel::GroupChoiceMode choiceMode = CT_InAbstractGroupModel::CG_ChooseMultipleIfMultiple,
                       CT_InAbstractGroupModel::GroupFinderMode finderMode = CT_InAbstractGroupModel::FG_IsObligatory);

    /**
     * @brief Add a CT_InStdSingularItemModel to another model (that represent a group) in the result.
     *
     * @param parentGroupUniqueName : the unique name of the model of the parent group (typically DEF_....). Set empty if you want to add item to the rootGroup !
     * @param autoUniqueName : object that automatically renames the model to avoid the same uniqueName exists in the hierarchy
     * @param itemType : the result of the method staticGetType of the ItemDrawable you want to search (example : CT_Scene::staticGetType() or CT_AbstractSingularItemDrawable::staticGetType())
     * @param displayableName: displayable name that will be show in gui
     * @param description : description of the group model
     * @param choiceMode : user selection mode in the input result configuration dialog. By default the user can choose only one itemdrawable type if multiple was founded.
     * @param finderMode : if item is obligatory or not. By default it must.
     *
     * @return true if the item was added to the parent group. false if the unique name already exist in the hierarchy or if the parent group was not found.
     */
    bool addItemModel(const QString &parentGroupUniqueName,
                      CT_AutoRenameModels &autoUniqueName,
                      const QString &itemType,
                      const QString &displayableName = "",
                      const QString &description = "",
                      CT_InAbstractModel::ChoiceMode choiceMode = CT_InAbstractModel::C_ChooseOneIfMultiple,
                      CT_InAbstractModel::FinderMode finderMode = CT_InAbstractModel::F_IsObligatory);

    // INHERITED FROM CT_AbstractModel //
    QList<CT_AbstractModel*> childrens() const;
    bool isEmpty() const;
    //                  //

    /**
     * @brief Search real results corresponding to selected possibilities from the step passed in parameter
     *        and recursively (go up in tree) if the parameter "isRecursive()" returns true.
     * @return An empty list if one of the model in the list of possibilities selected was not found or if there was no possibility selected.
     */
    QList<CT_AbstractResult*> searchResultsInStepForPossibilitiesSelected(const CT_VirtualAbstractStep *step) const;

    /**
     * @brief Search real results corresponding to the possibility passed in parameter from the step passed in parameter
     *        and recursively (go up in tree) if the parameter "isRecursive()" returns true.
     * @return NULL if the result was not found or if the possibility passed in parameter is not from this model
     */
    CT_AbstractResult* searchResultsInStepForThisPossibility(const CT_VirtualAbstractStep *step,
                                                             const CT_InStdModelPossibility *possibility) const;

    /**
     * @brief Returns all values to saved a model to restore them later with the method "setAllValues"
     */
    virtual QList<SettingsNodeGroup*> getAllValues() const;

    /**
     * @brief Restore this model with values previously saved by the method "getAllValues"
     */
    virtual bool setAllValues(const QList<SettingsNodeGroup*> &list);

    /**
     * @brief Copy this model with possibilities if the parameter is set to true
     */
    virtual CT_InAbstractModel* copy(bool withPossibilities) const;

    // INHERITED FROM CT_InAbstractModel
    /**
     * @brief Return true if this model can be compared with the OUTPUT model passed in parameter
     */
    virtual bool canBeComparedWith(const CT_OutAbstractModel &model) const;
    //      //

private:
    CT_InAbstractGroupModel             *_group;

protected:

    // INHERITED FROM CT_InAbstractModel
    /**
     * @brief Must return true if the model passed in parameter can be added in possibilities
     */
    virtual bool canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const;
    //      //

    /**
     * @brief Search real results corresponding to the OUTPUT model from the step passed in parameter
     *        and recursively (go up in tree of step).
     * @return NULL if result was not found.
     */
    CT_AbstractResult* recursiveSearchResultFromStepForModel(const CT_VirtualAbstractStep *step, const CT_OutAbstractResultModel *model) const;

    /**
     * @brief Search real results corresponding to the OUTPUT model passed in parameter in the step passed in parameter.
     * @return NULL if the result was not found or if the OUTPUT model was not created by the step passed in parameter.
     */
    CT_AbstractResult* searchResultInStepForModel(const CT_VirtualAbstractStep *step, const CT_OutAbstractResultModel *model) const;
};

#endif // CT_INRESULTMODELGROUP_H
