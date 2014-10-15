#include "ct_coordinatesystemmanager.h"

#include "ct_global/ct_context.h"

CT_CoordinateSystemManager::CT_CoordinateSystemManager()
{
    addCoordinateSystem(new CT_DefaultCoordinateSystem());
    m_current = m_cs.first();
}

CT_CoordinateSystemManager::~CT_CoordinateSystemManager()
{
    qDeleteAll(m_cs.begin(), m_cs.end());
}

void CT_CoordinateSystemManager::addCoordinateSystem(CT_AbstractCoordinateSystem *cs)
{
    m_cs.append(cs);
}

bool CT_CoordinateSystemManager::removeCoordinateSystem(CT_AbstractCoordinateSystem *cs)
{
    if((cs == m_cs.first()) || ((cs == m_current) && !staticCanChangeOrModifyCoordinateSystem()))
        return false;

    m_cs.removeOne(cs);

    if(cs == m_current)
        m_current = m_cs.first();

    delete cs;
    return true;
}

bool CT_CoordinateSystemManager::removeCoordinateSystem(int index)
{
    return removeCoordinateSystem(m_cs.at(index));
}

int CT_CoordinateSystemManager::size() const
{
    return m_cs.size();
}

CT_AbstractCoordinateSystem* CT_CoordinateSystemManager::coordinateSystemAt(int index) const
{
    return m_cs.at(index);
}

bool CT_CoordinateSystemManager::setCurrentCoordinateSystem(CT_AbstractCoordinateSystem *cs)
{
    return setCurrentCoordinateSystem(m_cs.indexOf(cs));
}

bool CT_CoordinateSystemManager::setCurrentCoordinateSystem(int index)
{
    if(!staticCanChangeOrModifyCoordinateSystem())
        return false;

    m_current = m_cs.at(index);
    return true;
}

CT_AbstractCoordinateSystem* CT_CoordinateSystemManager::currentCoordinateSystem() const
{
    return m_current;
}

bool CT_CoordinateSystemManager::staticCanChangeOrModifyCoordinateSystem()
{
    return (PS_REPOSITORY->globalCloud<CT_Point>()->size() == 0);
}

void CT_CoordinateSystemManager::initUsedOfAllCoordinateSystem()
{
    QListIterator<CT_AbstractCoordinateSystem*> it(m_cs);

    while(it.hasNext())
        it.next()->initUsed();
}
