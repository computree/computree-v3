#include "dm_geometriesconfiguration.h"

DM_GeometriesConfiguration::DM_GeometriesConfiguration()
{
    m_globalColorArray = NULL;
    m_globalNormalArray = NULL;
    m_globalVertexAttribArray = NULL;
    m_localVertexAttribArray = NULL;
    m_shaderProgram = NULL;
    m_globalStateSet = NULL;
    m_localStateSet = NULL;
}

void DM_GeometriesConfiguration::setGlobalColorArray(DM_PainterToOsgElements::ColorArrayType *colors)
{
    m_globalColorArray = colors;
}

void DM_GeometriesConfiguration::setGlobalNormalArray(DM_PainterToOsgElements::NormalArrayType *normals)
{
    m_globalNormalArray = normals;
}

void DM_GeometriesConfiguration::setGlobalGeometriesStateSet(osg::StateSet *set)
{
    m_globalStateSet = set;
}

void DM_GeometriesConfiguration::setGlobalGeometriesStateSetByPrimitiveSetMode(osg::PrimitiveSet::Mode mode, osg::StateSet *set)
{
    m_globalStateSetByPrimitiveSetMode.insert(mode, set);
}

void DM_GeometriesConfiguration::setGlobalVertexAttribArray(uint index, osg::Array *vAttribArray)
{
    m_globalVertexAttribArrayLocationIndex = index;
    m_globalVertexAttribArray = vAttribArray;
}

void DM_GeometriesConfiguration::setGlobalColorVertexAttribArrayLocationIndex(uint index)
{
    m_globalColorVertexAttribArrayLocationIndex = index;
}

void DM_GeometriesConfiguration::setLocalGeometriesStateSet(osg::StateSet *set)
{
    m_localStateSet = set;
}

void DM_GeometriesConfiguration::setLocalGeometriesStateSetByPrimitiveSetMode(osg::PrimitiveSet::Mode mode, osg::StateSet *set)
{
    m_localStateSetByPrimitiveSetMode.insert(mode, set);
}

void DM_GeometriesConfiguration::setLocalVertexAttribArray(uint index, osg::Array *vAttribArray)
{
    m_localVertexAttribArrayLocationIndex = index;
    m_localVertexAttribArray = vAttribArray;
}

void DM_GeometriesConfiguration::setLocalColorVertexAttribArrayLocationIndex(uint index)
{
    m_localColorVertexAttribArrayLocationIndex = index;
}

DM_PainterToOsgElements::ColorArrayType *DM_GeometriesConfiguration::globalColorArray() const
{
    return m_globalColorArray.get();
}

DM_PainterToOsgElements::NormalArrayType *DM_GeometriesConfiguration::globalNormalArray() const
{
    return m_globalNormalArray.get();
}

uint DM_GeometriesConfiguration::globalVertexAttribArrayLocationIndex() const
{
    return m_globalVertexAttribArrayLocationIndex;
}

osg::Array *DM_GeometriesConfiguration::globalVertexAttribArray() const
{
    return m_globalVertexAttribArray.get();
}

osg::Array *DM_GeometriesConfiguration::localVertexAttribArray() const
{
    return m_localVertexAttribArray;
}

uint DM_GeometriesConfiguration::localColorVertexAttribArrayLocationIndex() const
{
    return m_localColorVertexAttribArrayLocationIndex;
}

uint DM_GeometriesConfiguration::globalColorVertexAttribArrayLocationIndex() const
{
    return m_globalColorVertexAttribArrayLocationIndex;
}

uint DM_GeometriesConfiguration::localVertexAttribArrayLocationIndex() const
{
    return m_localVertexAttribArrayLocationIndex;
}

osg::StateSet* DM_GeometriesConfiguration::globalStateSet(osg::PrimitiveSet::Mode mode) const
{
    osg::StateSet *set = m_globalStateSetByPrimitiveSetMode.value(mode, NULL);

    if(set != NULL)
        return set;

    return m_globalStateSet;
}

osg::StateSet* DM_GeometriesConfiguration::localStateSet(osg::PrimitiveSet::Mode mode) const
{
    osg::StateSet *set = m_localStateSetByPrimitiveSetMode.value(mode, NULL);

    if(set != NULL)
        return set;

    return m_localStateSet;
}
