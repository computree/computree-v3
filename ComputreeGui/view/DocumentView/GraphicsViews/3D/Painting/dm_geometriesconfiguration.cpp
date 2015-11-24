#include "dm_geometriesconfiguration.h"

DM_GeometriesConfiguration::DM_GeometriesConfiguration()
{
    m_effectFunctionGlobal.first = NULL;
    m_effectFunctionLocal.first = NULL;
    m_globalColorArray = NULL;
    m_globalNormalArray = NULL;
    m_globalVertexAttribArray = NULL;
    m_localVertexAttribArray = NULL;
    m_shaderProgram = NULL;
}

void DM_GeometriesConfiguration::setGlobalColorArray(DM_PainterToOsgElements::ColorArrayType *colors)
{
    m_globalColorArray = colors;
}

void DM_GeometriesConfiguration::setGlobalNormalArray(DM_PainterToOsgElements::NormalArrayType *normals)
{
    m_globalNormalArray = normals;
}

void DM_GeometriesConfiguration::setFunctionToCreateNewEffectForGlobal(DM_GeometriesConfiguration::createNewEffectFunction f, void *context)
{
    m_effectFunctionGlobal.first = f;
    m_effectFunctionGlobal.second = context;
}

void DM_GeometriesConfiguration::setFunctionToCreateNewEffectForLocal(DM_GeometriesConfiguration::createNewEffectFunction f, void *context)
{
    m_effectFunctionLocal.first = f;
    m_effectFunctionLocal.second = context;
}

osgFX::Effect* DM_GeometriesConfiguration::createGlobalEffect(osg::PrimitiveSet::Mode mode)
{
    if(m_effectFunctionGlobal.first != NULL) {
        return (*m_effectFunctionGlobal.first)(mode, m_effectFunctionGlobal.second);
    }

    return NULL;
}

osgFX::Effect *DM_GeometriesConfiguration::createLocalEffect(osg::PrimitiveSet::Mode mode)
{
    if(m_effectFunctionLocal.first != NULL) {
        return (*m_effectFunctionLocal.first)(mode, m_effectFunctionLocal.second);
    }

    return NULL;
}

void DM_GeometriesConfiguration::setGlobalGeometriesStateSet(QList<osg::ref_ptr<osg::StateSet> > set)
{
    m_globalStateSet = set;
}

void DM_GeometriesConfiguration::setGlobalGeometriesStateSetByPrimitiveSetMode(osg::PrimitiveSet::Mode mode, QList<osg::ref_ptr<osg::StateSet> > set)
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

void DM_GeometriesConfiguration::setLocalGeometriesStateSet(QList<osg::ref_ptr<osg::StateSet> > set)
{
    m_localStateSet = set;
}

void DM_GeometriesConfiguration::setLocalGeometriesStateSetByPrimitiveSetMode(osg::PrimitiveSet::Mode mode, QList<osg::ref_ptr<osg::StateSet> > set)
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

QList< osg::ref_ptr<osg::StateSet> > DM_GeometriesConfiguration::globalStateSet(osg::PrimitiveSet::Mode mode) const
{
    QList< osg::ref_ptr<osg::StateSet> > set = m_globalStateSetByPrimitiveSetMode.value(mode);

    if(!set.isEmpty())
        return set;

    return m_globalStateSet;
}

QList< osg::ref_ptr<osg::StateSet> > DM_GeometriesConfiguration::localStateSet(osg::PrimitiveSet::Mode mode) const
{
    QList< osg::ref_ptr<osg::StateSet> > set = m_localStateSetByPrimitiveSetMode.value(mode);

    if(!set.isEmpty())
        return set;

    return m_localStateSet;
}
