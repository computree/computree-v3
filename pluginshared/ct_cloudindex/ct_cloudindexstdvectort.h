#ifndef CT_CLOUDINDEXSTDVECTORT_H
#define CT_CLOUDINDEXSTDVECTORT_H

#include "ct_cloudindex/abstract/ct_abstractmodifiablecloudindext.h"

#ifndef USE_PCL
#include <QSharedPointer>
#include <vector>
#endif

/**
 * A cloud of index (size_t) for T (point, face, etc...) that can be modifiabled and use a std::vector.
 */
template<typename T>
class CT_CloudIndexStdVectorT : public CT_AbstractModifiableCloudIndexT<T>
{
public:

    typedef typename CT_AbstractModifiableCloudIndexT<T>::size_type          size_type;
    typedef typename CT_AbstractModifiableCloudIndexT<T>::FindIfFunction     FindIfFunction;
    typedef typename CT_AbstractModifiableCloudIndexT<T>::RemoveIfFunction   RemoveIfFunction;
    typedef typename CT_AbstractModifiableCloudIndexT<T>::ShiftIfFunction    ShiftIfFunction;

    CT_CloudIndexStdVectorT(const size_t &size = 0);

    /**
     * @brief Define the sort type for the cloud.
     *
     * If the type changed this method rearrange indexes to concord with the new sort type.
     */
    void setSortType(CT_AbstractCloudIndex::SortType type);

    // CT_AbstractCloudIndex
    size_t size() const;
    size_t indexAt(const size_t &i) const;
    const size_type& constIndexAt(const size_t &i) const;
    size_t operator[](const size_t &i) const;
    void indexAt(const size_t &i, size_t &index) const;
    size_t first() const;
    size_t last() const;
    bool contains(const size_t &index) const;
    size_t lowerBound(const size_t &value) const;
    size_t upperBound(const size_t &value) const;

    // CT_AbstractModifiableCloudIndex
    void addIndex(const size_t &newIndex);
    void removeIndex(const size_t &index);
    void replaceIndex(const size_t &i, const size_type &newIndex, const bool &verifyRespectSort = true);

    /**
     * @warning Don't use this method. show the definition of this method in the superclass
     *
     * PREFERS addIndex method
     */
    void push_front(const size_t &newIndex);

    void fill();
    void clear();
    void erase(const size_t &beginIndex, const size_t &sizes);
    void resize(const size_t &newSize);
    void removeIfOrShiftIf(FindIfFunction findIf,
                           RemoveIfFunction removeIf,
                           ShiftIfFunction shiftIf,
                           const size_t &shiftValue,
                           const bool &negativeShift,
                           void *context);
    void shiftAll(const size_t& offset, const bool &negativeOffset);
    void eraseBetweenAndShiftRest(const size_t &eraseBeginPos, const size_t &eraseSize,
                                  const size_t &offset, const bool &negativeOffset);

#ifdef USE_PCL
    boost::shared_ptr< std::vector<int> > getPCLIndices() const;
#endif

    CT_AbstractCloudIndex* copy() const;

private:
#ifdef USE_PCL
    boost::shared_ptr< std::vector<int> > _vector;
#else
    QSharedPointer< std::vector<size_t> >    _vector;
#endif

    template<typename S>
    typename std::vector<S>::iterator vectorFindIf(FindIfFunction findIf, void *context) const;

protected:

    template<typename U> friend class CT_AbstractGlobalCloudManagerT;

    std::vector< typename CT_CloudIndexStdVectorT<T>::size_type >* internalData() const;
    void internalShiftAll(const size_t &offset, const bool &negativeOffset);
    void internalClear();
};

#include "ct_cloudindex/ct_cloudindexstdvectort.hpp"

#endif // CT_CLOUDINDEXSTDVECTORT_H
