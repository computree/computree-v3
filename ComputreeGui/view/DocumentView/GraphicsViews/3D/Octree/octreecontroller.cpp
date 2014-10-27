#include "octreecontroller.h"

#include "ct_cloudindex/ct_cloudindexstdvectort.h"
#include "ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregisteredt.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"

#include <qglviewer.h>
#include <limits>

OctreeController::OctreeController()
{
    m_octree = new Octree< CT_Repository::CT_AbstractModifiablePCIR >(32);
    m_newNumberOfCells = m_octree->size();
    resetNewMinAndMax();
    m_min = m_newMin;
    m_max = m_newMax;
    m_size = 0;
    m_forceMustBeReconstructed = false;
}

OctreeController::~OctreeController()
{
    clear();
}

void OctreeController::setNumberOfCells(const int &s)
{
    m_newNumberOfCells = s;

    if(mustBeReconstructed())
        emit octreeMustBeReconstructed(true);
}

int OctreeController::numberOfCells() const
{
    if(m_octree == NULL)
        return 0;

    return m_octree->size();
}

void OctreeController::addPoints(const CT_AbstractPointCloudIndex *index, QVector3D *min, QVector3D *max)
{
    if(!m_points.contains((CT_AbstractPointCloudIndex*)index)
            && !m_pointsToAdd.contains((CT_AbstractPointCloudIndex*)index))
    {
        PointsInfo info;

        if(min == NULL || max == NULL) {
            staticComputeMinMax(index, info);
        } else {
            info.m_min = *min;
            info.m_max = *max;
        }

        adjustNewMin(info.m_min);
        adjustNewMax(info.m_max);

        m_pointsToAdd.insert((CT_AbstractPointCloudIndex*)index, info);
        emit octreeMustBeReconstructed(true);
    }
}

void OctreeController::removePoints(const CT_AbstractPointCloudIndex *index)
{
    bool inOctree = m_points.remove((CT_AbstractPointCloudIndex*)index);

    if(inOctree)
    {
        removePoinsFromOctree(index);

        m_forceMustBeReconstructed = true;
        emit octreeMustBeReconstructed(true);
    }
    else
    {
        m_pointsToAdd.remove((CT_AbstractPointCloudIndex*)index);
    }
}

void OctreeController::indexOfPoint(const CT_Point &point, int &x, int &y, int &z)
{
    x = ((point.getX() - m_octreeMinCorner.x()) / m_size);
    y = ((point.getY() - m_octreeMinCorner.y()) / m_size);
    z = ((point.getZ() - m_octreeMinCorner.z()) / m_size);

    if(x == m_newNumberOfCells)
        x = m_newNumberOfCells-1;

    if(y == m_newNumberOfCells)
        y = m_newNumberOfCells-1;

    if(z == m_newNumberOfCells)
        z = m_newNumberOfCells-1;
}

bool OctreeController::hasElements() const
{
    return !m_points.isEmpty();
}

bool OctreeController::hasElementsAt(int x, int y, int z) const
{
    if(m_octree == NULL)
        return false;

    return (m_octree->at(x, y, z).data() != NULL);
}

const CT_AbstractCloudIndexT<CT_Point>* OctreeController::pointsAt(int x, int y, int z) const
{
    if(m_octree == NULL)
        return NULL;

    CT_AbstractModifiableCloudIndexRegisteredT<CT_Point> *m = m_octree->at(x, y, z).data();

    if(m == NULL)
        return NULL;

    return m->abstractCloudIndexT();
}

const CT_AbstractCloudIndex* OctreeController::at(int x, int y, int z) const
{
    return pointsAt(x, y, z);
}

bool OctreeController::isCellVisibleInFrustrum(int x, int y, int z, GLdouble planeCoefficients[6][4]) const
{
    if(m_octree == NULL)
        return false;

    double cellSize = cellsSize();

    QVector3D p1(m_octreeMinCorner.x()+(x*cellSize), m_octreeMinCorner.y()+(y*cellSize), m_octreeMinCorner.z()+(z*cellSize));
    QVector3D p2(m_octreeMinCorner.x()+((x+1)*cellSize), m_octreeMinCorner.y()+((y+1)*cellSize), m_octreeMinCorner.z()+((z+1)*cellSize));

    return aaBoxIsVisible(p1, p2, planeCoefficients);
}

bool OctreeController::isCellVisibleInFrustrum(int x, int y, int z, GLdouble planeCoefficients[6][4], bool &entirely) const
{
    if(m_octree == NULL)
        return false;

    double cellSize = cellsSize();

    QVector3D p1(m_octreeMinCorner.x()+(x*cellSize), m_octreeMinCorner.y()+(y*cellSize), m_octreeMinCorner.z()+(z*cellSize));
    QVector3D p2(m_octreeMinCorner.x()+((x+1)*cellSize), m_octreeMinCorner.y()+((y+1)*cellSize), m_octreeMinCorner.z()+((z+1)*cellSize));

    return aaBoxIsVisible(p1, p2, planeCoefficients, &entirely);
}

double OctreeController::cellsSize() const
{
    return m_size;
}

QVector3D OctreeController::octreeMinCorner() const
{
    return QVector3D(m_octreeMinCorner.x(), m_octreeMinCorner.y(), m_octreeMinCorner.z());
}

QVector3D OctreeController::octreeMaxCorner() const
{
    return QVector3D(m_octreeMaxCorner.x(), m_octreeMaxCorner.y(), m_octreeMaxCorner.z());
}

bool OctreeController::mustBeReconstructed() const
{
    return cornersChanged() || (m_newNumberOfCells != numberOfCells()) || !m_pointsToAdd.isEmpty();
}

bool OctreeController::cornersChanged() const
{
    return (m_min != m_newMin) || (m_max != m_newMax);
}

void OctreeController::construct()
{
    if(mustBeReconstructed())
    {
        m_forceMustBeReconstructed = false;

        delete m_octree;
        m_octree = new Octree< CT_Repository::CT_AbstractModifiablePCIR >(m_newNumberOfCells);

        if(m_newNumberOfCells > 0)
        {
            QHashIterator<CT_AbstractPointCloudIndex*, PointsInfo> itAdd(m_pointsToAdd);

            while(itAdd.hasNext())
            {
                itAdd.next();
                m_points.insert(itAdd.key(), itAdd.value());
            }

            double maxW = std::max(m_newMax.x() - m_newMin.x(), m_newMax.y() - m_newMin.y());
            maxW = std::max(maxW, m_newMax.z() - m_newMin.z());

            m_size = maxW / m_newNumberOfCells;

            QVector3D middle((m_newMax.x() + m_newMin.x())/2.0, (m_newMax.y() + m_newMin.y())/2.0, (m_newMax.z() + m_newMin.z())/2.0);
            m_octreeMaxCorner = Corner(middle.x()+(maxW/2.0), middle.y()+(maxW/2.0), middle.z()+(maxW/2.0));
            m_octreeMinCorner = Corner(middle.x()-(maxW/2.0), middle.y()-(maxW/2.0), middle.z()-(maxW/2.0));

            m_nPointsToAdd = 0;
            m_pointsAdded = 0;
            m_progress = 0;

            QHashIterator<CT_AbstractPointCloudIndex*, PointsInfo> it(m_points);

            while(it.hasNext())
                m_nPointsToAdd += it.next().key()->size();

            it.toFront();

            while(it.hasNext())
                addPointsToOctree(it.next().key());

            m_pointsToAdd.clear();

            m_min = m_newMin;
            m_max = m_newMax;

            emit octreeMustBeReconstructed(false);
        }
    }
}

void OctreeController::clear()
{
    m_points.clear();
    resetNewMinAndMax();
    m_min = m_newMin;
    m_max = m_newMax;
    m_newNumberOfCells = 0;
    m_size = 0;
    delete m_octree;
    m_octree = NULL;
}

float OctreeController::distanceToFrustumPlane(int index, const double &x, const double &y, const double &z, GLdouble planeCoefficients[6][4]) const
{
    return qglviewer::Vec(x, y, z) * qglviewer::Vec(planeCoefficients[index]) - planeCoefficients[index][3];
}

bool OctreeController::aaBoxIsVisible(const QVector3D &p1, const QVector3D &p2, GLdouble planeCoefficients[6][4], bool *entirely) const
{
    bool allInForAllPlanes = true;
    for (int i=0; i<6; ++i)
      {
        bool allOut = true;
        for (unsigned int c=0; c<8; ++c)
      {
        if (distanceToFrustumPlane(i, (c&4)?p1.x():p2.x(), (c&2)?p1.y():p2.y(), (c&1)?p1.z():p2.z(), planeCoefficients) > 0.0)
          allInForAllPlanes = false;
        else
          allOut = false;
      }

        // The eight points are on the outside side of this plane
        if (allOut)
      return false;
      }

    if (entirely)
      // Entirely visible : the eight points are on the inside side of the 6 planes
      *entirely = allInForAllPlanes;

    // Too conservative, but tangent cases are too expensive to detect
    return true;
}


void OctreeController::resetNewMinAndMax()
{
    m_newMin = QVector3D(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    m_newMax = QVector3D(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
}

void OctreeController::computeNewMinAndMax()
{
    resetNewMinAndMax();

    QHashIterator<CT_AbstractPointCloudIndex*, PointsInfo> it(m_points);

    while(it.hasNext())
    {
        PointsInfo info = it.next().value();

        adjustNewMin(info.m_min);
        adjustNewMax(info.m_max);
    }
}

void OctreeController::adjustNewMin(const OctreeController::Corner &min)
{
    if(m_newMin.x() > min.x())
        m_newMin.setX(min.x());

    if(m_newMin.y() > min.y())
        m_newMin.setY(min.y());

    if(m_newMin.z() > min.z())
        m_newMin.setZ(min.z());
}

void OctreeController::adjustNewMax(const OctreeController::Corner &max)
{
    if(m_newMax.x() < max.x())
        m_newMax.setX(max.x());

    if(m_newMax.y() < max.y())
        m_newMax.setY(max.y());

    if(m_newMax.z() < max.z())
        m_newMax.setZ(max.z());
}

void OctreeController::addPointsToOctree(const CT_AbstractPointCloudIndex *index)
{
    CT_AbstractPointCloudIndex::ConstIterator begin = index->constBegin();
    CT_AbstractPointCloudIndex::ConstIterator end = index->constEnd();

    int xPos, yPos, zPos;

    while(begin != end) {

        const CT_Point &p = begin.cT();
        const size_t &pIndex = begin.cIndex();

        indexOfPoint(p, xPos, yPos, zPos);

        CT_Repository::CT_AbstractModifiablePCIR pcir = (*m_octree)(xPos, yPos, zPos);

        if(pcir.data() == NULL)
        {
            CT_CloudIndexStdVectorT<CT_Point> *list = new CT_CloudIndexStdVectorT<CT_Point>();
            pcir = PS_REPOSITORY->registerPointCloudIndex(list);

            m_octree->set(xPos, yPos, zPos, pcir);
        }

        pcir->addIndex(pIndex);

        ++begin;
        ++m_pointsAdded;

        size_t progress = (m_pointsAdded * 100)/m_nPointsToAdd;

        if(m_progress != progress)
        {
            m_progress = progress;
            emit constructionInProgress(m_progress);
        }
    }
}

void OctreeController::removePoinsFromOctree(const CT_AbstractPointCloudIndex *index)
{
    Q_UNUSED(index)

    /*CT_AbstractPointCloudIndex::ConstIterator begin = index->constBegin();
    CT_AbstractPointCloudIndex::ConstIterator end = index->constEnd();

    int xPos, yPos, zPos;

    while(begin != end) {

        const CT_Point &p = begin.cT();
        const size_t &pIndex = begin.cIndex();

        indexOfPoint(p, xPos, yPos, zPos);

        CT_AbstractModifiableCloudIndexRegisteredT<CT_Point> *cir = (*m_octree)(xPos, yPos, zPos).data();

        if(cir != NULL)
        {
            CT_AbstractModifiableCloudIndexT<CT_Point> *list = cir->abstractModifiableCloudIndexT();

            list->removeIndex(pIndex);

            if(list->size() == 0)
                m_octree->erase(xPos, yPos, zPos);
        }

        ++begin;
    }*/

    construct();
}

void OctreeController::staticComputeMinMax(const CT_AbstractPointCloudIndex *index, OctreeController::PointsInfo &info)
{
    info.m_min.setX(std::numeric_limits<float>::max());
    info.m_min.setY(info.m_min.x());
    info.m_min.setZ(info.m_min.x());
    info.m_max.setX(-std::numeric_limits<float>::max());
    info.m_max.setY(info.m_max.x());
    info.m_max.setZ(info.m_max.x());

    CT_AbstractPointCloudIndex::ConstIterator begin = index->constBegin();
    CT_AbstractPointCloudIndex::ConstIterator end = index->constEnd();

    while(begin != end) {
        const CT_Point &p = begin.cT();

        if(p.getX() < info.m_min.x())
            info.m_min.setX(p.getX());

        if(p.getY() < info.m_min.y())
            info.m_min.setY(p.getY());

        if(p.getZ() < info.m_min.z())
           info. m_min.setZ(p.getZ());

        if(p.getX() > info.m_max.x())
            info.m_max.setX(p.getX());

        if(p.getY() > info.m_max.y())
            info.m_max.setY(p.getY());

        if(p.getZ() > info.m_max.z())
            info.m_max.setZ(p.getZ());

        ++begin;
    }
}
