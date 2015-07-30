#ifndef DM_DRAWELEMENTSUINTSYNCHRONIZED_H
#define DM_DRAWELEMENTSUINTSYNCHRONIZED_H

#include "ct_cloudindex/tools/ct_cloudindexstdvectortmethodimpl.h"

#include <osg/PrimitiveSet>

class DM_DrawElementsUIntSynchronized : public osg::DrawElementsUInt, public CT_AbstractModifiablePointCloudIndex
{
public:
    DM_DrawElementsUIntSynchronized(GLenum mode=0);
    DM_DrawElementsUIntSynchronized(const DM_DrawElementsUIntSynchronized& array,const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
    ~DM_DrawElementsUIntSynchronized();

    // CT_AbstractCloudIndex //
    size_t size() const;
    size_t indexAt(const size_t &i) const;
    size_t operator[](const size_t &i) const;
    void indexAt(const size_t &i, size_t &index) const;
    const ct_index_type& constIndexAt(const size_t &i) const;
    size_t first() const;
    size_t last() const;
    bool contains(const size_t &index) const;
    size_t indexOf(const size_t &index) const;
    size_t lowerBound(const size_t &value) const;
    size_t upperBound(const size_t &value) const;

#ifdef USE_PCL
    boost::shared_ptr< std::vector<int> > getPCLIndices() const;
#endif

    // CT_AbstractModifiablePointCloudIndex //
    void addIndex(const size_t &newIndex);
    void removeIndex(const size_t &index);
    void replaceIndex(const size_t &i, const ct_index_type &newIndex, const bool &verifyRespectSort = true);
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

    void shiftAll(const size_t &offset, const bool &negativeOffset);

    void eraseBetweenAndShiftRest(const size_t &eraseBeginPos, const size_t &eraseSize,
                                          const size_t &offset, const bool &negativeOffset);

    CT_AbstractCloud* copy() const;

protected:
    // CT_AbstractCloudIndexT //

    void internalShiftAll(const size_t &offset, const bool &negativeOffset);
    void internalClear();

private:
    CT_CloudIndexStdVectorTMethodImpl<osg::DrawElementsUInt::value_type>   *m_impl;
};

#endif // DM_DRAWELEMENTSUINTSYNCHRONIZED_H
