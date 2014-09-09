#include "ct_inabstractitemattributemodel.h"

CT_InAbstractItemAttributeModel::CT_InAbstractItemAttributeModel(const QString &uniqueName,
                                                                 const QList<QString> &categoriesType,
                                                                 CT_AbstractCategory::ValueType valueType,
                                                                 const QString &description,
                                                                 const QString &displayableName) : DEF_CT_InAbstractIAModel(uniqueName,
                                                                                                                            description,
                                                                                                                            displayableName)
{
    m_categoriesType = categoriesType;
    m_vType = valueType;
}

QList<QString> CT_InAbstractItemAttributeModel::categoriesType() const
{
    return m_categoriesType;
}

CT_AbstractCategory::ValueType CT_InAbstractItemAttributeModel::valueType() const
{
    return m_vType;
}
