#ifndef CT_STDITEMATTRIBUTECOORDINATET_HPP
#define CT_STDITEMATTRIBUTECOORDINATET_HPP

#include "ct_attributes/ct_stditemattributecoordinatet.h"

template <typename VType>
CT_StdItemAttributeCoordinateT<VType>::CT_StdItemAttributeCoordinateT(const CT_OutAbstractItemAttributeModel *model,
                                                                      const CT_AbstractCategory *category,
                                                                      const CT_AbstractResult *result,
                                                                      const VType &dataX,
                                                                      const VType &dataY,
                                                                      const VType &dataZ) : CT_StdItemAttributeT<VType>(model,
                                                                                                                        category,
                                                                                                                        result,
                                                                                                                        dataX)
{
    m_dataY = dataY;
    m_dataZ = dataZ;

    CT_AbstractCategory *cat = this->category();

    if(cat->isEquivalentTo(CT_AbstractCategory::staticInitDataX()))
        m_convert = CT_AbstractCoordinateSystem::CONVERT_X;
    else if(cat->isEquivalentTo(CT_AbstractCategory::staticInitDataY()))
        m_convert = CT_AbstractCoordinateSystem::CONVERT_Y;
    else
        m_convert = CT_AbstractCoordinateSystem::CONVERT_Z;
}

template <typename VType>
CT_StdItemAttributeCoordinateT<VType>::CT_StdItemAttributeCoordinateT(const QString &modelName,
                                                                      const QString &categoryName,
                                                                      const CT_AbstractResult *result,
                                                                      const VType &dataX,
                                                                      const VType &dataY,
                                                                      const VType &dataZ) : CT_StdItemAttributeT<VType>(modelName,
                                                                                                                        categoryName,
                                                                                                                        result,
                                                                                                                        dataX)
{
    m_dataY = dataY;
    m_dataZ = dataZ;

    CT_AbstractCategory *cat = this->category();

    if(cat->isEquivalentTo(CT_AbstractCategory::staticInitDataX()))
        m_convert = CT_AbstractCoordinateSystem::CONVERT_X;
    else if(cat->isEquivalentTo(CT_AbstractCategory::staticInitDataY()))
        m_convert = CT_AbstractCoordinateSystem::CONVERT_Y;
    else
        m_convert = CT_AbstractCoordinateSystem::CONVERT_Z;
}

template <typename VType>
CT_StdItemAttributeCoordinateT<VType>::CT_StdItemAttributeCoordinateT(const QString &categoryName) : CT_StdItemAttributeT<VType>(categoryName)
{
    m_dataY = 0;
    m_dataZ = 0;
    m_convert = 0;
}

template <typename VType>
CT_AbstractItemAttribute* CT_StdItemAttributeCoordinateT<VType>::copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result)
{
    return new CT_StdItemAttributeCoordinateT<VType>(model, this->category(), result, m_data, m_dataY, m_dataZ);
}

template <typename VType>
VType CT_StdItemAttributeCoordinateT<VType>::data(const CT_AbstractItemDrawable *item) const
{
    Q_UNUSED(item)

    if(this->useCoordinateSystem())
    {
        CT_AbstractCoordinateSystem::realEx xc, yc, zc;

        PS_COODINATES_SYS->convertExport(m_data,
                                         m_dataY,
                                         m_dataZ,
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
        return m_data;
    else if(m_convert == CT_AbstractCoordinateSystem::CONVERT_Y)
        return m_dataY;

    return m_dataZ;
}

#endif // CT_STDITEMATTRIBUTECOORDINATET_HPP
