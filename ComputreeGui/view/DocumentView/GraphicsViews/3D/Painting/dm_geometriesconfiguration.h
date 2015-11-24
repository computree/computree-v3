#ifndef DM_GEOMETRIESCONFIGURATION_H
#define DM_GEOMETRIESCONFIGURATION_H

#include <osg/StateSet>
#include <osg/Program>
#include <osg/Array>
#include <osgFX/Effect>

#include "view/DocumentView/GraphicsViews/3D/Painting/dm_paintertoosgelements.h"

/**
 * @brief Class that contains elements like state set (shaders) and array for configure local and global geometries created by the painter (DM_PainterToOsgElements)
 */
class DM_GeometriesConfiguration
{
public:
    typedef osgFX::Effect* (*createNewEffectFunction)(osg::PrimitiveSet::Mode mode, void *context);

    DM_GeometriesConfiguration();

    /**
     * @brief Set the colors array that was sync with the global points cloud
     */
    void setGlobalColorArray(DM_PainterToOsgElements::ColorArrayType *colors);

    /**
     * @brief Set the normals array that was sync with the global points cloud
     */
    void setGlobalNormalArray(DM_PainterToOsgElements::NormalArrayType *normals);

    /**
     * @brief Set a function to use to create a new osgFX::Effect when necessary for globals geometries
     */
    void setFunctionToCreateNewEffectForGlobal(createNewEffectFunction f, void *context);

    /**
     * @brief Set a function to use to create a new osgFX::Effect when necessary for globals geometries
     */
    void setFunctionToCreateNewEffectForLocal(createNewEffectFunction f, void *context);

    /**
     * @brief Create a Effect for a specific type of global geometries
     */
    osgFX::Effect* createGlobalEffect(osg::PrimitiveSet::Mode mode);

    /**
     * @brief Create a Effect for a specific type of local geometries
     */
    osgFX::Effect* createLocalEffect(osg::PrimitiveSet::Mode mode);

    /**
     * @brief Set the state set to use with global geometries (points from global points cloud)
     */
    void setGlobalGeometriesStateSet(QList< osg::ref_ptr<osg::StateSet> > set);
    void setGlobalGeometriesStateSetByPrimitiveSetMode(osg::PrimitiveSet::Mode mode, QList< osg::ref_ptr<osg::StateSet> > set);

    /**
     * @brief Set the vertex attributes array that will be used by the shader for vertex of the global points cloud
     */
    void setGlobalVertexAttribArray(uint index, osg::Array *vAttribArray);

    /**
     * @brief Set the location index of the global color array that will be pass like a vertex attribute array
     */
    void setGlobalColorVertexAttribArrayLocationIndex(uint index);

    /**
     * @brief Set the state set to use with local geometries (points from circle, triangle, etc...)
     */
    void setLocalGeometriesStateSet(QList< osg::ref_ptr<osg::StateSet> > set);
    void setLocalGeometriesStateSetByPrimitiveSetMode(osg::PrimitiveSet::Mode mode, QList< osg::ref_ptr<osg::StateSet> > set);

    QList< osg::ref_ptr<osg::StateSet> > globalStateSet(osg::PrimitiveSet::Mode mode) const;
    QList< osg::ref_ptr<osg::StateSet> > localStateSet(osg::PrimitiveSet::Mode mode) const;

    /**
     * @brief Set the vertex attributes array that will be used by the shader for local vertex
     */
    void setLocalVertexAttribArray(uint index, osg::Array *vAttribArray);

    /**
     * @brief Set the location index of the local color array that will be pass like a vertex attribute array
     */
    void setLocalColorVertexAttribArrayLocationIndex(uint index);

    DM_PainterToOsgElements::ColorArrayType* globalColorArray() const;
    DM_PainterToOsgElements::NormalArrayType* globalNormalArray() const;

    uint globalVertexAttribArrayLocationIndex() const;
    osg::Array* globalVertexAttribArray() const;

    uint localVertexAttribArrayLocationIndex() const;
    osg::Array* localVertexAttribArray() const;

    uint localColorVertexAttribArrayLocationIndex() const;
    uint globalColorVertexAttribArrayLocationIndex() const;

private:
    osg::ref_ptr< DM_PainterToOsgElements::ColorArrayType >                 m_globalColorArray;
    osg::ref_ptr< DM_PainterToOsgElements::NormalArrayType >                m_globalNormalArray;

    QPair<createNewEffectFunction, void*>                                   m_effectFunctionGlobal;
    QPair<createNewEffectFunction, void*>                                   m_effectFunctionLocal;

    QList< osg::ref_ptr<osg::StateSet> >                                    m_globalStateSet;
    QHash<osg::PrimitiveSet::Mode, QList< osg::ref_ptr<osg::StateSet> > >   m_globalStateSetByPrimitiveSetMode;
    uint                                                                    m_globalVertexAttribArrayLocationIndex;
    osg::ref_ptr<osg::Array>                                                m_globalVertexAttribArray;

    QList< osg::ref_ptr<osg::StateSet> >                                    m_localStateSet;
    QHash<osg::PrimitiveSet::Mode, QList< osg::ref_ptr<osg::StateSet> > >   m_localStateSetByPrimitiveSetMode;
    uint                                                                    m_localVertexAttribArrayLocationIndex;
    osg::ref_ptr<osg::Array>                                                m_localVertexAttribArray;

    uint                                                                    m_globalColorVertexAttribArrayLocationIndex;
    uint                                                                    m_localColorVertexAttribArrayLocationIndex;

    osg::ref_ptr< osg::Program >                                            m_shaderProgram;
};

#endif // DM_GEOMETRIESCONFIGURATION_H
