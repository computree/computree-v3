#include "ct_mutablepointiterator.h"

#include "ct_global/ct_context.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

CT_MutablePointIterator::CT_MutablePointIterator(const CT_AbstractPointCloudIndex *pci)
{
    m_it = new CT_PointIterator(pci);
}

CT_MutablePointIterator::CT_MutablePointIterator(CT_CIR cir)
{
    m_it = new CT_PointIterator(cir);
}

CT_MutablePointIterator::~CT_MutablePointIterator()
{
    delete m_it;
}

bool CT_MutablePointIterator::hasNext() const
{
    return m_it->hasNext();
}

CT_MutablePointIterator& CT_MutablePointIterator::next()
{
    m_it->next();
    return *this;
}

bool CT_MutablePointIterator::hasPrevious() const
{
    return m_it->hasPrevious();
}

CT_MutablePointIterator& CT_MutablePointIterator::previous()
{
    m_it->previous();
    return *this;
}

CT_MutablePointIterator& CT_MutablePointIterator::jump(size_t n)
{
    m_it->jump(n);
    return *this;
}

void CT_MutablePointIterator::toFront()
{
    m_it->toFront();
}

void CT_MutablePointIterator::toBack()
{
    m_it->toBack();
}

size_t CT_MutablePointIterator::size() const
{
    return m_it->size();
}

size_t CT_MutablePointIterator::currentGlobalIndex() const
{
    return m_it->currentGlobalIndex();
}

CT_AbstractCoordinateSystem* CT_MutablePointIterator::currentCoordinateSystem() const
{
    return m_it->currentCoordinateSystem();
}

GLuint CT_MutablePointIterator::currentCoordinateSystemIndex() const
{
    return m_it->currentCoordinateSystemIndex();
}

const CT_Point &CT_MutablePointIterator::currentPoint() const
{
    return m_it->currentPoint();
}

void CT_MutablePointIterator::replaceCurrentPoint(const CT_Point &p, const CT_AbstractCoordinateSystem *coordinateSystem)
{
    CT_PointData &pData = m_it->currentInternalPoint();

    coordinateSystem->convertImport(p(CT_Point::X), p(CT_Point::Y), p(CT_Point::Z), pData(CT_PointData::X), pData(CT_PointData::Y), pData(CT_PointData::Z));

    PS_COORDINATES_SYS_MANAGER->setCoordinateSystemForPointAt(currentGlobalIndex(), PS_COORDINATES_SYS_MANAGER->indexOfCoordinateSystem(coordinateSystem));
}

void CT_MutablePointIterator::replaceCurrentPoint(const CT_Point &p, const GLuint &coordinateSystemGlobalIndex)
{
    CT_AbstractCoordinateSystem *coordinateSystem = PS_COORDINATES_SYS_MANAGER->coordinateSystemAt(coordinateSystemGlobalIndex);

    CT_PointData &pData = m_it->currentInternalPoint();

    coordinateSystem->convertImport(p(CT_Point::X), p(CT_Point::Y), p(CT_Point::Z), pData(CT_PointData::X), pData(CT_PointData::Y), pData(CT_PointData::Z));

    PS_COORDINATES_SYS_MANAGER->setCoordinateSystemForPointAt(currentGlobalIndex(), coordinateSystemGlobalIndex);
}

void CT_MutablePointIterator::replaceCurrentPoint(const CT_Point &p)
{
    CT_AbstractCoordinateSystem *coordinateSystem = currentCoordinateSystem();

    CT_PointData &pData = m_it->currentInternalPoint();

    coordinateSystem->convertImport(p(CT_Point::X), p(CT_Point::Y), p(CT_Point::Z), pData(CT_PointData::X), pData(CT_PointData::Y), pData(CT_PointData::Z));
}

CT_PointData &CT_MutablePointIterator::currentInternalPoint()
{
    return m_it->currentInternalPoint();
}

const CT_PointData& CT_MutablePointIterator::currentConstInternalPoint() const
{
    return m_it->currentConstInternalPoint();
}

void CT_MutablePointIterator::replaceCurrentInternalPoint(const CT_PointData &p, const GLuint &coordinateSystemGlobalIndex)
{
    CT_PointData &pData = m_it->currentInternalPoint();
    pData = p;

    PS_COORDINATES_SYS_MANAGER->setCoordinateSystemForPointAt(currentGlobalIndex(), coordinateSystemGlobalIndex);
}

void CT_MutablePointIterator::replaceCurrentInternalPoint(const CT_PointData &p)
{
    CT_PointData &pData = m_it->currentInternalPoint();
    pData = p;
}

void CT_MutablePointIterator::replaceCurrentInternalPoint(const CT_PointData &p, const CT_AbstractCoordinateSystem *coordinateSystem)
{
    CT_PointData &pData = m_it->currentInternalPoint();
    pData = p;

    PS_COORDINATES_SYS_MANAGER->setCoordinateSystemForPointAt(currentGlobalIndex(), PS_COORDINATES_SYS_MANAGER->indexOfCoordinateSystem(coordinateSystem));
}
