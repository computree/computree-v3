#ifndef CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_HPP
#define CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_HPP

#include "ct_attributes/ct_stditemattributecoordinatewrappert.h"

template <class ItemDrawableClass, typename VType>
CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::CT_StdItemAttributeCoordinateWrapperT(const CT_OutAbstractItemAttributeModel *model,
                                                                                                      const CT_AbstractCategory *category,
                                                                                                      const CT_AbstractResult *result,
                                                                                                      getter gMethodX,
                                                                                                      getter gMethodY,
                                                                                                      getter gMethodZ) : CT_StdItemAttributeWrapperT<ItemDrawableClass,VType>(model,
                                                                                                                                                                              category,
                                                                                                                                                                              result,
                                                                                                                                                                              gMethodX)
{
    m_gMethodY = gMethodY;
    m_gMethodZ = gMethodZ;

    CT_AbstractCategory *cat = this->category();

    if(cat->isEquivalentTo(CT_AbstractCategory::staticInitDataX()))
        m_convert = CT_AbstractCoordinateSystem::CONVERT_X;
    else if(cat->isEquivalentTo(CT_AbstractCategory::staticInitDataY()))
        m_convert = CT_AbstractCoordinateSystem::CONVERT_Y;
    else
        m_convert = CT_AbstractCoordinateSystem::CONVERT_Z;
}

template <class ItemDrawableClass, typename VType>
CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::CT_StdItemAttributeCoordinateWrapperT(const QString &modelName,
                                                                                                      const QString &categoryName,
                                                                                                      const CT_AbstractResult *result,
                                                                                                      getter gMethodX,
                                                                                                      getter gMethodY,
                                                                                                      getter gMethodZ) : CT_StdItemAttributeWrapperT<ItemDrawableClass,VType>(modelName,
                                                                                                                                                                              categoryName,
                                                                                                                                                                              result,
                                                                                                                                                                              gMethodX)
{
    m_gMethodY = gMethodY;
    m_gMethodZ = gMethodZ;

    CT_AbstractCategory *cat = this->category();

    if(cat->isEquivalentTo(CT_AbstractCategory::staticInitDataX()))
        m_convert = CT_AbstractCoordinateSystem::CONVERT_X;
    else if(cat->isEquivalentTo(CT_AbstractCategory::staticInitDataY()))
        m_convert = CT_AbstractCoordinateSystem::CONVERT_Y;
    else
        m_convert = CT_AbstractCoordinateSystem::CONVERT_Z;
}

template <class ItemDrawableClass, typename VType>
CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::CT_StdItemAttributeCoordinateWrapperT(const QString &categoryName) : CT_StdItemAttributeWrapperT<ItemDrawableClass,VType>(categoryName)
{
    m_gMethodY = NULL;
    m_gMethodZ = NULL;
    m_convert = 0;
}

template <class ItemDrawableClass, typename VType>
CT_AbstractItemAttribute* CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result)
{
    return new CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>(model, this->category(), result, m_gMethod, m_gMethodY, m_gMethodZ);
}

template <class ItemDrawableClass, typename VType>
VType CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass,VType>::data(const CT_AbstractItemDrawable *item) const
{
    if(this->useCoordinateSystem())
    {
        CT_AbstractCoordinateSystem::realEx xc, yc, zc;
        CT_AbstractCoordinateSystem::realIm z = 0;

        if(m_gMethodZ != NULL)
            z = (((ItemDrawableClass*)item)->*m_gMethodZ)();

        PS_COODINATES_SYS->convertExport((((ItemDrawableClass*)item)->*m_gMethod)(),
                                         (((ItemDrawableClass*)item)->*m_gMethodY)(),
                                         z,
                                         xc,
                                         yc,
                                         zc,
                                         m_convert);

        if(m_convert == CT_AbstractCoordinateSystem::CONVERT_X)
            return xc;
        else if(m_convert == CT_AbstractCoordinateSystem::CONVERT_Y)
            return yc;

        return zc;
    }

    if(m_convert == CT_AbstractCoordinateSystem::CONVERT_X)
        return (((ItemDrawableClass*)item)->*m_gMethod)();
    else if(m_convert == CT_AbstractCoordinateSystem::CONVERT_Y)
        return (((ItemDrawableClass*)item)->*m_gMethodY)();

    return (((ItemDrawableClass*)item)->*m_gMethodZ)();
}

#endif // CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_HPP
