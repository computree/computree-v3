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
    typedef osgFX::Effect* (*CreateNewEffectFunction)(osg::PrimitiveSet::Mode mode, void *context);

    typedef DM_PainterToOsgElements::ColorArrayType* (*GetOrCreateColorArrayFunction)(void *context);
    typedef DM_PainterToOsgElements::NormalArrayType* (*GetOrCreateNormalArrayFunction)(void *context);
    typedef osg::Array* (*GetOrCreateAttribArrayFunction)(void *context);

    DM_GeometriesConfiguration();

    /**
     * @brief Set the colors array that was sync with the global points cloud
     */
    void setFunctionToGetOrCreateGlobalColorArray(GetOrCreateColorArrayFunction func, void *context);

    /**
     * @brief Set the normals array that was sync with the global points cloud
     */
    void setFunctionToGetOrCreateGlobalNormalArray(GetOrCreateNormalArrayFunction func, void *context);

    /**
     * @brief Set the vertex attributes array that will be used by the shader for vertex of the global points cloud
     */
    void setFunctionToGetOrCreateGlobalVertexAttribArray(uint index, GetOrCreateAttribArrayFunction func, void *context);

    /**
     * @brief Set the vertex attributes array that will be used by the shader for local vertex
     */
    void setLocalVertexAttribArray(uint index, osg::Array *vAttribArray);

    /**
     * @brief Set a function to use to create a new osgFX::Effect when necessary for globals geometries
     */
    void setFunctionToCreateNewEffectForGlobal(CreateNewEffectFunction f, void *context);

    /**
     * @brief Set a function to use to create a new osgFX::Effect when necessary for globals geometries
     */
    void setFunctionToCreateNewEffectForLocal(CreateNewEffectFunction f, void *context);

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
     * @brief Set the state set to use with local geometries (points from circle, triangle, etc...)
     */
    void setLocalGeometriesStateSet(QList< osg::ref_ptr<osg::StateSet> > set);
    void setLocalGeometriesStateSetByPrimitiveSetMode(osg::PrimitiveSet::Mode mode, QList< osg::ref_ptr<osg::StateSet> > set);

    QList< osg::ref_ptr<osg::StateSet> > globalStateSet(osg::PrimitiveSet::Mode mode) const;
    QList< osg::ref_ptr<osg::StateSet> > localStateSet(osg::PrimitiveSet::Mode mode) const;

    DM_PainterToOsgElements::ColorArrayType* globalColorArray() const;
    DM_PainterToOsgElements::NormalArrayType* globalNormalArray() const;

    uint globalVertexAttribArrayLocationIndex() const;
    osg::Array* globalVertexAttribArray() const;

    uint localVertexAttribArrayLocationIndex() const;
    osg::Array* localVertexAttribArray() const;

private:
    template<typename FUNC>
    struct FunctionGetOrCreate
    {
        FunctionGetOrCreate() : function(NULL), context(NULL), location(0) {}

        FUNC function;
        void *context;
        uint location;
    };

    FunctionGetOrCreate<GetOrCreateColorArrayFunction>                      m_globalColorArray;
    FunctionGetOrCreate<GetOrCreateNormalArrayFunction>                     m_globalNormalArray;
    FunctionGetOrCreate<GetOrCreateAttribArrayFunction>                     m_globalVertexAttribArray;

    FunctionGetOrCreate<CreateNewEffectFunction>                            m_effectFunctionGlobal;
    FunctionGetOrCreate<CreateNewEffectFunction>                            m_effectFunctionLocal;

    QList< osg::ref_ptr<osg::StateSet> >                                    m_globalStateSet;
    QHash<osg::PrimitiveSet::Mode, QList< osg::ref_ptr<osg::StateSet> > >   m_globalStateSetByPrimitiveSetMode;

    QList< osg::ref_ptr<osg::StateSet> >                                    m_localStateSet;
    QHash<osg::PrimitiveSet::Mode, QList< osg::ref_ptr<osg::StateSet> > >   m_localStateSetByPrimitiveSetMode;
    uint                                                                    m_localVertexAttribArrayLocationIndex;
    osg::ref_ptr<osg::Array>                                                m_localVertexAttribArray;

    osg::ref_ptr< osg::Program >                                            m_shaderProgram;
};

#endif // DM_GEOMETRIESCONFIGURATION_H
