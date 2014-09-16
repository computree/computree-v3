#ifndef CT_INSTDGROUPMODEL_H
#define CT_INSTDGROUPMODEL_H

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.h"
#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"
/*!
 *  Use this class to search a group. Add other model if you want to search a group with childrens.
 */
class PLUGINSHAREDSHARED_EXPORT CT_InStdGroupModel : public CT_InAbstractGroupModel
{
    Q_OBJECT

public:

    /**
     * @brief Construct a model that search a group
     * @param uniqueName : a unique name in all the hierarchy
     * @param groupType : the type of the group (CT_GroupXXX::staticGetType). The group can be abstract if you want to be generic. MUST NOT BE EMPTY !
     * @param displayableName : a displayable name (name displayed in gui for user information). "Groupe" by default.
     * @param description : a description (displayed in gui for user information). Empty by default.
     * @param choiceMode : type of selection you want. Choose multiple by default.
     * @param finderMode : if you want that the group is optionnal or not. Obligatory by default.
     */
    CT_InStdGroupModel(const QString &uniqueName,
                       const QString &groupType = CT_AbstractItemGroup::staticGetType(),
                       const QString &displayableName = "Groupe",
                       const QString &description = "",
                       CT_InAbstractGroupModel::GroupChoiceMode choiceMode = CT_InAbstractGroupModel::CG_ChooseMultipleIfMultiple,
                       CT_InAbstractGroupModel::GroupFinderMode finderMode = CT_InAbstractGroupModel::FG_IsObligatory);


    /**
     * @brief Returns the model type in a displayable QString (for gui)
     */
    virtual QString modelTypeDisplayable() const;

    /**
     * @brief Copy this model
     */
    virtual CT_InAbstractModel* copy(bool withPossibilities) const;

    // INHERITED FROM CT_AbstractGroupModelT //
    virtual DEF_CT_AbstractGroupModelIn* copyGroup() const { return (DEF_CT_AbstractGroupModelIn*)copy(true); }

    /**
     * @brief Return true if this model can be compared with the OUTPUT model passed in parameter
     */
    virtual bool canBeComparedWith(const CT_OutAbstractModel &model) const;

protected:
    /**
     * @brief Must return true if the model passed in parameter can be added in possibilities
     */
    virtual bool canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const;
};

#endif // CT_INSTDGROUPMODEL_H
