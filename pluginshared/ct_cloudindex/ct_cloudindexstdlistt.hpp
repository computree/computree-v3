#ifndef CT_CLOUDINDEXSTDLISTT_HPP
#define CT_CLOUDINDEXSTDLISTT_HPP

#include "ct_cloudindex/ct_cloudindexstdlistt.h"

#include <functional>

template<typename T>
CT_CloudIndexStdListT<T>::CT_CloudIndexStdListT(const size_t &size) : CT_AbstractModifiableCloudIndexT<T>()
{
    this->internalSetSortType(CT_AbstractCloudIndex::SortedInAscendingOrder);
    this->internalSetUnregisteredWhenIsEmpty(false);

#ifdef USE_PCL
    m_collection = boost::shared_ptr< std::list<int> >(new std::list<int>(size));
#else
    m_collection = QSharedPointer< std::list<size_t> >(new std::list<size_t>(size));
#endif
}

template<typename T>
void CT_CloudIndexStdListT<T>::setSortType(CT_AbstractCloudIndex::SortType type)
{
    if(this->sortType() != type)
    {
        this->internalSetSortType(type);

        if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
            m_collection->sort();
    }
}

template<typename T>
size_t CT_CloudIndexStdListT<T>::size() const
{
    return m_collection->size();
}

template<typename T>
size_t CT_CloudIndexStdListT<T>::indexAt(const size_t &i) const
{
    typename std::list<size_type>::const_iterator it = m_collection->begin();
    std::advance(it, i);

    return *it;
}

template<typename T>
const typename CT_CloudIndexStdListT<T>::size_type& CT_CloudIndexStdListT<T>::constIndexAt(const size_t &i) const
{
    typename std::list<size_type>::iterator it = m_collection->begin();
    std::advance(it, i);

    return *it;
}

template<typename T>
size_t CT_CloudIndexStdListT<T>::operator[](const size_t &i) const
{
    typename std::list<size_type>::const_iterator it = m_collection->begin();
    std::advance(it, i);

    return *it;
}

template<typename T>
void CT_CloudIndexStdListT<T>::indexAt(const size_t &i, size_t &index) const
{
    typename std::list<size_type>::const_iterator it = m_collection->begin();
    std::advance(it, i);

    index = *it;
}

template<typename T>
size_t CT_CloudIndexStdListT<T>::first() const
{
    return m_collection->front();
}

template<typename T>
size_t CT_CloudIndexStdListT<T>::last() const
{
    return m_collection->back();
}

template<typename T>
bool CT_CloudIndexStdListT<T>::contains(const size_t &index) const
{
    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        return std::binary_search(m_collection->begin(), m_collection->end(), index);

    return (std::find(m_collection->begin(), m_collection->end(), index) != m_collection->end());
}

template<typename T>
size_t CT_CloudIndexStdListT<T>::indexOf(const size_t &index) const
{
    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
    {
        typename std::list<size_type>::iterator first = m_collection->begin();
        typename std::list<size_type>::iterator last = m_collection->end();

        first = std::lower_bound(first, last, index);

        if(first!=last && !(index<(*first)))
           return *first;
    }
    else
    {
        std::list<size_type>::const_iterator it = std::find(m_collection->begin(), m_collection->end(), index);

        if(it != m_collection->end())
            return *it;
    }

    return size();
}

template<typename T>
size_t CT_CloudIndexStdListT<T>::lowerBound(const size_t &value) const
{
    typename std::list<size_type>::iterator it;
    typename std::list<size_type>::iterator itEnd = m_collection->end();

    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        it = std::lower_bound(m_collection->begin(), m_collection->end(), value);
    else
        it = std::find_if(m_collection->begin(), m_collection->end(), std::bind2nd(std::greater_equal<size_type>(), value));

    return size() - std::distance(it, itEnd);
}

template<typename T>
size_t CT_CloudIndexStdListT<T>::upperBound(const size_t &value) const
{
    typename std::list<size_type>::iterator it;
    typename std::list<size_type>::iterator itEnd = m_collection->end();

    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        it = std::upper_bound(m_collection->begin(), m_collection->end(), value);
    else
        std::find_if(m_collection->begin(), m_collection->end(), std::bind2nd(std::greater<size_type>(), value));

    return size() - std::distance(it, itEnd);
}

template<typename T>
void CT_CloudIndexStdListT<T>::addIndex(const size_t &newIndex)
{
    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        m_collection->insert(std::lower_bound(m_collection->begin(), m_collection->end(), newIndex), newIndex);
    else
        m_collection->push_back(newIndex);
}

template<typename T>
void CT_CloudIndexStdListT<T>::removeIndex(const size_t &index)
{
    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
    {
        typename std::list<size_type>::iterator first = m_collection->begin();
        typename std::list<size_type>::iterator last = m_collection->end();

        first = std::lower_bound(first, last, index);

        if(first!=last && !(index<(*first)))
            m_collection->erase(first);
    }
    else
    {
        m_collection->erase(std::find(m_collection->begin(), m_collection->end(), index));
    }
}

template<typename T>
void CT_CloudIndexStdListT<T>::replaceIndex(const size_t &i, const size_type &newIndex, const bool &verifyRespectSort)
{
    typename std::list<size_type>::iterator it = m_collection->begin();
    std::advance(it, i);

    (*it) = newIndex;

    if(verifyRespectSort
            && (this->sortType() == CT_CloudIndexStdListT<T>::SortedInAscendingOrder))
    {
        bool ok = false;

        if(i>0)
        {
            typename std::list<size_type>::const_iterator itP = it;
            --itP;

            if((*itP) <= newIndex)
                ok = true;
        }

        if(ok)
        {
            typename std::list<size_type>::const_iterator itN = it;
            ++itN;

            if(itN != m_collection->end())
            {
                if((*itN) >= newIndex)
                    ok = true;
                else
                    ok = false;
            }
        }

        if(!ok)
            this->setSortType(CT_CloudIndexStdListT<T>::NotSorted);
    }
}

template<typename T>
void CT_CloudIndexStdListT<T>::push_front(const size_t &newIndex)
{
    CT_AbstractCloudIndex::SortType t = this->sortType();

    if(t != CT_AbstractCloudIndex::NotSorted)
    {
        if(size() > 0)
        {
            if(newIndex > first())
                t = CT_AbstractCloudIndex::NotSorted;
        }
    }

    m_collection->insert(m_collection->begin(), newIndex);
}

template<typename T>
void CT_CloudIndexStdListT<T>::fill()
{
    typename std::list<size_type>::iterator it = m_collection->begin();
    typename std::list<size_type>::iterator end = m_collection->end();

    size_type i = 0;

    while(it != end)
    {
        (*it) = i;
        ++it;
        ++i;
    }
}

template<typename T>
void CT_CloudIndexStdListT<T>::clear()
{
    internalClear();
}

template<typename T>
void CT_CloudIndexStdListT<T>::erase(const size_t &beginIndex, const size_t &sizes)
{
    typename std::list<size_type>::iterator b = m_collection->begin();
    std::advance(b, beginIndex);

    typename std::list<size_type>::iterator e = m_collection->begin();
    std::advance(e, beginIndex+sizes);

    m_collection->erase(b, e);
}

template<typename T>
void CT_CloudIndexStdListT<T>::resize(const size_t &newSize)
{
    m_collection->resize(newSize);
}

template<typename T>
void CT_CloudIndexStdListT<T>::removeIfOrShiftIf(typename CT_CloudIndexStdListT<T>::FindIfFunction findIf,
                                                typename CT_CloudIndexStdListT<T>::RemoveIfFunction removeIf,
                                                typename CT_CloudIndexStdListT<T>::ShiftIfFunction shiftIf,
                                                const size_t &shiftValue,
                                                const bool &negativeShift,
                                                void *context)
{
    typename std::list<size_type>::iterator first = listFindIf<size_type>(findIf, context);
    typename std::list<size_type>::iterator last = m_collection->end();

    if(first != last)
    {
        typename std::list<size_type>::iterator i = first;

        size_t nI;

        while(i != last)
        {
            nI = *i;

            if(!(*removeIf)(context, nI))
            {
                if((*shiftIf)(context, nI))
                {
                    if(negativeShift)
                        *i -= shiftValue;
                    else
                        *i += shiftValue;
                }

                *first = *i;
                ++first;
            }

            ++i;
        }
    }

    if(first != last)
        m_collection->erase(first);
}

template<typename T>
void CT_CloudIndexStdListT<T>::shiftAll(const size_t &offset, const bool &negativeOffset)
{
    this->internalShiftAll(offset, negativeOffset);
}

template<typename T>
void CT_CloudIndexStdListT<T>::eraseBetweenAndShiftRest(const size_t &eraseBeginPos, const size_t &eraseSize,
                                                        const size_t &offset, const bool &negativeOffset)
{
    erase(eraseBeginPos, eraseSize);

    typename std::list<size_type>::iterator b = m_collection->begin();
    typename std::list<size_type>::iterator e = m_collection->end();
    std::advance(b, eraseBeginPos);

    if(negativeOffset)
    {
        while(b != e)
        {
            (*b) = (*b) - offset;
            ++b;
        }
    }
    else
    {
        while(b != e)
        {
            (*b) = (*b) + offset;
            ++b;
        }
    }
}

#ifdef USE_PCL
template<typename T>
boost::shared_ptr< std::vector<int> > CT_CloudIndexStdListT<T>::getPCLIndices() const
{
    boost::shared_ptr< std::vector<int> > indices = boost::shared_ptr< std::vector<int> >(new std::vector<int>(size()));

    typename std::list<int>::iterator f = m_collection->begin();
    typename std::list<int>::iterator l = m_collection->end();

    size_t i = 0;

    while(f != l)
    {
        (*indices.get())[i] = *f;
        ++f;
        ++i;
    }

    return indices;
}
#endif

template<typename T>
CT_AbstractCloudIndex* CT_CloudIndexStdListT<T>::copy() const
{
    CT_CloudIndexStdListT<T> *index = new CT_CloudIndexStdListT<T>(size());

    std::copy(m_collection->begin(), m_collection->end(), index->m_collection->begin());

    index->setSortType(this->sortType());

    return index;
}

template<typename T>
template<typename S>
typename std::list<S>::iterator CT_CloudIndexStdListT<T>::listFindIf(typename CT_CloudIndexStdListT<T>::FindIfFunction findIf, void *context) const
{
    typename std::list<S>::iterator first = m_collection->begin();
    typename std::list<S>::iterator last = m_collection->end();

    size_t tmp;

    while (first!=last)
    {
        tmp = *first;

        if ((*findIf)(context, tmp))
            return first;

        ++first;
    }

    return last;
}

template<typename T>
std::list< typename CT_CloudIndexStdListT<T>::size_type >* CT_CloudIndexStdListT<T>::internalData() const
{
#ifdef USE_PCL
    return m_collection.get();
#else
    return m_collection.data();
#endif
}

template<typename T>
void CT_CloudIndexStdListT<T>::internalShiftAll(const size_t &offset, const bool &negativeOffset)
{
    typename std::list<size_type>::iterator first = m_collection->begin();
    typename std::list<size_type>::iterator last = m_collection->end();

    if(negativeOffset)
    {
        while(first != last)
        {
            *first -= offset;
            ++first;
        }
    }
    else
    {
        while(first != last)
        {
            *first += offset;
            ++first;
        }
    }
}

template<typename T>
void CT_CloudIndexStdListT<T>::internalClear()
{
    m_collection->clear();
}

#endif // CT_CLOUDINDEXSTDLISTT_HPP
