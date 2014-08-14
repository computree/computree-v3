#ifndef CT_ATTRIBUTESSCALART_HPP
#define CT_ATTRIBUTESSCALART_HPP

#include "ct_attributes/ct_attributesscalart.h"

#include "ct_math/ct_math.h"
#include <limits>

template<typename SCALAR>
CT_AttributesScalarT<SCALAR>::CT_AttributesScalarT(const size_t &size)
{
    m_collection = new CT_StandardCloudStdVectorT<SCALAR>(size);

    initMinMax();
}

template<typename SCALAR>
CT_AttributesScalarT<SCALAR>::CT_AttributesScalarT(CT_StandardCloudStdVectorT<SCALAR> *collection)
{
    m_collection = collection;

    std::pair< typename std::vector<SCALAR>::iterator, typename std::vector<SCALAR>::iterator > res = CT_Math::minmax_element(m_collection->begin(), m_collection->end());

    m_min = *res.first;
    m_max = *res.second;
}

template<typename SCALAR>
CT_AttributesScalarT<SCALAR>::CT_AttributesScalarT(CT_StandardCloudStdVectorT<SCALAR> *collection, const SCALAR &min, const SCALAR &max)
{
    m_collection = collection;
    m_min = min;
    m_max = max;
}

template<typename SCALAR>
CT_AttributesScalarT<SCALAR>::~CT_AttributesScalarT()
{
    delete m_collection;
}

template<typename SCALAR>
double CT_AttributesScalarT<SCALAR>::dMin() const
{
    return min();
}

template<typename SCALAR>
double CT_AttributesScalarT<SCALAR>::dMax() const
{
    return max();
}

template<typename SCALAR>
double CT_AttributesScalarT<SCALAR>::dValueAt(const size_t &index) const
{
    return valueAt(index);
}

template<typename SCALAR>
SCALAR CT_AttributesScalarT<SCALAR>::min() const
{
    return m_min;
}

template<typename SCALAR>
SCALAR CT_AttributesScalarT<SCALAR>::max() const
{
    return m_max;
}

template<typename SCALAR>
void CT_AttributesScalarT<SCALAR>::setMin(const SCALAR &min)
{
    m_min = min;
}

template<typename SCALAR>
void CT_AttributesScalarT<SCALAR>::setMax(const SCALAR &max)
{
    m_max = max;
}

template<typename SCALAR>
const SCALAR &CT_AttributesScalarT<SCALAR>::valueAt(const size_t &index) const
{
    return (*m_collection)[index];
}

template<typename SCALAR>
void CT_AttributesScalarT<SCALAR>::setValueAt(const size_t &index, const SCALAR &value)
{
    (*m_collection)[index] = value;

    if(value < m_min)
        m_min = value;

    if(value > m_max)
        m_max = value;
}

template<typename SCALAR>
size_t CT_AttributesScalarT<SCALAR>::attributesSize() const
{
    return m_collection->size();
}

template<typename SCALAR>
void CT_AttributesScalarT<SCALAR>::initMinMax()
{
    m_min = std::numeric_limits<SCALAR>::max();
    m_max = -std::numeric_limits<SCALAR>::max();
}

#endif // CT_ATTRIBUTESSCALART_HPP
