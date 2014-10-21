#ifndef CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_HPP
#define CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_HPP

#include "ct_attributes/ct_stditemattributecoordinatewrappert.h"

template <class ItemDrawableClass, typename VType>
CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::CT_StdItemAttributeCoordinateWrapperT(const CT_OutAbstractItemAttributeModel *model,
                                                                                                      const CT_AbstractCategory *category,
                                                                                                      const CT_AbstractResult *result,
                                                                                                      getter gMethodX,
                                                                                                      getter gMethodY,
                                                                                                      getter gMethodZ) : CT_AbstractItemAttributeT<VType>(model,
                                                                                                                                                          category,
                                                                                                                                                          result)
{
    m_gMethods[0] = gMethodX;
    m_gMethods[1] = gMethodY;
    m_gMethods[2] = gMethodZ;

    initConvertionVariable();
}

template <class ItemDrawableClass, typename VType>
CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::CT_StdItemAttributeCoordinateWrapperT(const QString &modelName,
                                                                                                      const QString &categoryName,
                                                                                                      const CT_AbstractResult *result,
                                                                                                      getter gMethodX,
                                                                                                      getter gMethodY,
                                                                                                      getter gMethodZ) : CT_AbstractItemAttributeT<VType>(modelName,
                                                                                                                                                          categoryName,
                                                                                                                                                          result)
{
    m_gMethods[0] = gMethodX;
    m_gMethods[1] = gMethodY;
    m_gMethods[2] = gMethodZ;

    initConvertionVariable();
}

template <class ItemDrawableClass, typename VType>
CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::CT_StdItemAttributeCoordinateWrapperT(const QString &categoryName) : CT_AbstractItemAttributeT<VType>(categoryName)
{
    m_gMethods[0] = NULL;
    m_gMethods[1] = NULL;
    m_gMethods[2] = NULL;
    m_convert = 0;
    m_index = 0;
}

template <class ItemDrawableClass, typename VType>
bool CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::canUseCoordinateSystem() const
{
    return true;
}

template <class ItemDrawableClass, typename VType>
CT_AbstractItemAttribute* CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result)
{
    return new CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>(model, this->category(), result, m_gMethods[0], m_gMethods[1], m_gMethods[2]);
}

template <class ItemDrawableClass, typename VType>
VType CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::data(const CT_AbstractItemDrawable *item) const
{
    return (((ItemDrawableClass*)item)->*m_gMethods[m_index])();
}

template <class ItemDrawableClass, typename VType>
CT_AbstractCoordinateSystem::realEx CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::dataConverted(const CT_AbstractItemDrawable *item) const
{
    CT_AbstractCoordinateSystem::realEx c[3];
    CT_AbstractCoordinateSystem::realIm z = 0;

    if(m_gMethods[2] != NULL)
        z = (((ItemDrawableClass*)item)->*m_gMethods[2])();

    PS_COORDINATES_SYS->convertExport((((ItemDrawableClass*)item)->*m_gMethods[0])(),
                                      (((ItemDrawableClass*)item)->*m_gMethods[1])(),
                                      z,
                                      c[0],
                                      c[1],
                                      c[2],
                                      m_convert);
    return c[m_index];
}

template <class ItemDrawableClass, typename VType>
void CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::initConvertionVariable()
{
    CT_AbstractCategory *cat = this->category();

    if(cat->isEquivalentTo(CT_AbstractCategory::staticInitDataX()))
    {
        m_convert = CT_AbstractCoordinateSystem::CONVERT_X;
        m_index = 0;
    }
    else if(cat->isEquivalentTo(CT_AbstractCategory::staticInitDataY()))
    {
        m_convert = CT_AbstractCoordinateSystem::CONVERT_Y;
        m_index = 1;
    }
    else
    {
        m_convert = CT_AbstractCoordinateSystem::CONVERT_Z;
        m_index = 2;
    }
}

#endif // CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_HPP
