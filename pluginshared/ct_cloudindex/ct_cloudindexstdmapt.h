#ifndef CT_CLOUDINDEXSTDMAPT_H
#define CT_CLOUDINDEXSTDMAPT_H

#include "ct_cloudindex/abstract/ct_abstractmodifiablecloudindext.h"

/**
 * A cloud of index (size_t) for T (point, face, etc...) that can be modifiabled and use a std::map.
 */
template<typename T, typename ValueT>
class CT_CloudIndexStdMapT : public CT_AbstractModifiableCloudIndexT<T>
{
public:

    typedef typename CT_AbstractModifiableCloudIndexT<T>::size_type          size_type;
    typedef typename CT_AbstractModifiableCloudIndexT<T>::FindIfFunction     FindIfFunction;
    typedef typename CT_AbstractModifiableCloudIndexT<T>::RemoveIfFunction   RemoveIfFunction;
    typedef typename CT_AbstractModifiableCloudIndexT<T>::ShiftIfFunction    ShiftIfFunction;

    CT_CloudIndexStdMapT();

    // CT_AbstractCloudIndex
    size_t size() const;
    size_t indexAt(const size_t &i) const;
    const size_type& constIndexAt(const size_t &i) const;
    size_t operator[](const size_t &i) const;
    void indexAt(const size_t &i, size_t &index) const;
    size_t first() const;
    size_t last() const;
    bool contains(const size_t &index) const;
    size_t indexOf(const size_t &index) const;
    size_t lowerBound(const size_t &value) const;
    size_t upperBound(const size_t &value) const;

    // CT_AbstractModifiableCloudIndex
    void addIndex(const size_t &newIndex);
    void removeIndex(const size_t &index);
    void replaceIndex(const size_t &i, const size_type &newIndex, const bool &verifyRespectSort = true);

    /**
     * @brief insert index and value in the map. If the index already exist, the value is replaced by the new.
     */
    void insertIndexAndValue(const size_t &index, const ValueT & value);

    /**
     * @brief return the value for the index of point, face or edge in the map
     */
    const ValueT valueAtGlobalIndex(const size_t &index, const ValueT & defaultValue = ValueT()) const;

    /**
     * @brief return the value for the index 'index' in the map
     */
    const ValueT valueAt(const size_t &index, const ValueT & defaultValue = ValueT()) const;

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

    CT_AbstractCloud* copy() const;

private:
#ifdef USE_PCL
    boost::shared_ptr< std::map<int,ValueT > > m_collection;
#else
    QSharedPointer< std::map<size_t,ValueT > >    m_collection;
#endif

    template<typename S>
    typename std::map<S,ValueT >::iterator mapFindIf(FindIfFunction findIf, void *context) const;

protected:

    template<typename U> friend class CT_AbstractGlobalCloudManagerT;

    std::map< typename CT_CloudIndexStdMapT<T,ValueT >::size_type, ValueT >* internalData() const;
    void internalShiftAll(const size_t &offset, const bool &negativeOffset);
    void internalClear();
};

#include "ct_cloudindex/ct_cloudindexstdmapt.hpp"

#endif // CT_CLOUDINDEXSTDMAPT_H
