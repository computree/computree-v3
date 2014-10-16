#ifndef CT_STDITEMATTRIBUTET_HPP
#define CT_STDITEMATTRIBUTET_HPP

#include "ct_attributes/ct_stditemattributet.h"

template <typename VType>
CT_StdItemAttributeT<VType>::CT_StdItemAttributeT(const CT_OutAbstractItemAttributeModel *model,
                                                  const CT_AbstractCategory *category,
                                                  const CT_AbstractResult *result,
                                                  const VType &data) : CT_AbstractItemAttributeT<VType>(model,
                                                                                                        category,
                                                                                                        result)
{
    m_data = data;
}

template <typename VType>
CT_StdItemAttributeT<VType>::CT_StdItemAttributeT(const QString &modelName,
                                                  const QString &categoryName,
                                                  const CT_AbstractResult *result,
                                                  const VType &data) : CT_AbstractItemAttributeT<VType>(modelName,
                                                                                                        categoryName,
                                                                                                        result)
{
    m_data = data;
}

template <typename VType>
CT_StdItemAttributeT<VType>::CT_StdItemAttributeT(const QString &categoryName) : CT_AbstractItemAttributeT<VType>(categoryName)
{
    m_data = 0;
}

template <typename VType>
CT_AbstractItemAttribute* CT_StdItemAttributeT<VType>::copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result)
{
    return new CT_StdItemAttributeT<VType>(model, this->category(), result, m_data);
}

template <typename VType>
VType CT_StdItemAttributeT<VType>::data(const CT_AbstractItemDrawable *item) const
{
    Q_UNUSED(item)

    return m_data;
}

#endif // CT_STDITEMATTRIBUTET_HPP
