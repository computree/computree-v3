#include "ct_coordinatesystemmanager.h"

#include "ct_global/ct_context.h"
#include "ct_coordinates/ct_defaultcoordinatesystem.h"

CT_CoordinateSystemManager::CT_CoordinateSystemManager()
{
    // create a new cloud that contains index of coordinate system for all points. It will be synchronized with the size of the global point cloud
    m_indexOfCoordinateSystemOfPoints = PS_REPOSITORY->createNewCloudT< CT_StdCloudRegisteredT< CT_CoordinateSystemCloudIndex >, CT_CoordinateSystemCloudIndex >(CT_Repository::SyncWithPointCloud);

    m_default = registerCoordinateSystem(new CT_DefaultCoordinateSystem(0, 0, 0));
}

CT_CoordinateSystemManager::~CT_CoordinateSystemManager()
{
}

QSharedPointer<CT_AbstractCoordinateSystem> CT_CoordinateSystemManager::registerCoordinateSystem(CT_AbstractCoordinateSystem *cs)
{
    m_cs.append(cs);
    cs->initUsed();

    setCurrentCoordinateSystem(size()-1);

    return QSharedPointer<CT_AbstractCoordinateSystem>(cs, CT_CoordinateSystemManager::staticUnregisterCoordinateSystem);
}

bool CT_CoordinateSystemManager::removeCoordinateSystem(CT_AbstractCoordinateSystem *cs)
{
    return removeCoordinateSystem(indexOfCoordinateSystem(cs));
}

bool CT_CoordinateSystemManager::removeCoordinateSystem(int index)
{
    if(index == 0)
        return false;

    // if it was other coordinate system after this, we must update all indexes contained in the cloud
    if(index < m_cs.size()-1)
        m_indexOfCoordinateSystemOfPoints->cloudT()->decreaseAfterValue(index);

    CT_AbstractCoordinateSystem *cs = m_cs.takeAt(index);

    if(cs == m_current)
        setCurrentCoordinateSystem(0);

    delete cs;
    return true;
}

int CT_CoordinateSystemManager::size() const
{
    return m_cs.size();
}

GLuint CT_CoordinateSystemManager::indexOfCoordinateSystem(const CT_AbstractCoordinateSystem *cs) const
{
    int i = m_cs.indexOf((CT_AbstractCoordinateSystem*)cs);

    if(i < 0)
        return 0;

    return i;
}

CT_AbstractCoordinateSystem* CT_CoordinateSystemManager::coordinateSystemAt(const GLuint &index) const
{
    return m_cs.at(index);
}

CT_AbstractCoordinateSystem* CT_CoordinateSystemManager::coordinateSystemForPointAt(const size_t &globalIndex) const
{
    return coordinateSystemAt(m_indexOfCoordinateSystemOfPoints->cloudT()->valueAt(globalIndex));
}

GLuint CT_CoordinateSystemManager::coordinateSystemIndexForPointAt(const size_t &globalIndex) const
{
    return m_indexOfCoordinateSystemOfPoints->cloudT()->valueAt(globalIndex);
}

void CT_CoordinateSystemManager::setCoordinateSystemForPointAt(const size_t &globalIndex, const GLuint &coordinateSystemIndex) const
{
    m_indexOfCoordinateSystemOfPoints->cloudT()->replaceValueAt(globalIndex, coordinateSystemIndex);
}


void CT_CoordinateSystemManager::setCoordinateSystemForPoints(QSharedPointer<CT_AbstractCloudIndexRegisteredT<CT_Point> > pcir, CT_AbstractCoordinateSystem *cs)
{
    if(cs != NULL)
        setCoordinateSystemIndexForPoints(pcir, indexOfCoordinateSystem(cs));
}

void CT_CoordinateSystemManager::setCoordinateSystemIndexForPoints(QSharedPointer<CT_AbstractCloudIndexRegisteredT<CT_Point> > pcir, const GLuint &coordinateSystemIndex)
{
    if(!pcir.isNull()) {

        CT_AbstractCloudIndexT<CT_Point>::ConstIterator it = pcir->abstractCloudIndexT()->constBegin();
        CT_AbstractCloudIndexT<CT_Point>::ConstIterator end = pcir->abstractCloudIndexT()->constEnd();

        while(it != end) {
            m_indexOfCoordinateSystemOfPoints->cloudT()->replaceValueAt(it.cIndex(), coordinateSystemIndex);
            ++it;
        }
    }
}

void CT_CoordinateSystemManager::setCurrentCoordinateSystem(CT_AbstractCoordinateSystem *cs)
{
    if(cs == NULL)
        setCurrentCoordinateSystem(0);
    else
        setCurrentCoordinateSystem(indexOfCoordinateSystem(cs));
}

void CT_CoordinateSystemManager::setCurrentCoordinateSystem(int index)
{
    m_current = m_cs.at(index);

    m_indexOfCoordinateSystemOfPoints->cloudT()->setResizeNewValue(index);
}

CT_AbstractCoordinateSystem* CT_CoordinateSystemManager::currentCoordinateSystem() const
{
    return m_current;
}

bool CT_CoordinateSystemManager::wasAtLeastOneUsed() const
{
    QVectorIterator<CT_AbstractCoordinateSystem*> it(m_cs);

    while(it.hasNext()) {
        if(it.next()->wasUsed())
            return true;
    }

    return false;
}

CT_CoordinateSystemCloudIndex* CT_CoordinateSystemManager::indexCloudOfCoordinateSystemOfPoints() const
{
    return m_indexOfCoordinateSystemOfPoints->cloudT();
}

void CT_CoordinateSystemManager::staticUnregisterCoordinateSystem(CT_AbstractCoordinateSystem *cs)
{
    PS_COORDINATES_SYS_MANAGER->removeCoordinateSystem(cs);
}

void CT_CoordinateSystemManager::initUsedOfAllCoordinateSystem()
{
    QVectorIterator<CT_AbstractCoordinateSystem*> it(m_cs);

    while(it.hasNext())
        it.next()->initUsed();

    setCurrentCoordinateSystem(0);
}
