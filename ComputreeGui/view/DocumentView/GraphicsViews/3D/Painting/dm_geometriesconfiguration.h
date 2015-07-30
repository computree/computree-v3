#ifndef DM_GEOMETRIESCONFIGURATION_H
#define DM_GEOMETRIESCONFIGURATION_H

#include <osg/StateSet>
#include <osg/Program>
#include <osg/Array>

#include "view/DocumentView/GraphicsViews/3D/Painting/dm_paintertoosgelements.h"

/**
 * @brief Class that contains elements like state set (shaders) and array for configure local and global geometries created by the painter (DM_PainterToOsgElements)
 */
class DM_GeometriesConfiguration
{
public:
    DM_GeometriesConfiguration();

    /**
     * @brief Set the colors array that was sync with the global points cloud
     */
    void setGlobalColorArray(DM_PainterToOsgElements::ColorArrayType *colors);

    /**
     * @brief Set the state set to use with global geometries (points from global points cloud)
     */
    void setGlobalGeometriesStateSet(osg::StateSet *set);

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
    void setLocalGeometriesStateSet(osg::StateSet *set);

    /**
     * @brief Set the vertex attributes array that will be used by the shader for local vertex
     */
    void setLocalVertexAttribArray(uint index, osg::Array *vAttribArray);

    /**
     * @brief Set the location index of the local color array that will be pass like a vertex attribute array
     */
    void setLocalColorVertexAttribArrayLocationIndex(uint index);

    DM_PainterToOsgElements::ColorArrayType* globalColorArray() const;

    uint globalVertexAttribArrayLocationIndex() const;
    osg::Array* globalVertexAttribArray() const;

    uint localVertexAttribArrayLocationIndex() const;
    osg::Array* localVertexAttribArray() const;

    uint localColorVertexAttribArrayLocationIndex() const;
    uint globalColorVertexAttribArrayLocationIndex() const;

    osg::StateSet* globalStateSet() const;
    osg::StateSet* localStateSet() const;

private:
    osg::ref_ptr< DM_PainterToOsgElements::ColorArrayType >                 m_globalColorArray;

    osg::ref_ptr<osg::StateSet>                                             m_globalStateSet;
    uint                                                                    m_globalVertexAttribArrayLocationIndex;
    osg::ref_ptr<osg::Array>                                                m_globalVertexAttribArray;

    osg::ref_ptr<osg::StateSet>                                             m_localStateSet;
    uint                                                                    m_localVertexAttribArrayLocationIndex;
    osg::ref_ptr<osg::Array>                                                m_localVertexAttribArray;

    uint                                                                    m_globalColorVertexAttribArrayLocationIndex;
    uint                                                                    m_localColorVertexAttribArrayLocationIndex;

    osg::ref_ptr< osg::Program >                                            m_shaderProgram;
};

#endif // DM_GEOMETRIESCONFIGURATION_H
