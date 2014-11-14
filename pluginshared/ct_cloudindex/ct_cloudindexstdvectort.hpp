#ifndef CT_CLOUDINDEXSTDVECTORT_HPP
#define CT_CLOUDINDEXSTDVECTORT_HPP

#include "ct_cloudindex/ct_cloudindexstdvectort.h"

#include <functional>

template<typename T>
CT_CloudIndexStdVectorT<T>::CT_CloudIndexStdVectorT(const size_t &size) : CT_AbstractModifiableCloudIndexT<T>()
{
    this->internalSetSortType(CT_AbstractCloudIndex::SortedInAscendingOrder);

#ifdef USE_PCL
    _vector = boost::shared_ptr< std::vector<int> >(new std::vector<int>(size));
#else
    _vector = QSharedPointer< std::vector<size_t> >(new std::vector<size_t>(size));
#endif
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::setSortType(CT_AbstractCloudIndex::SortType type)
{
    if(this->sortType() != type)
    {
        this->internalSetSortType(type);

        if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
            std::sort(_vector->begin(), _vector->end());
    }
}

template<typename T>
size_t CT_CloudIndexStdVectorT<T>::size() const
{
    return _vector->size();
}

template<typename T>
size_t CT_CloudIndexStdVectorT<T>::indexAt(const size_t &i) const
{
#ifdef USE_PCL
    return (*_vector.get())[i];
#else
    return (*_vector.data())[i];
#endif
}

template<typename T>
const typename CT_CloudIndexStdVectorT<T>::size_type& CT_CloudIndexStdVectorT<T>::constIndexAt(const size_t &i) const
{
#ifdef USE_PCL
    return (*_vector.get())[i];
#else
    return (*_vector.data())[i];
#endif
}

template<typename T>
size_t CT_CloudIndexStdVectorT<T>::operator[](const size_t &i) const
{
#ifdef USE_PCL
    return (*_vector.get())[i];
#else
    return (*_vector.data())[i];
#endif
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::indexAt(const size_t &i, size_t &index) const
{
#ifdef USE_PCL
    index = (*_vector.get())[i];
#else
    index = (*_vector.data())[i];
#endif
}

template<typename T>
size_t CT_CloudIndexStdVectorT<T>::first() const
{
    return _vector->front();
}

template<typename T>
size_t CT_CloudIndexStdVectorT<T>::last() const
{
    return _vector->back();
}

template<typename T>
bool CT_CloudIndexStdVectorT<T>::contains(const size_t &index) const
{
    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        return std::binary_search(_vector->begin(), _vector->end(), index);

    return (std::find(_vector->begin(), _vector->end(), index) != _vector->end());
}

template<typename T>
size_t CT_CloudIndexStdVectorT<T>::indexOf(const size_t &index) const
{
    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
    {
        typename std::vector<size_type>::iterator first = _vector->begin();
        typename std::vector<size_type>::iterator last = _vector->end();

        first = std::lower_bound(first, last, index);

        if(first!=last && !(index<(*first)))
            return *first;
    }
    else
    {
        typename std::vector<size_type>::const_iterator it = std::find(_vector->begin(), _vector->end(), index);

        if(it != _vector->end())
            return *it;
    }

    return size();
}

template<typename T>
size_t CT_CloudIndexStdVectorT<T>::lowerBound(const size_t &value) const
{
    typename std::vector<size_type>::iterator it;
    typename std::vector<size_type>::iterator itEnd = _vector->end();

    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        it = std::lower_bound(_vector->begin(), _vector->end(), value);
    else
        it = std::find_if(_vector->begin(), _vector->end(), std::bind2nd(std::greater_equal<size_type>(), value));

    return size() - (itEnd-it);
}

template<typename T>
size_t CT_CloudIndexStdVectorT<T>::upperBound(const size_t &value) const
{
    typename std::vector<size_type>::iterator it;
    typename std::vector<size_type>::iterator itEnd = _vector->end();

    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        it = std::upper_bound(_vector->begin(), _vector->end(), value);
    else
        std::find_if(_vector->begin(), _vector->end(), std::bind2nd(std::greater<size_type>(), value));

    return size() - (itEnd-it);
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::addIndex(const size_t &newIndex)
{
    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
        _vector->insert(std::lower_bound(_vector->begin(), _vector->end(), newIndex), newIndex);
    else
        _vector->push_back(newIndex);
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::removeIndex(const size_t &index)
{
    if(this->sortType() == CT_AbstractCloudIndex::SortedInAscendingOrder)
    {
        typename std::vector<size_type>::iterator first = _vector->begin();
        typename std::vector<size_type>::iterator last = _vector->end();

        first = std::lower_bound(first, last, index);

        if(first!=last && !(index<(*first)))
            _vector->erase(first);
    }
    else
    {
        _vector->erase(std::find(_vector->begin(), _vector->end(), index));
    }
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::replaceIndex(const size_t &i, const size_type &newIndex, const bool &verifyRespectSort)
{
#ifdef USE_PCL
    (*_vector.get())[i] = newIndex;
#else
    (*_vector.data())[i] = newIndex;
#endif

    if(verifyRespectSort
            && (this->sortType() == CT_CloudIndexStdVectorT<T>::SortedInAscendingOrder))
    {
        bool ok = false;

        if(i>0)
        {
            if(_vector->at(i-1) <= newIndex)
                ok = true;
        }

        if(ok && i<(size()-1))
        {
            if(_vector->at(i+1) >= newIndex)
                ok = true;
            else
                ok = false;
        }

        if(!ok)
            this->setSortType(CT_CloudIndexStdVectorT<T>::NotSorted);
    }
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::push_front(const size_t &newIndex)
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

    _vector->insert(_vector->begin(), newIndex);
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::fill()
{
    size_t s = size();

    for(size_t i=0; i<s; ++i)
    {
        #ifdef USE_PCL
            (*_vector.get())[i] = i;
        #else
            (*_vector.data())[i] = i;
        #endif
    }
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::clear()
{
    internalClear();
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::erase(const size_t &beginIndex, const size_t &sizes)
{
    size_t endIndex = (beginIndex+sizes)-1;
    size_t cpySize = size()-(beginIndex+sizes);

    if(cpySize > 0)
    {
        size_type *data = _vector->data();

        size_type *dst = data+beginIndex;
        size_type *src = data+endIndex;

        memcpy(dst, src, sizeof(size_type)*cpySize);
    }

    resize(size()-sizes);
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::resize(const size_t &newSize)
{
    _vector->resize(newSize);
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::reserve(const size_t &newSize)
{
    _vector->reserve(newSize);
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::removeIfOrShiftIf(typename CT_CloudIndexStdVectorT<T>::FindIfFunction findIf,
                                                typename CT_CloudIndexStdVectorT<T>::RemoveIfFunction removeIf,
                                                typename CT_CloudIndexStdVectorT<T>::ShiftIfFunction shiftIf,
                                                const size_t &shiftValue,
                                                const bool &negativeShift,
                                                void *context)
{
    typename std::vector<size_type>::iterator first = vectorFindIf<size_type>(findIf, context);
    typename std::vector<size_type>::iterator last = _vector->end();

    if(first != last)
    {
        typename std::vector<size_type>::iterator i = first;

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
        _vector->erase(first);
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::shiftAll(const size_t &offset, const bool &negativeOffset)
{
    this->internalShiftAll(offset, negativeOffset);
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::eraseBetweenAndShiftRest(const size_t &eraseBeginPos, const size_t &eraseSize,
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
            _vector->at(i) -= offset;

            ++j;
            ++i;
        }
    }
    else
    {
        while(j < isize)
        {
            _vector->at(i) += offset;

            ++j;
            ++i;
        }
    }

    // we erase the end of the cloud
    if(i < isize)
        erase(i, isize-i);
}

#ifdef USE_PCL
template<typename T>
boost::shared_ptr< std::vector<int> > CT_CloudIndexStdVectorT<T>::getPCLIndices() const
{
    return _vector;
}
#endif

template<typename T>
CT_AbstractCloudIndex* CT_CloudIndexStdVectorT<T>::copy() const
{
    size_t s = size();

    CT_CloudIndexStdVectorT<T> *index = new CT_CloudIndexStdVectorT<T>(s);

#ifdef USE_PCL
    std::vector<size_type> *vector = _vector.get();
    std::vector<size_type> *cpyVector = index->_vector.get();
#else
    std::vector<size_type> *vector = _vector.data();
    std::vector<size_type> *cpyVector = index->_vector.data();
#endif

    for(size_t i=0; i<s; ++i)
        (*cpyVector)[i] = (*vector)[i];

    index->setSortType(this->sortType());

    return index;
}

template<typename T>
template<typename S>
typename std::vector<S>::iterator CT_CloudIndexStdVectorT<T>::vectorFindIf(typename CT_CloudIndexStdVectorT<T>::FindIfFunction findIf, void *context) const
{
    typename std::vector<S>::iterator first = _vector->begin();
    typename std::vector<S>::iterator last = _vector->end();

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
std::vector< typename CT_CloudIndexStdVectorT<T>::size_type >* CT_CloudIndexStdVectorT<T>::internalData() const
{
#ifdef USE_PCL
    return _vector.get();
#else
    return _vector.data();
#endif
}

template<typename T>
void CT_CloudIndexStdVectorT<T>::internalShiftAll(const size_t &offset, const bool &negativeOffset)
{
    typename std::vector<size_type>::iterator first = _vector->begin();
    typename std::vector<size_type>::iterator last = _vector->end();

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
void CT_CloudIndexStdVectorT<T>::internalClear()
{
    _vector->clear();
}

#endif // CT_CLOUDINDEXSTDVECTORT_HPP
