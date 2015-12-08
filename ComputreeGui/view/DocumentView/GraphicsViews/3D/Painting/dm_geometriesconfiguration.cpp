#include "dm_geometriesconfiguration.h"

DM_GeometriesConfiguration::DM_GeometriesConfiguration()
{
    m_localVertexAttribArray = NULL;
    m_shaderProgram = NULL;
}

void DM_GeometriesConfiguration::setFunctionToGetOrCreateGlobalColorArray(DM_GeometriesConfiguration::GetOrCreateColorArrayFunction func, void *context)
{
    m_globalColorArray.function = func;
    m_globalColorArray.context = context;
}

void DM_GeometriesConfiguration::setFunctionToGetOrCreateGlobalNormalArray(DM_GeometriesConfiguration::GetOrCreateNormalArrayFunction func, void *context)
{
    m_globalNormalArray.function = func;
    m_globalNormalArray.context = context;
}

void DM_GeometriesConfiguration::setFunctionToGetOrCreateGlobalVertexAttribArray(uint index, DM_GeometriesConfiguration::GetOrCreateAttribArrayFunction func, void *context)
{
    m_globalVertexAttribArray.location = index;
    m_globalVertexAttribArray.function = func;
    m_globalVertexAttribArray.context = context;
}

void DM_GeometriesConfiguration::setFunctionToCreateNewEffectForGlobal(DM_GeometriesConfiguration::CreateNewEffectFunction f, void *context)
{
    m_effectFunctionGlobal.function = f;
    m_effectFunctionGlobal.context = context;
}

void DM_GeometriesConfiguration::setFunctionToCreateNewEffectForLocal(DM_GeometriesConfiguration::CreateNewEffectFunction f, void *context)
{
    m_effectFunctionLocal.function = f;
    m_effectFunctionLocal.context = context;
}

osgFX::Effect* DM_GeometriesConfiguration::createGlobalEffect(osg::PrimitiveSet::Mode mode)
{
    if(m_effectFunctionGlobal.function != NULL) {
        return (*m_effectFunctionGlobal.function)(mode, m_effectFunctionGlobal.context);
    }

    return NULL;
}

osgFX::Effect *DM_GeometriesConfiguration::createLocalEffect(osg::PrimitiveSet::Mode mode)
{
    if(m_effectFunctionLocal.function != NULL) {
        return (*m_effectFunctionLocal.function)(mode, m_effectFunctionLocal.context);
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

DM_PainterToOsgElements::ColorArrayType *DM_GeometriesConfiguration::globalColorArray() const
{
    if(m_globalColorArray.function != NULL)
        return (*m_globalColorArray.function)(m_globalColorArray.context);

    return NULL;
}

DM_PainterToOsgElements::NormalArrayType *DM_GeometriesConfiguration::globalNormalArray() const
{
    if(m_globalNormalArray.function != NULL)
        return (*m_globalNormalArray.function)(m_globalNormalArray.context);

    return NULL;
}

uint DM_GeometriesConfiguration::globalVertexAttribArrayLocationIndex() const
{
    return m_globalVertexAttribArray.location;
}

osg::Array *DM_GeometriesConfiguration::globalVertexAttribArray() const
{
    if(m_globalVertexAttribArray.function != NULL)
        return (*m_globalVertexAttribArray.function)(m_globalVertexAttribArray.context);

    return NULL;
}

osg::Array *DM_GeometriesConfiguration::localVertexAttribArray() const
{
    return m_localVertexAttribArray;
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
