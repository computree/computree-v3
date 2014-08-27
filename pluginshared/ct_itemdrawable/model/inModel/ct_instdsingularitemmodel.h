#ifndef CT_INSTDSINGULARITEMMODEL_H
#define CT_INSTDSINGULARITEMMODEL_H

#include "ct_itemdrawable/model/inModel/abstract/def_ct_abstractitemdrawablemodelin.h"

/**
 * @brief Use this class to search a singular item (generic or specific)
 */
class PLUGINSHAREDSHARED_EXPORT CT_InStdSingularItemModel : public DEF_CT_AbstractItemDrawableModelIn
{
public:

    /**
     * @brief Construct a model that search a singular item
     * @param uniqueName : a unique name in all the hierarchy
     * @param itemType : the type of the item (CT_ItemXXX::staticGetType). Item can be abstract if you want to be generic. MUST NOT BE EMPTY !
     * @param displayableName : a displayable name (name displayed in gui for user information). Empty by default.
     * @param description : a description (displayed in gui for user information). Empty by default.
     * @param choiceMode : type of selection you want. Choose only one by default.
     * @param finderMode : if you want that the item is optionnal or not. Obligatory by default.
     */
    CT_InStdSingularItemModel(const QString &uniqueName,
                              const QString &itemType,
                              const QString &displayableName = "",
                              const QString &description = "",
                              CT_InStdSingularItemModel::ChoiceMode choiceMode = CT_InStdSingularItemModel::C_ChooseOneIfMultiple,
                              CT_InStdSingularItemModel::FinderMode finderMode = CT_InStdSingularItemModel::F_IsObligatory);

    /**
     * @brief Returns the model type in a displayable QString (for gui)
     */
    virtual QString modelTypeDisplayable() const;

    CT_InAbstractModel* copy(bool withPossibilities) const;

    /**
     * @brief Return true if this model can be compared with the OUTPUT model passed in parameter
     */
    bool canBeComparedWith(const CT_OutAbstractModel &model) const;

protected:
    /**
     * @brief Must return true if the model passed in parameter can be added in possibilities
     */
    bool canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const;

private:

    /**
     * @brief ReCopy constructor (used in method "copy")
     */
    CT_InStdSingularItemModel(const CT_InStdSingularItemModel &model);
};

#endif // CT_INSTDSINGULARITEMMODEL_H
