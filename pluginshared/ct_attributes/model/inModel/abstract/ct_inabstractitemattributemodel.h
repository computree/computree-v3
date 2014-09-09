#ifndef CT_INABSTRACTITEMATTRIBUTEMODEL_H
#define CT_INABSTRACTITEMATTRIBUTEMODEL_H

#include "ct_attributes/model/inModel/abstract/def_ct_inabstractitemattributemodel.h"
#include "ct_categories/abstract/ct_abstractcategory.h"

class PLUGINSHAREDSHARED_EXPORT CT_InAbstractItemAttributeModel : public DEF_CT_InAbstractIAModel
{
public:
    /**
     * @brief Construct a model that represent a item attribute
     * @param uniqueName : a unique name in all the hierarchy
     * @param categoriesType : a list of categories type (per example QList<...>() << CT_AbstractCategory::DATA_X_DIMENSION << CT_AbstractCategory::DATA_X_RESOLUTION). The
     *                         list will be used with the logical operator 'OR'. Item attribute with a category type in the list will be accepted if value type is ok.
     * @param valueType : pass the value type that you want (per example CT_AbstractCategory::NUMBER if you want
     *                    to be generic and accept int, double, float, etc... or CT_AbstractCategory::UINT64 for a specific type)
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_InAbstractItemAttributeModel(const QString &uniqueName,
                                    const QList<QString> &categoriesType,
                                    CT_AbstractCategory::ValueType valueType,
                                    const QString &description,
                                    const QString &displayableName);

    /**
     * @brief Returns categories type used by this model
     */
    QList<QString> categoriesType() const;

    /**
     * @brief Returns value type of categories
     */
    CT_AbstractCategory::ValueType valueType() const;

private:
    QList<QString>                  m_categoriesType;
    CT_AbstractCategory::ValueType  m_vType;
};

#endif // CT_INABSTRACTITEMATTRIBUTEMODEL_H
