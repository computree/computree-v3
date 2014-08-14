#ifndef CT_ABSTRACTCLOUDINDEXT_H
#define CT_ABSTRACTCLOUDINDEXT_H

#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"
#include "ct_cloudindex/tools/ct_cloudindexiteratort.h"

/**
 * A cloud of index (size_t) for T (point, face, etc...)
 */
template<typename T>
class CT_AbstractCloudIndexT : virtual public CT_AbstractCloudIndex
{
public:
    typedef CT_CloudIndexIteratorT<T> ConstIterator;

    /**
     * @brief Returns a const iterator to the beginning of the cloud
     */
    ConstIterator constBegin() const { return ConstIterator(*this); }

    /**
     * @brief Returns a const iterator to the end of the cloud
     */
    ConstIterator constEnd() const { return ConstIterator(*this, this->size()); }

    /**
     * @return value at index passed in parameter.
     *
     *         For example, if the cloud index starts at the index 500 in the global cloud, call
     *         this method this way : tAt(0) will return the first value (T) in the cloud index which is
     *         the location 500 in the global cloud.
     *
     *         Use the method in in this manner:
     *
     *         CT_AbstractCloudIndexT<T> *index = ...;
     *         size_t size = index->size();
     *
     *         for(size_t i=0; i<size; ++i)
     *              T &myT = index->tAt(index);
     */
    T& tAt(const size_t &index) const;

    /**
     * @brief same method as previously but you can get in the same time the index in the global cloud
     * @param index : index in the cloud index
     * @param globalIndex : index in the global cloud (out parameter)
     * @return a reference to the value
     */
    T& tAt(const size_t &index, size_t &globalIndex) const;

    /**
     * @brief return a reference to the value but you pass the index of the global cloud
     *
     * The specified index corresponds to the index in the global cloud, not in this cloud index
     */
    T& tAtGlobalIndex(const size_t &index) const;

    /**
     * @brief same method as previously but return a constant reference
     */
    const T& constTAt(const size_t &index) const;

    /**
     * @brief same method as previously but you can get in the same time the index in the global cloud
     * @param index : index in the cloud index
     * @param globalIndex : index in the global cloud (out parameter)
     * @return a reference to the value
     */
    const T& constTAt(const size_t &index, size_t &globalIndex) const;

    /**
     * @brief return a constante reference to the value but you pass the index of the global cloud
     *
     * The specified index corresponds to the index in the global cloud, not in this cloud index
     */
    const T& constTAtGlobalIndex(const size_t &globalIndex) const;

protected:

    template<typename U> friend class CT_AbstractGlobalCloudManagerT;

    virtual void internalShiftAll(const size_t &offset, const bool &negativeOffset) = 0;
    virtual void internalClear() = 0;

private:

    template<typename A> friend class CT_CloudIndexStdListT;
    template<typename A> friend class CT_CloudIndexStdVectorT;
    template<typename A> friend class CT_CloudIndexLessMemoryT;
    template<typename A> friend class CT_AbstractModifiableCloudIndexT;
    template<typename A, typename B> friend class CT_CloudIndexStdMapT;
    template<typename A> friend class CT_IndexCloudColorStdMapT;
    friend class CT_PointCloudIndexVector;
    friend class CT_PointCloudIndexLessMemory;
    friend class CT_FaceCloudIndexVector;
    friend class CT_FaceCloudIndexLessMemory;
    friend class CT_EdgeCloudIndexVector;
    friend class CT_EdgeCloudIndexLessMemory;
    friend class CT_AbstractPointCloudIndex;
    friend class CT_AbstractFaceCloudIndex;
    friend class CT_AbstractEdgeCloudIndex;

    // private and :
    // friend class CT_CloudIndexStdVectorT
    // friend class CT_CloudIndexLessMemoryT
    // friend class CT_AbstractModifiableCloudIndexT
    // friend class CT_PointCloudIndexVector
    // friend class CT_PointCloudIndexLessMemory
    // friend class CT_AbstractPointCloudIndex
    //
    // why ? show CT_CloudIndexLessMemoryT constructor in ct_cloudindexlessmemoryt.h
    CT_AbstractCloudIndexT() : CT_AbstractCloudIndex() {}
};

#include "ct_cloudindex/abstract/ct_abstractcloudindext.hpp"

#endif // CT_ABSTRACTCLOUDINDEXT_H
