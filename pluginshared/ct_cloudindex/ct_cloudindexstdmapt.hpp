#ifndef CT_CLOUDINDEXSTDMAPT_HPP
#define CT_CLOUDINDEXSTDMAPT_HPP

#include "ct_cloudindex/ct_cloudindexstdmapt.h"

#include <functional>

template<typename T, typename ValueT>
CT_CloudIndexStdMapT<T, ValueT>::CT_CloudIndexStdMapT() : CT_AbstractModifiableCloudIndexT<T>()
{
    this->internalSetSortType(CT_AbstractCloudIndex::SortedInAscendingOrder);
    this->internalSetUnregisteredWhenIsEmpty(false);

#ifdef USE_PCL
    m_collection = boost::shared_ptr< std::map<int,ValueT > >(new std::map<int,ValueT >());
#else
    m_collection = QSharedPointer< std::map<size_t,ValueT > >(new std::map<size_t,ValueT >());
#endif
}

template<typename T, typename ValueT>
size_t CT_CloudIndexStdMapT<T, ValueT>::size() const
{
    return m_collection->size();
}

template<typename T, typename ValueT>
size_t CT_CloudIndexStdMapT<T, ValueT>::indexAt(const size_t &i) const
{
    typename std::map<size_type,ValueT >::const_iterator it = m_collection->begin();
    std::advance(it, i);

    return it->first;
}

template<typename T, typename ValueT>
const typename CT_CloudIndexStdMapT<T, ValueT>::size_type& CT_CloudIndexStdMapT<T, ValueT>::constIndexAt(const size_t &i) const
{
    typename std::map<size_type,ValueT >::const_iterator it = m_collection->begin();
    std::advance(it, i);

    return it->first;
}

template<typename T, typename ValueT>
size_t CT_CloudIndexStdMapT<T, ValueT>::operator[](const size_t &i) const
{
    typename std::map<size_type,ValueT >::const_iterator it = m_collection->begin();
    std::advance(it, i);

    return it->first;
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::indexAt(const size_t &i, size_t &index) const
{
    typename std::map<size_type,ValueT >::const_iterator it = m_collection->begin();
    std::advance(it, i);

    index = it->first;
}

template<typename T, typename ValueT>
size_t CT_CloudIndexStdMapT<T, ValueT>::first() const
{
    return m_collection->begin()->first;
}

template<typename T, typename ValueT>
size_t CT_CloudIndexStdMapT<T, ValueT>::last() const
{
    return m_collection->rbegin()->first;
}

template<typename T, typename ValueT>
bool CT_CloudIndexStdMapT<T, ValueT>::contains(const size_t &index) const
{
    return (m_collection->find(index) != m_collection->end());
}

template<typename T, typename ValueT>
size_t CT_CloudIndexStdMapT<T, ValueT>::lowerBound(const size_t &value) const
{
    typename std::map<size_type,ValueT >::iterator it;
    typename std::map<size_type,ValueT >::iterator itEnd = m_collection->end();

    it = m_collection->lower_bound(value);

    return size() - std::distance(it, itEnd);
}

template<typename T, typename ValueT>
size_t CT_CloudIndexStdMapT<T, ValueT>::upperBound(const size_t &value) const
{
    typename std::map<size_type,ValueT >::iterator it;
    typename std::map<size_type,ValueT >::iterator itEnd = m_collection->end();

    it = m_collection->upper_bound(value);

    return size() - std::distance(it, itEnd);
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::addIndex(const size_t &newIndex)
{
    ValueT val = ValueT();
    insertIndexAndValue(newIndex, val);
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::removeIndex(const size_t &index)
{
    typename std::map<size_type,ValueT >::iterator f = m_collection->begin();
    typename std::map<size_type,ValueT >::iterator l = m_collection->end();

    f = m_collection->lower_bound(index);

    if(f!=l && !(index < f->first))
        m_collection->erase(f);
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::replaceIndex(const size_t &i, const size_type &newIndex, const bool &verifyRespectSort)
{
    Q_UNUSED(verifyRespectSort)

    typename std::map<size_type,ValueT >::iterator f = m_collection->begin();
    std::advance(f, i);

    ValueT val = f->second;
    m_collection->erase(f);

    insertIndexAndValue(newIndex, val);
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::insertIndexAndValue(const size_t &index, const ValueT & value)
{
    std::pair< typename std::map<size_type,ValueT >::iterator, bool > ret;
    ret = m_collection->insert( std::pair<size_type,ValueT >(index,value) );

    if (ret.second == false)
        ret.first->second = value;
}

template<typename T, typename ValueT>
const ValueT CT_CloudIndexStdMapT<T, ValueT>::valueAtGlobalIndex(const size_t &index, const ValueT & defaultValue) const
{
    typename std::map<size_type,ValueT >::const_iterator it = m_collection->find(index);

    if(it != m_collection->end())
        return it->second;

    return defaultValue;
}

template<typename T, typename ValueT>
const ValueT CT_CloudIndexStdMapT<T, ValueT>::valueAt(const size_t &index, const ValueT & defaultValue) const
{
    typename std::map<size_type,ValueT >::const_iterator it = m_collection->begin();
    std::advance(it, index);

    if(it != m_collection->end())
        return it->second;

    return defaultValue;
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::push_front(const size_t &newIndex)
{
    m_collection->insert( std::pair<size_type, ValueT >(newIndex, ValueT()) );
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::fill()
{
    qFatal("CT_CloudIndexStdMapT<T, ValueT>::fill must not be used");
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::clear()
{
    internalClear();
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::erase(const size_t &beginIndex, const size_t &sizes)
{
    typename std::map<size_type,ValueT >::iterator b = m_collection->begin();
    std::advance(b, beginIndex);

    typename std::map<size_type,ValueT >::iterator e = m_collection->begin();
    std::advance(e, beginIndex+sizes);

    m_collection->erase(b, e);
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::resize(const size_t &newSize)
{
    qFatal("CT_CloudIndexStdMapT<T, ValueT>::resize must not be used");
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::removeIfOrShiftIf(typename CT_CloudIndexStdMapT<T, ValueT>::FindIfFunction findIf,
                                                typename CT_CloudIndexStdMapT<T, ValueT>::RemoveIfFunction removeIf,
                                                typename CT_CloudIndexStdMapT<T, ValueT>::ShiftIfFunction shiftIf,
                                                const size_t &shiftValue,
                                                const bool &negativeShift,
                                                void *context)
{
    // used only for not sorted cloud index
    qFatal("CT_CloudIndexStdMapT<T, ValueT>::removeIfOrShiftIf must not be used");
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::shiftAll(const size_t &offset, const bool &negativeOffset)
{
    this->internalShiftAll(offset, negativeOffset);
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::eraseBetweenAndShiftRest(const size_t &eraseBeginPos, const size_t &eraseSize,
                                                               const size_t &offset, const bool &negativeOffset)
{
    typename std::map<size_type,ValueT >::iterator f = m_collection->begin();
    typename std::map<size_type,ValueT >::iterator l = m_collection->end();
    std::advance(f, eraseBeginPos+eraseSize);

    std::map<size_type,ValueT > tmp;

    if(negativeOffset)
    {
        while(f != l)
        {
            tmp[f->first - offset] = f->second;
            ++f;
        }
    }
    else
    {
        while(f != l)
        {
            tmp[f->first + offset] = f->second;
            ++f;
        }
    }

    erase(eraseBeginPos, size()-eraseBeginPos);
    m_collection->insert(tmp.begin(), tmp.end());
}

#ifdef USE_PCL
template<typename T, typename ValueT>
boost::shared_ptr< std::map<int, ValueT > > CT_CloudIndexStdMapT<T, ValueT>::getPCLIndices() const
{
    return m_collection;
}
#endif

template<typename T, typename ValueT>
CT_AbstractCloudIndex* CT_CloudIndexStdMapT<T, ValueT>::copy() const
{
    CT_CloudIndexStdMapT<T, ValueT> *index = new CT_CloudIndexStdMapT<T, ValueT>();

    index->m_collection->insert(m_collection->begin(), m_collection->end());

    return index;
}

template<typename T, typename ValueT>
template<typename S>
typename std::map<S, ValueT>::iterator CT_CloudIndexStdMapT<T, ValueT>::mapFindIf(typename CT_CloudIndexStdMapT<T, ValueT>::FindIfFunction findIf, void *context) const
{
    typename std::map<S, ValueT>::iterator f = m_collection->begin();
    typename std::map<S, ValueT>::iterator l = m_collection->end();

    size_t tmp;

    while (f!=l)
    {
        tmp = f->first;

        if ((*findIf)(context, tmp))
            return f;

        ++f;
    }

    return l;
}

template<typename T, typename ValueT>
std::map< typename CT_CloudIndexStdMapT<T, ValueT>::size_type, ValueT >* CT_CloudIndexStdMapT<T, ValueT>::internalData() const
{
#ifdef USE_PCL
    return m_collection.get();
#else
    return m_collection.data();
#endif
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::internalShiftAll(const size_t &offset, const bool &negativeOffset)
{
    typename std::map<size_type,ValueT >::iterator f = m_collection->begin();
    typename std::map<size_type,ValueT >::iterator l = m_collection->end();

    std::map<size_type,ValueT > tmp;

    if(negativeOffset)
    {
        while(f != l)
        {
            tmp[f->first - offset] = f->second;
            ++f;
        }
    }
    else
    {
        while(f != l)
        {
            tmp[f->first + offset] = f->second;
            ++f;
        }
    }

    internalClear();
    m_collection->insert(tmp.begin(), tmp.end());
}

template<typename T, typename ValueT>
void CT_CloudIndexStdMapT<T, ValueT>::internalClear()
{
    m_collection->clear();
}

#endif // CT_CLOUDINDEXSTDMAPT_HPP
