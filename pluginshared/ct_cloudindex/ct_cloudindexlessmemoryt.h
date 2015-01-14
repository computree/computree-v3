#ifndef CT_CLOUDINDEXLESSMEMORYT_H
#define CT_CLOUDINDEXLESSMEMORYT_H

#include "ct_cloudindex/abstract/ct_abstractcloudindext.h"

/**
 * A cloud of index (size_t) for T (point, face, etc...) that cannot be modifiabled but use just
 * two size_t (begin and size) in memory.
 */
template<typename T>
class CT_CloudIndexLessMemoryT : virtual public CT_AbstractCloudIndexT<T>
{
public:
    // CT_AbstractCloudIndex
    size_t size() const;
    size_t indexAt(const size_t &i) const;
    const CT_AbstractCloudIndex::size_type& constIndexAt(const size_t &i) const;
    size_t operator[](const size_t &i) const;
    void indexAt(const size_t &i, size_t &index) const;
    size_t first() const;
    size_t last() const;
    bool contains(const size_t &index) const;
    size_t indexOf(const size_t &index) const;
    size_t lowerBound(const size_t &value) const;
    size_t upperBound(const size_t &value) const;

    void shiftAll(const size_t &offset, const bool &negativeOffset);
    bool cut(const size_t &begin, const size_t &end);
    void clear();

#ifdef USE_PCL
    boost::shared_ptr< std::vector<int> > getPCLIndices() const;
#endif

    CT_AbstractCloud* copy() const;

private:

    friend class CT_PointCloudIndexLessMemory;
    friend class CT_FaceCloudIndexLessMemory;
    friend class CT_EdgeCloudIndexLessMemory;

    // private and friend class CT_XCloudIndexLessMemory to allow
    // dynamic_cast a CT_AbstractCloudIndexT<CT_Point> to ICloudIndex.
    // dynamic_cast a CT_AbstractCloudIndexT<CT_Face> to ICloudIndex.
    // dynamic_cast a CT_AbstractCloudIndexT<CT_Edge> to ICloudIndex.
    //
    // only CT_PointCloudIndexLessMemory can create a CT_CloudIndexLessMemoryT
    // only CT_FaceCloudIndexLessMemory can create a CT_CloudIndexLessMemoryT
    // only CT_EdgeCloudIndexLessMemory can create a CT_CloudIndexLessMemoryT
    CT_CloudIndexLessMemoryT(const size_t &begin, const size_t &size);

    size_t                                      m_begin;
    size_t                                      m_size;
    mutable CT_AbstractCloudIndex::size_type    m_temporary;

protected:
    void erase(const size_t &beginIndex, const size_t &sizes);
    void resize(const size_t &newSize);
    void internalShiftAll(const size_t &offset, const bool &negativeOffset);
    void internalClear();
};

#include "ct_cloudindex/ct_cloudindexlessmemoryt.hpp"

#endif // CT_CLOUDINDEXLESSMEMORYT_H
