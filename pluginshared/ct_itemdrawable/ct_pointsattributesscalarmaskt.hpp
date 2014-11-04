#ifndef CT_POINTSATTRIBUTESSCALARMASKT_HPP
#define CT_POINTSATTRIBUTESSCALARMASKT_HPP

#include "ct_itemdrawable/ct_pointsattributesscalarmaskt.h"

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::CT_PointsAttributesScalarMaskT() : CT_AbstractPointAttributesScalar(), CT_AbstractAttributesScalar()
{
    m_collection = NULL;
    m_autoDeleteCollection = true;
    m_mask = StructMASK(0);
    m_shiftRight = 0;
    m_min = StructMASK(0);
    m_max = StructMASK(0);
}

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::CT_PointsAttributesScalarMaskT(const CT_OutAbstractSingularItemModel *model,
                                                                           const CT_AbstractResult *result,
                                                                           StructMASK mask,
                                                                           quint16 shiftRight,
                                                                           CT_StandardCloudStdVectorT<StructType> *collection,
                                                                           CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir,
                                                                           bool autoDeleteCollection) : CT_AbstractPointAttributesScalar(model,
                                                                                                                                         result,
                                                                                                                                         pcir),
                                                                                                        CT_AbstractAttributesScalar()
{
    m_collection = collection;
    m_autoDeleteCollection = autoDeleteCollection;
    m_mask = mask;
    m_shiftRight = shiftRight;

    std::pair< typename std::vector<StructType>::iterator, typename std::vector<StructType>::iterator > res = CT_Math::minmax_element(m_collection->begin(),
                                                                                                                                      m_collection->end(),
                                                                                                                                      (void*)this,
                                                                                                                                      CT_PointsAttributesScalarMaskT<StructType>::staticCompareStruct);

    m_min = (((*res.first).entire & m_mask) >> m_shiftRight);
    m_max = (((*res.second).entire & m_mask) >> m_shiftRight);
}

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::CT_PointsAttributesScalarMaskT(const QString &modelName,
                                                                           const CT_AbstractResult *result,
                                                                           StructMASK mask,
                                                                           quint16 shiftRight,
                                                                           CT_StandardCloudStdVectorT<StructType> *collection,
                                                                           CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir,
                                                                           bool autoDeleteCollection) : CT_AbstractPointAttributesScalar(modelName,
                                                                                                                                         result,
                                                                                                                                         pcir),
                                                                                                        CT_AbstractAttributesScalar()
{
    m_collection = collection;
    m_autoDeleteCollection = autoDeleteCollection;
    m_mask = mask;
    m_shiftRight = shiftRight;

    std::pair< typename std::vector<StructType>::iterator, typename std::vector<StructType>::iterator > res = CT_Math::minmax_element(m_manager->collection()->begin(),
                                                                                                                                      m_manager->collection()->end(),
                                                                                                                                      (void*)this,
                                                                                                                                      CT_PointsAttributesScalarMaskT<StructType>::staticCompareStruct);

    m_min = (((*res.first).entire & m_mask) >> m_shiftRight);
    m_max = (((*res.second).entire & m_mask) >> m_shiftRight);
}

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::CT_PointsAttributesScalarMaskT(const CT_OutAbstractSingularItemModel *model,
                                                                           const CT_AbstractResult *result,
                                                                           StructMASK mask,
                                                                           quint16 shiftRight,
                                                                           const StructMASK &min,
                                                                           const StructMASK &max,
                                                                           CT_StandardCloudStdVectorT<StructType> *collection,
                                                                           CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir,
                                                                           bool autoDeleteCollection) : CT_AbstractPointAttributesScalar(model,
                                                                                                                                         result,
                                                                                                                                         pcir),
                                                                                                        CT_AbstractAttributesScalar()
{
    m_collection = collection;
    m_autoDeleteCollection = autoDeleteCollection;
    m_mask = mask;
    m_shiftRight = shiftRight;

    m_min = min;
    m_max = max;
}

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::CT_PointsAttributesScalarMaskT(const QString &modelName,
                                                                           const CT_AbstractResult *result,
                                                                           StructMASK mask,
                                                                           quint16 shiftRight,
                                                                           const StructMASK &min,
                                                                           const StructMASK &max,
                                                                           CT_StandardCloudStdVectorT<StructType> *collection,
                                                                           CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir,
                                                                           bool autoDeleteCollection) : CT_AbstractPointAttributesScalar(modelName,
                                                                                                                                         result,
                                                                                                                                         pcir),
                                                                                                        CT_AbstractAttributesScalar()
{
    m_collection = collection;
    m_autoDeleteCollection = autoDeleteCollection;
    m_mask = mask;
    m_shiftRight = shiftRight;

    m_min = min;
    m_max = max;
}


template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::~CT_PointsAttributesScalarMaskT()
{
    if(m_autoDeleteCollection)
        delete m_collection;
}

template<typename StructType>
QString CT_PointsAttributesScalarMaskT<StructType>::getType() const
{
    return staticGetType();
}

template<typename StructType>
QString CT_PointsAttributesScalarMaskT<StructType>::staticGetType()
{
    return CT_AbstractPointAttributesScalar::staticGetType() + "/CT_PointsAttributesScalarMaskT<" + QString(typeid(StructType).name()) + ">";
}

template<typename StructType>
double CT_PointsAttributesScalarMaskT<StructType>::dMin() const
{
    return m_min;
}

template<typename StructType>
double CT_PointsAttributesScalarMaskT<StructType>::dMax() const
{
    return m_max;
}

template<typename StructType>
double CT_PointsAttributesScalarMaskT<StructType>::dValueAt(const size_t &index) const
{
    return ((m_collection->tAt(index).entire & m_mask) >> m_shiftRight);
}

template<typename StructType>
size_t CT_PointsAttributesScalarMaskT<StructType>::attributesSize() const
{
    return m_collection->size();
}

/*template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::StructMASK CT_PointsAttributesScalarMaskT<StructType>::min() const
{
    return m_min;
}

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::StructMASK CT_PointsAttributesScalarMaskT<StructType>::max() const
{
    return m_max;
}

template<typename StructType>
void CT_PointsAttributesScalarMaskT<StructType>::setMin(const StructMASK &min)
{
    m_min = min;
}

template<typename StructType>
void CT_PointsAttributesScalarMaskT<StructType>::setMax(const StructMASK &max)
{
    m_max = max;
}*/

template<typename StructType>
CT_StandardCloudStdVectorT<StructType>* CT_PointsAttributesScalarMaskT<StructType>::collection() const
{
    return m_collection;
}

template<typename StructType>
CT_AbstractItemDrawable* CT_PointsAttributesScalarMaskT<StructType>::copy(const CT_OutAbstractItemModel *model,
                                                                          const CT_AbstractResult *result,
                                                                          CT_ResultCopyModeList copyModeList)
{
    return new CT_PointsAttributesScalarMaskT<StructType>((const CT_OutAbstractSingularItemModel*)model,
                                                          result,
                                                          m_mask,
                                                          m_shiftRight,
                                                          m_min,
                                                          m_max,
                                                          m_collection,
                                                          getPointCloudIndexRegistered(),
                                                          false);
}

template<typename StructType>
bool CT_PointsAttributesScalarMaskT<StructType>::staticCompareStruct(const StructType &a, const StructType &b, void* context)
{
    return (a.entire & ((CT_PointsAttributesScalarMaskT<StructType>*)context)->m_mask) < (b.entire & ((CT_PointsAttributesScalarMaskT<StructType>*)context)->m_mask);
}

#endif // CT_POINTSATTRIBUTESSCALARMASKT_HPP
