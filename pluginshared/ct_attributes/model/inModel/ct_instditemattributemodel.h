#ifndef CT_INSTDITEMATTRIBUTEMODEL_H
#define CT_INSTDITEMATTRIBUTEMODEL_H

#include "ct_attributes/model/inModel/abstract/ct_inabstractitemattributemodel.h"

/**
 * @brief Use this class if you want to search a item attribute in a OUTPUT model
 */
class PLUGINSHAREDSHARED_EXPORT CT_InStdItemAttributeModel : public CT_InAbstractItemAttributeModel
{
public:
    /**
     * @brief Construct a model that represent a item attribute
     * @param uniqueName : a unique name in all the hierarchy
     * @param categoryType : the category type (per example CT_AbstractCategory::DATA_ANGLE)
     * @param valueType : pass the value type that you want (per example CT_AbstractCategory::NUMBER if you want
     *                    to be generic and accept int, double, float, etc... or CT_AbstractCategory::UINT64 for a specific type)
     * @param displayableName : a displayable name
     * @param description : a description
     */
    CT_InStdItemAttributeModel(const QString &uniqueName,
                               const QString &categoryType,
                               CT_AbstractCategory::ValueType valueType,
                               const QString &displayableName = "",
                               const QString &description = "",
                               CT_InStdItemAttributeModel::ChoiceMode choiceMode = CT_InStdItemAttributeModel::C_ChooseOneIfMultiple,
                               CT_InStdItemAttributeModel::FinderMode finderMode = CT_InStdItemAttributeModel::F_IsObligatory);

    /**
     * @brief Construct a model that represent a item attribute
     * @param uniqueName : a unique name in all the hierarchy
     * @param categoriesType : a list of categories type (per example QList<...>() << CT_AbstractCategory::DATA_X_DIMENSION << CT_AbstractCategory::DATA_X_RESOLUTION). The
     *                         list will be used with the logical operator 'OR'. Item attribute with a category type in the list will be accepted if value type is ok.
     * @param valueType : pass the value type that you want (per example CT_AbstractCategory::NUMBER if you want
     *                    to be generic and accept int, double, float, etc... or CT_AbstractCategory::UINT64 for a specific type)
     * @param displayableName : a displayable name
     * @param description : a description
     */
    CT_InStdItemAttributeModel(const QString &uniqueName,
                               const QList<QString> &categoriesType,
                               CT_AbstractCategory::ValueType valueType,
                               const QString &displayableName = "",
                               const QString &description = "",
                               CT_InStdItemAttributeModel::ChoiceMode choiceMode = CT_InStdItemAttributeModel::C_ChooseOneIfMultiple,
                               CT_InStdItemAttributeModel::FinderMode finderMode = CT_InStdItemAttributeModel::F_IsObligatory);

    /**
     * @brief Returns the model type in a displayable QString (for gui)
     */
    virtual QString modelTypeDisplayable() const;

    /**
     * @brief Returns a copy of this model and its children.
     * @param withPossibilities : true if you want to copy possibilities in the copy of the model
     */
    virtual CT_InAbstractModel* copy(bool withPossibilities) const;

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

#endif // CT_INSTDITEMATTRIBUTEMODEL_H
