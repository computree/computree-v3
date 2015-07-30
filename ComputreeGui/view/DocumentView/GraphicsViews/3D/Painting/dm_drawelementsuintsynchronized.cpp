#include "dm_drawelementsuintsynchronized.h"

DM_DrawElementsUIntSynchronized::DM_DrawElementsUIntSynchronized(GLenum mode) : osg::DrawElementsUInt(mode), CT_AbstractModifiablePointCloudIndex()
{
    this->internalSetSortType(CT_AbstractCloudIndex::NotSorted);
    m_impl = new CT_CloudIndexStdVectorTMethodImpl<osg::DrawElementsUInt::value_type>(*this, asVector());
}

DM_DrawElementsUIntSynchronized::DM_DrawElementsUIntSynchronized(const DM_DrawElementsUIntSynchronized &array, const osg::CopyOp &copyop) : osg::DrawElementsUInt(array, copyop), CT_AbstractModifiablePointCloudIndex()
{
    this->internalSetSortType(array.sortType());
    m_impl = new CT_CloudIndexStdVectorTMethodImpl<osg::DrawElementsUInt::value_type>(*this, asVector());
}

DM_DrawElementsUIntSynchronized::~DM_DrawElementsUIntSynchronized()
{
    delete m_impl;
}

size_t DM_DrawElementsUIntSynchronized::size() const
{
    return m_impl->size();
}

size_t DM_DrawElementsUIntSynchronized::indexAt(const size_t &i) const
{
    return m_impl->indexAt(i);
}

size_t DM_DrawElementsUIntSynchronized::operator[](const size_t &i) const
{
    return m_impl->indexAt(i);
}

void DM_DrawElementsUIntSynchronized::indexAt(const size_t &i, size_t &index) const
{
    m_impl->indexAt(i, index);
}

const ct_index_type &DM_DrawElementsUIntSynchronized::constIndexAt(const size_t &i) const
{
    return m_impl->constIndexAt(i);
}

size_t DM_DrawElementsUIntSynchronized::first() const
{
    return m_impl->first();
}

size_t DM_DrawElementsUIntSynchronized::last() const
{
    return m_impl->last();
}

bool DM_DrawElementsUIntSynchronized::contains(const size_t &index) const
{
    return m_impl->contains(index);
}

size_t DM_DrawElementsUIntSynchronized::indexOf(const size_t &index) const
{
    return m_impl->indexOf(index);
}

size_t DM_DrawElementsUIntSynchronized::lowerBound(const size_t &value) const
{
    return m_impl->lowerBound(value);
}

size_t DM_DrawElementsUIntSynchronized::upperBound(const size_t &value) const
{
    return m_impl->upperBound(value);
}

void DM_DrawElementsUIntSynchronized::addIndex(const size_t &newIndex)
{
    m_impl->addIndex(newIndex);
}

void DM_DrawElementsUIntSynchronized::removeIndex(const size_t &index)
{
    m_impl->removeIndex(index);
}

void DM_DrawElementsUIntSynchronized::replaceIndex(const size_t &i, const ct_index_type &newIndex, const bool &verifyRespectSort)
{
    m_impl->replaceIndex(i, newIndex, verifyRespectSort);
}

void DM_DrawElementsUIntSynchronized::push_front(const size_t &newIndex)
{
    m_impl->push_front(newIndex);
}

void DM_DrawElementsUIntSynchronized::fill()
{
    m_impl->fill();
}

void DM_DrawElementsUIntSynchronized::clear()
{
    m_impl->clear();
}

void DM_DrawElementsUIntSynchronized::erase(const size_t &beginIndex, const size_t &sizes)
{
    m_impl->erase(beginIndex, sizes);
}

void DM_DrawElementsUIntSynchronized::resize(const size_t &newSize)
{
    m_impl->resize(newSize);
}

void DM_DrawElementsUIntSynchronized::removeIfOrShiftIf(CT_AbstractModifiableCloudIndexT::FindIfFunction findIf,
                                                        CT_AbstractModifiableCloudIndexT::RemoveIfFunction removeIf,
                                                        CT_AbstractModifiableCloudIndexT::ShiftIfFunction shiftIf,
                                                        const size_t &shiftValue,
                                                        const bool &negativeShift,
                                                        void *context)
{
    m_impl->removeIfOrShiftIf(findIf, removeIf, shiftIf, shiftValue, negativeShift, context);
}

void DM_DrawElementsUIntSynchronized::shiftAll(const size_t &offset, const bool &negativeOffset)
{
    m_impl->shiftAll(offset, negativeOffset);
}

void DM_DrawElementsUIntSynchronized::eraseBetweenAndShiftRest(const size_t &eraseBeginPos, const size_t &eraseSize, const size_t &offset, const bool &negativeOffset)
{
    m_impl->eraseBetweenAndShiftRest(eraseBeginPos, eraseSize, offset, negativeOffset);
}

CT_AbstractCloud *DM_DrawElementsUIntSynchronized::copy() const
{
    DM_DrawElementsUIntSynchronized *index = new DM_DrawElementsUIntSynchronized(*this);

    return index;
}

void DM_DrawElementsUIntSynchronized::internalShiftAll(const size_t &offset, const bool &negativeOffset)
{
    m_impl->internalShiftAll(offset, negativeOffset);
}

void DM_DrawElementsUIntSynchronized::internalClear()
{
    m_impl->internalClear();
}

#ifdef USE_PCL
boost::shared_ptr< std::vector<int> > DM_DrawElementsUIntSynchronized::getPCLIndices() const
{
    return boost::shared_ptr< std::vector<int> >(NULL);
}
#endif
