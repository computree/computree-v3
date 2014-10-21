#ifndef CT_STDITEMATTRIBUTECOORDINATET_HPP
#define CT_STDITEMATTRIBUTECOORDINATET_HPP

#include "ct_attributes/ct_stditemattributecoordinatet.h"

template <typename VType>
CT_StdItemAttributeCoordinateT<VType>::CT_StdItemAttributeCoordinateT(const CT_OutAbstractItemAttributeModel *model,
                                                                      const CT_AbstractCategory *category,
                                                                      const CT_AbstractResult *result,
                                                                      const VType &dataX,
                                                                      const VType &dataY,
                                                                      const VType &dataZ) : CT_AbstractItemAttributeT<VType>(model,
                                                                                                                             category,
                                                                                                                             result)
{
    m_datas[0] = dataX;
    m_datas[1] = dataY;
    m_datas[2] = dataZ;

    initConvertionVariable();
}

template <typename VType>
CT_StdItemAttributeCoordinateT<VType>::CT_StdItemAttributeCoordinateT(const QString &modelName,
                                                                      const QString &categoryName,
                                                                      const CT_AbstractResult *result,
                                                                      const VType &dataX,
                                                                      const VType &dataY,
                                                                      const VType &dataZ) : CT_AbstractItemAttributeT<VType>(modelName,
                                                                                                                             categoryName,
                                                                                                                             result)
{
    m_datas[0] = dataX;
    m_datas[1] = dataY;
    m_datas[2] = dataZ;

    initConvertionVariable();
}

template <typename VType>
CT_StdItemAttributeCoordinateT<VType>::CT_StdItemAttributeCoordinateT(const QString &categoryName) : CT_AbstractItemAttributeT<VType>(categoryName)
{
    m_datas[0] = VType(0);
    m_datas[1] = VType(0);
    m_datas[2] = VType(0);
    m_convert = 0;
    m_index = 0;
}

template <typename VType>
bool CT_StdItemAttributeCoordinateT<VType>::canUseCoordinateSystem() const
{
    return true;
}

template <typename VType>
CT_AbstractItemAttribute* CT_StdItemAttributeCoordinateT<VType>::copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result)
{
    return new CT_StdItemAttributeCoordinateT<VType>(model, this->category(), result, m_datas[0], m_datas[1], m_datas[2]);
}

template <typename VType>
VType CT_StdItemAttributeCoordinateT<VType>::data(const CT_AbstractItemDrawable *item) const
{
    Q_UNUSED(item)

    return m_datas[m_index];
}

template <typename VType>
CT_AbstractCoordinateSystem::realEx CT_StdItemAttributeCoordinateT<VType>::dataConverted(const CT_AbstractItemDrawable *item) const
{
    Q_UNUSED(item)

    CT_AbstractCoordinateSystem::realEx c[3];

    PS_COORDINATES_SYS->convertExport(m_datas[0],
                                      m_datas[1],
                                      m_datas[2],
                                      c[0],
                                      c[1],
                                      c[2],
                                      m_convert);

    return c[m_index];
}

template <typename VType>
void CT_StdItemAttributeCoordinateT<VType>::initConvertionVariable()
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

#endif // CT_STDITEMATTRIBUTECOORDINATET_HPP
