#ifndef CT_STDITEMATTRIBUTEWRAPPERT_HPP
#define CT_STDITEMATTRIBUTEWRAPPERT_HPP

#include "ct_attributes/ct_stditemattributewrappert.h"

template <class ItemDrawableClass, typename VType>
CT_StdItemAttributeWrapperT<ItemDrawableClass,VType>::CT_StdItemAttributeWrapperT(const CT_OutAbstractItemAttributeModel *model,
                                                                                  const CT_AbstractCategory *category,
                                                                                  const CT_AbstractResult *result,
                                                                                  getter gMethod) : CT_AbstractItemAttributeT<VType>(model,
                                                                                                                                     category,
                                                                                                                                     result)
{
    m_gMethod = gMethod;
}

template <class ItemDrawableClass, typename VType>
CT_StdItemAttributeWrapperT<ItemDrawableClass,VType>::CT_StdItemAttributeWrapperT(const QString &modelName,
                                                                                  const QString &categoryName,
                                                                                  const CT_AbstractResult *result,
                                                                                  getter gMethod) : CT_AbstractItemAttributeT<VType>(modelName,
                                                                                                                                     categoryName,
                                                                                                                                     result)
{
    m_gMethod = gMethod;
}

template <class ItemDrawableClass, typename VType>
CT_AbstractItemAttribute* CT_StdItemAttributeWrapperT<ItemDrawableClass,VType>::copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result)
{
    return new CT_StdItemAttributeWrapperT<ItemDrawableClass,VType>(model, this->category(), result, m_gMethod);
}

template <class ItemDrawableClass, typename VType>
VType CT_StdItemAttributeWrapperT<ItemDrawableClass,VType>::data(const CT_AbstractItemDrawable *item) const
{
    return (((ItemDrawableClass*)item)->*m_gMethod)();
}

#endif // CT_STDITEMATTRIBUTEWRAPPERT_HPP
