#include "ct_standardundefinedsizepointcloud.h"

#include "ct_global/ct_context.h"

size_t CT_StandardUndefinedSizePointCloud::beginIndex() const
{
    return m_bIndex;
}

size_t CT_StandardUndefinedSizePointCloud::size() const
{
    return m_pc->size()-m_bIndex;
}

void CT_StandardUndefinedSizePointCloud::addPoint(const CT_Point &point)
{
    m_pc->addT(point);
}

CT_Point &CT_StandardUndefinedSizePointCloud::addPoint()
{
    return m_pc->addT();
}

CT_Point& CT_StandardUndefinedSizePointCloud::operator[](const size_t &i)
{
    return (*m_pc)[i+m_bIndex];
}

CT_Point& CT_StandardUndefinedSizePointCloud::pointAt(const size_t &i)
{
    return (*m_pc)[i+m_bIndex];
}

CT_Point& CT_StandardUndefinedSizePointCloud::operator[](const size_t &i) const
{
    return (*m_pc)[i+m_bIndex];
}

const CT_Point& CT_StandardUndefinedSizePointCloud::constPointAt(const size_t &i) const
{
    return (*m_pc)[i+m_bIndex];
}

CT_StandardUndefinedSizePointCloud::CT_StandardUndefinedSizePointCloud(const size_t &beginIndex, const CT_StandardCloudStdVectorT<CT_Point> *pc) : CT_AbstractUndefinedSizePointCloud()
{
    m_bIndex = beginIndex;
    m_pc = (CT_StandardCloudStdVectorT<CT_Point>*)pc;
}
