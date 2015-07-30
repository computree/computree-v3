#ifndef CT_CloudIndexStdVectorTMethodImpl_HPP
#define CT_CloudIndexStdVectorTMethodImpl_HPP

#include "ct_cloudindex/tools/ct_cloudindexstdvectortmethodimpl.h"

template<typename T>
CT_CloudIndexStdVectorTMethodImpl<T>::CT_CloudIndexStdVectorTMethodImpl(const CT_AbstractCloudIndex &ci, std::vector<T> &vector) : _vector(vector), m_ci(ci)
{
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::setSortType(CT_AbstractCloudIndex::SortType type)
{
    if(m_ci.sortType() != type)
    {
        if(m_ci.sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
            std::sort(_vector.begin(), _vector.end());
    }
}

template<typename T>
size_t CT_CloudIndexStdVectorTMethodImpl<T>::size() const
{
    return _vector.size();
}

template<typename T>
size_t CT_CloudIndexStdVectorTMethodImpl<T>::indexAt(const size_t &i) const
{
    return _vector[i];
}

template<typename T>
size_t CT_CloudIndexStdVectorTMethodImpl<T>::operator[](const size_t &i) const
{
    return _vector[i];
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::indexAt(const size_t &i, size_t &index) const
{
    index = _vector[i];
}

template<typename T>
const T& CT_CloudIndexStdVectorTMethodImpl<T>::constIndexAt(const size_t &i) const
{
    return _vector[i];
}

template<typename T>
size_t CT_CloudIndexStdVectorTMethodImpl<T>::first() const
{
    return _vector.front();
}

template<typename T>
size_t CT_CloudIndexStdVectorTMethodImpl<T>::last() const
{
    return _vector.back();
}

template<typename T>
bool CT_CloudIndexStdVectorTMethodImpl<T>::contains(const size_t &index) const
{
    if(m_ci.sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        return std::binary_search(_vector.begin(), _vector.end(), index);

    return (std::find(_vector.begin(), _vector.end(), index) != _vector.end());
}

template<typename T>
size_t CT_CloudIndexStdVectorTMethodImpl<T>::indexOf(const size_t &index) const
{
    if(m_ci.sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
    {
        typename std::vector<T>::iterator first = _vector.begin();
        typename std::vector<T>::iterator last = _vector.end();

        first = std::lower_bound(first, last, index);

        if(first!=last && !(index<(*first)))
            return *first;
    }
    else
    {
        typename std::vector<T>::const_iterator it = std::find(_vector.begin(), _vector.end(), index);

        if(it != _vector.end())
            return *it;
    }

    return size();
}

template<typename T>
size_t CT_CloudIndexStdVectorTMethodImpl<T>::lowerBound(const size_t &value) const
{
    typename std::vector<T>::iterator it;
    typename std::vector<T>::iterator itEnd = _vector.end();

    if(m_ci.sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        it = std::lower_bound(_vector.begin(), _vector.end(), value);
    else
        it = std::find_if(_vector.begin(), _vector.end(), std::bind2nd(std::greater_equal<T>(), value));

    return size() - (itEnd-it);
}

template<typename T>
size_t CT_CloudIndexStdVectorTMethodImpl<T>::upperBound(const size_t &value) const
{
    typename std::vector<T>::iterator it;
    typename std::vector<T>::iterator itEnd = _vector.end();

    if(m_ci.sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        it = std::upper_bound(_vector.begin(), _vector.end(), value);
    else
        std::find_if(_vector.begin(), _vector.end(), std::bind2nd(std::greater<T>(), value));

    return size() - (itEnd-it);
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::addIndex(const size_t &newIndex)
{
    if(m_ci.sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        _vector.insert(std::lower_bound(_vector.begin(), _vector.end(), newIndex), newIndex);
    else
        _vector.push_back(newIndex);
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::removeIndex(const size_t &index)
{
    if(m_ci.sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
    {
        typename std::vector<T>::iterator first = _vector.begin();
        typename std::vector<T>::iterator last = _vector.end();

        first = std::lower_bound(first, last, index);

        if(first!=last && !(index<(*first)))
            _vector.erase(first);
    }
    else
    {
        _vector.erase(std::find(_vector.begin(), _vector.end(), index));
    }
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::replaceIndex(const size_t &i, const T &newIndex, const bool &verifyRespectSort)
{
    _vector[i] = newIndex;

    if(verifyRespectSort
            && (m_ci.sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder))
    {
        bool ok = false;

        if(i>0)
        {
            if(_vector.at(i-1) <= newIndex)
                ok = true;
        }

        if(ok && i<(size()-1))
        {
            if(_vector.at(i+1) >= newIndex)
                ok = true;
            else
                ok = false;
        }

        if(!ok)
            this->setSortType(CT_AbstractCloudIndex::NotSorted);
    }
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::push_front(const size_t &newIndex)
{
    CT_AbstractCloudIndex::SortType t = m_ci.sortType();

    if(t != CT_AbstractCloudIndex::NotSorted)
    {
        if(size() > 0)
        {
            if(newIndex > first())
                t = CT_AbstractCloudIndex::NotSorted;
        }
    }

    _vector.insert(_vector.begin(), newIndex);
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::fill()
{
    size_t s = size();

    for(size_t i=0; i<s; ++i)
        _vector[i] = i;
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::clear()
{
    internalClear();
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::erase(const size_t &beginIndex, const size_t &sizes)
{
    size_t endIndex = (beginIndex+sizes)-1;
    size_t cpySize = size()-(beginIndex+sizes);

    if(cpySize > 0)
    {
        T *data = _vector.data();

        T *dst = data+beginIndex;
        T *src = data+endIndex;

        memcpy(dst, src, sizeof(T)*cpySize);
    }

    resize(size()-sizes);
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::resize(const size_t &newSize)
{
    _vector.resize(newSize, 0);
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::reserve(const size_t &newSize)
{
    _vector.reserve(newSize);
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::removeIfOrShiftIf(typename CT_CloudIndexStdVectorTMethodImpl<T>::FindIfFunction findIf,
                                                typename CT_CloudIndexStdVectorTMethodImpl<T>::RemoveIfFunction removeIf,
                                                typename CT_CloudIndexStdVectorTMethodImpl<T>::ShiftIfFunction shiftIf,
                                                const size_t &shiftValue,
                                                const bool &negativeShift,
                                                void *context)
{
    typename std::vector<T>::iterator first = vectorFindIf(findIf, context);
    typename std::vector<T>::iterator last = _vector.end();

    if(first != last)
    {
        typename std::vector<T>::iterator i = first;

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
        _vector.erase(first);
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::shiftAll(const size_t &offset, const bool &negativeOffset)
{
    this->internalShiftAll(offset, negativeOffset);
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::eraseBetweenAndShiftRest(const size_t &eraseBeginPos, const size_t &eraseSize,
                                                          const size_t &offset, const bool &negativeOffset)
{
    size_t isize = size();
    size_t i = eraseBeginPos;
    size_t j = i+eraseSize;

    // we copy the shift part to the erase part and shift the index in the same time
    if(negativeOffset)
    {
        while(j < isize)
        {
            _vector.at(i) -= offset;

            ++j;
            ++i;
        }
    }
    else
    {
        while(j < isize)
        {
            _vector.at(i) += offset;

            ++j;
            ++i;
        }
    }

    // we erase the end of the cloud
    if(i < isize)
        erase(i, isize-i);
}

template<typename T>
typename std::vector<T>::iterator CT_CloudIndexStdVectorTMethodImpl<T>::vectorFindIf(typename CT_CloudIndexStdVectorTMethodImpl<T>::FindIfFunction findIf,
                                                                                     void *context) const
{
    typename std::vector<T>::iterator first = _vector.begin();
    typename std::vector<T>::iterator last = _vector.end();

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
void CT_CloudIndexStdVectorTMethodImpl<T>::internalShiftAll(const size_t &offset, const bool &negativeOffset)
{
    typename std::vector<T>::iterator first = _vector.begin();
    typename std::vector<T>::iterator last = _vector.end();

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
void CT_CloudIndexStdVectorTMethodImpl<T>::internalClear()
{
    _vector.clear();
}

template<typename T>
void CT_CloudIndexStdVectorTMethodImpl<T>::copy(std::vector<T> &destination)
{
    std::vector<T>::const_iterator it = _vector.begin();
    std::vector<T>::const_iterator end = _vector.begin();
    std::vector<T>::iterator itDest = destination.begin();

    while(it != end) {
        (*itDest) = (*it);
        ++it;
        ++itDest;
    }
}

#endif // CT_CloudIndexStdVectorTMethodImpl_HPP
