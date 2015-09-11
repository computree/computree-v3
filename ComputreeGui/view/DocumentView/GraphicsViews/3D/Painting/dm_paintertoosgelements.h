#ifndef DM_PAINTERTOOSGELEMENTS_H
#define DM_PAINTERTOOSGELEMENTS_H

#include "interfaces.h"

#include <QStack>
#include <QTime>
#include <QDebug>

#include <osg/Group>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osgSim/SphereSegment>

#include "ct_accessor/ct_faceaccessor.h"
#include "ct_accessor/ct_edgeaccessor.h"

#include "ct_colorcloud/ct_colorcloudosg.h"

class DM_GeometriesConfiguration;

struct DM_PainterToOsgElementsResult
{
    osg::ref_ptr<osg::Group>    m_rootGroup;
    QList<CT_MPCIR>             m_pointCloudIndexRegisteredCollection;
};

/**
 * @brief Class that will convert elements of an itemdrawable to osg elements (geode, etc...) to add it in a scene
 *
 * The structure will be like this :
 *
 * osg::Group (Result)
 *  |
 *  |-- osg::Group (Elements that can't be draw with the global vertex array managed by the pluginShared)
 *  |       |-- osg::Tranform (osg::PositionAttitudeTransform that represent a coordinate system, per example the coordinate system 0)
 *  |       |               |-- osg::Geode (A geode that will contains all drawables)
 *  |       |                   |-- osg::Drawable (Points per example)
 *  |       |                   |-- osg::Drawable (Lines per example)
 *  |       |                   |-- osg::Drawable (Triangles per example)
 *  |       |
 *  |       |-- osg::Tranform (osg::PositionAttitudeTransform that represent a coordinate system, per example the coordinate system 6)
 *  |                       |-- osg::Geode (A geode that will contains all drawables)
 *  |                           |-- osg::Drawable (Points per example)
 *  |                           |-- osg::Drawable (Lines per example)
 *  |                           |-- osg::Drawable (Triangles per example)
 *  |
 *  |-- osg::Group (Elements that can be draw with the global vertex array managed by the pluginShared)
 *  |       |-- osg::Tranform (osg::PositionAttitudeTransform that represent a coordinate system, per example the coordinate system 0)
 *  |       |               |-- osg::Geode (A geode that will contains all drawables)
 *  |       |                   |-- osg::Drawable (Points per example)
 *  |       |                   |-- osg::Drawable (Lines per example)
 *  |       |                   |-- osg::Drawable (Triangles per example)
 *  |       |
 *  |       |-- osg::Tranform (osg::PositionAttitudeTransform that represent a coordinate system, per example the coordinate system 6)
 *  |                       |-- osg::Geode (A geode that will contains all drawables)
 *  |                           |-- osg::Drawable (Points per example)
 *  |                           |-- osg::Drawable (Lines per example)
 *  |                           |-- osg::Drawable (Triangles per example)
 *  |
 *
 */
class DM_PainterToOsgElements : public PainterInterface
{
    Q_INTERFACES(PainterInterface)

public:
    static std::string NAME_GLOBAL_GEOMETRIES_GROUP;
    static std::string NAME_LOCAL_GEOMETRIES_GROUP;

    /**
     * @brief Color array type used by this painter
     */
    typedef CT_ColorCloudOsg::AType ColorArrayType;

//#define COLOR_ARRAY_FLOAT
#ifdef COLOR_ARRAY_FLOAT
    typedef osg::Vec4Array LocalColorArrayType;
#else
    typedef ColorArrayType LocalColorArrayType;
#endif

    DM_PainterToOsgElements();
    ~DM_PainterToOsgElements();

    /**
     * @brief Call this method to cancel the conversion
     */
    void cancel();

    /**
     * @brief Returns true if the painting was canceled
     */
    bool isCanceled() const;

    /**
     * @brief Call this method before get the results to finish conversion
     */
    void finish();

    /**
     * @brief Return the results of conversion
     * @warning call "finish" method before get results
     */
    DM_PainterToOsgElementsResult results() const;

    /**
     * @brief Set the configuration (shaders, vertex attrib, etc...) of geometries
     */
    void setGeometriesConfiguration(DM_GeometriesConfiguration *config);

    /**
     * @brief Enable/Disable the use of display list for all elements in draw
     */
    void setUseDisplayList(bool enable);

    // STATIC METHODS

    /**
     * @brief Change color of the itemdrawable represented by the osg::Group (result) recovered by the method "results()" of this class
     */
    static void staticChangeColorOfItemDrawableInResult(CT_AbstractItemDrawable *item, GraphicsViewInterface *gv, osg::Group *result, const QColor &color);

    /**
     * @brief Dirty display list of globals elements in osg::Group that represent a itemdrawable: result recovered by the method "results()" of this class
     *        the global color array
     */
    static void staticDirtyDisplayListOfGlobalElements(osg::Group *result);

    /**
     * @brief Dirty display list of locals elements in osg::Group that represent a itemdrawable: result recovered by the method "results()" of this class
     *        the global color array
     */
    static void staticDirtyDisplayListOfLocalElements(osg::Group *result);

    /**
     * @brief Dirty display list of all elements in osg::Group that represent a itemdrawable: result recovered by the method "results()" of this class
     *        the global color array
     */
    static void staticDirtyDisplayListOfAllElements(osg::Group *result);

    /**
     * @brief Get the group that contains recursively local geometries from a osg::Group that represent a itemdrawable: result recovered by the method "results()" of this class
     */
    static osg::Group* staticGetLocalGeometriesGroup(osg::Group *result);

    /**
     * @brief Get the group that contains recursively local geometries from a osg::Group that represent a itemdrawable: result recovered by the method "results()" of this class
     */
    static osg::Group* staticGetGlobalGeometriesGroup(osg::Group *result);

    /**
     * @brief Get the local vertex attrib array that was contained recursively in osg::Drawable from a osg::Group that represent a itemdrawable: result recovered by the method "results()" of this class
     */
    static osg::Array* staticGetLocalVertexAttribArray(osg::Group *result, const uint &locationIndex);

    // METHODS NECESSARY TO GET WHAT DRAW THE ITEMDRAWABLE

    /**
     * @brief Save the current color, pen, etc...
     */
    virtual void save();

    /**
     * @brief Restore the last color, pen, etc... that was saved
     */
    virtual void restore();

    /**
     * @brief Enable/Disable the "setColor" method for next draw
     */
    virtual void enableSetColor(bool enable);

    /**
     * @brief Returns true if "setColor" can change the color
     */
    bool isSetColorEnable() const;

    /**
     * @brief Change the color of elements that will be draw next
     */
    virtual void setColor(int r, int g, int b);

    /**
     * @brief Change the color of elements that will be draw next
     */
    virtual void setColor(QColor color);

    /**
     * @brief Returns the curren color used
     */
    virtual QColor getColor();

    /**
     * @brief Draw a point that was not in the global points cloud (otherwise use drawPoint(globalIndex) method)
     * @warning To get best performance don't mix this method with other method that don't draw point (like triangles or
     *          cubes or lines, etc...)
     */
    virtual void drawPoint(const double &x, const double &y, const double &z);

    /**
     * @brief Draw a point that was not in the global points cloud (otherwise use drawPoint(globalIndex) method)
     * @warning To get best performance don't mix this method with other method that don't draw point (like triangles or
     *          cubes or lines, etc...)
     */
    virtual void drawPoint(double *p);

    /**
     * @brief Draw a point that was in the global points cloud
     * @param globalIndex: index of the point in the global points cloud
     * @warning To get best performance don't mix this method with method that draw classical point that was not in the global cloud.
     *          To get best performance don't mix this method with other method that don't draw point from the global cloud (like triangles or
     *          cubes or lines, etc...)
     */
    virtual void drawPoint(const size_t &globalIndex);

    /**
     * @brief Draw all points that was pointed by indexes in "pci" in the global points cloud
     * @param pci : indexes of points to draw
     * @warning To get best performance don't mix this method with method that draw classical point that was not in the global cloud.
     *          To get best performance don't mix this method with other method that don't draw point from the global cloud (like triangles or
     *          cubes or lines, etc...)
     */
    virtual void drawPointCloud(const CT_AbstractCloudIndex *pci);

    /**
     * @brief Draw the octree and points (depend of the parameter modes)
     * @param octree : octree to draw
     * @param modes : what you want to draw => Cubes and/or Points
     */
    virtual void drawOctreeOfPoints(const OctreeInterface *octree, DrawOctreeModes modes);

    /**
     * @brief Draw a 3D cube defined by its bounding box (bottom left and top right corner). (NOT FILLED by default)
     * @warning To get best performance don't mix this method with other method that don't draw cube (like triangles or
     *          points or line, etc...)
     */
    virtual void drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2);

    /**
     * @brief Draw a 3D cube defined by its bounding box (bottom left and top right corner).
     * @param faces : Specifies the polygons that mode applies to. Must be GL_FRONT_AND_BACK for front- and back-facing polygons. Show glPolygonMode(...) for more details.
     * @param mode : Specifies how polygons will be rasterized. Accepted values are GL_POINT, GL_LINE, and GL_FILL. Show glPolygonMode(...) for more details.
     * @warning To get best performance don't mix this method with other method that don't draw cube or quads (like triangles or
     *          points or line, etc...).
     */
    virtual void drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2, GLenum faces, GLenum mode);

    /**
     * @brief Draw a not filled quad face in 3D
     * @warning To get best performance don't mix this method with other method that don't draw cube or quads (like triangles or
     *          points or line, etc...).
     */
    virtual void drawQuadFace( const double &x1, const double &y1, const double &z1,
                               const double &x2, const double &y2, const double &z2,
                               const double &x3, const double &y3, const double &z3,
                               const double &x4, const double &y4, const double &z4 );

    /**
     * @brief Draw a filled quad face in 3D
     * @warning To get best performance don't mix this method with other method that don't draw cube or quads (like triangles or
     *          points or line, etc...).
     */
    virtual void fillQuadFace( const double &x1, const double &y1, const double &z1,
                               const double &x2, const double &y2, const double &z2,
                               const double &x3, const double &y3, const double &z3,
                               const double &x4, const double &y4, const double &z4 );

    /**
     * @brief Draw a not filled quad face in 3D with associated colors
     * @warning To get best performance don't mix this method with other method that don't draw cube or quads (like triangles or
     *          points or line, etc...).
     */
    virtual void drawQuadFace( const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                               const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                               const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                               const double &x4, const double &y4, const double &z4, int r4, int g4, int b4 );

    /**
     * @brief Draw a filled quad face in 3D with associated colors
     * @warning To get best performance don't mix this method with other method that don't draw cube or quads (like triangles or
     *          points or line, etc...).
     */
    virtual void fillQuadFace( const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                               const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                               const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                               const double &x4, const double &y4, const double &z4, int r4, int g4, int b4 );

    /**
     * @brief Draw a rectangle in XY plane at level Z
     * @warning To get best performance don't mix this method with other method that don't draw cube or quads (like triangles or
     *          points or line, etc...).
     *          To get best performance prefer method "drawQuadFace".
     */
    virtual void drawRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z);

    /**
     * @brief Draw a filled rectangle in XY plane at level Z
     * @warning To get best performance don't mix this method with other method that don't draw cube or quads (like triangles or
     *          points or line, etc...).
     *          To get best performance prefer method "fillQuadFace".
     */
    virtual void fillRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z);


    /**
     * @brief Draw a rectangle in XZ plane at level Y
     * @warning To get best performance don't mix this method with other method that don't draw cube or quads (like triangles or
     *          points or line, etc...).
     *          To get best performance prefer method "drawQuadFace".
     */
    virtual void drawRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y);

    /**
     * @brief Draw a filled rectangle in XZ plane at level Y
     * @warning To get best performance don't mix this method with other method that don't draw cube or quads (like triangles or
     *          points or line, etc...).
     *          To get best performance prefer method "fillQuadFace".
     */
    virtual void fillRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y);


    /**
     * @brief Draw a rectangle in YZ plane at level X
     * @warning To get best performance don't mix this method with other method that don't draw cube or quads (like triangles or
     *          points or line, etc...).
     *          To get best performance prefer method "drawQuadFace".
     */
    virtual void drawRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x);

    /**
     * @brief Draw a filled rectangle in YZ plane at level X
     * @warning To get best performance don't mix this method with other method that don't draw cube or quads (like triangles or
     *          points or line, etc...).
     *          To get best performance prefer method "fillQuadFace".
     */
    virtual void fillRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x);

    /**
     * @brief Draw a line in 3D
     * @warning To get best performance don't mix this method with other method that don't draw lines (like triangles or
     *          points or quads or cubes, etc...).
     */
    virtual void drawLine(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2);

    /**
     * @brief Draw a line in 3D from points in the global cloud
     * @warning To get best performance don't mix this method with other method that don't draw lines (like triangles or
     *          points or quads or cubes, etc...).
     */
    virtual void drawLine(const size_t &p1GlobalIndex,
                          const size_t &p2GlobalIndex);

    /**
     * @brief Draw a triangle in 3D
     * @warning To get best performance don't mix this method with other method that don't draw triangles or faces (like lines or
     *          points or quads or cubes, etc...).
     */
    virtual void drawTriangle(const double &x1, const double &y1, const double &z1,
                              const double &x2, const double &y2, const double &z2,
                              const double &x3, const double &y3, const double &z3);

    /**
     * @brief Draw a triangle in 3D from points in the global cloud
     * @warning To get best performance don't mix this method with other method that don't draw triangles or faces (like lines or
     *          points or quads or cubes, etc...).
     */
    virtual void drawTriangle(const size_t &p1GlobalIndex,
                              const size_t &p2GlobalIndex,
                              const size_t &p3GlobalIndex);

    /**
     * @brief Draw a 2D circle (XY)
     * @warning To get best performance don't mix this method with other method that don't draw lines (like triangles or
     *          points or quads or cubes, etc...).
     */
    virtual void drawCircle(const double &x, const double &y, const double &z, const double &radius);

    /**
     * @brief Draw a 3D circle
     * @warning To get best performance don't mix this method with other method that don't draw lines (like triangles or
     *          points or quads or cubes, etc...).
     */
    virtual void drawCircle3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius);

    /**
     * @brief Draw a 2D ellipse (XY)
     * @warning To get best performance don't mix this method with other method that don't draw lines (like triangles or
     *          points or quads or cubes, etc...).
     */
    virtual void drawEllipse(const double &x, const double &y, const double &z, const double &radiusA, const double &radiusB);

    /**
     * @brief Call this method when you want to start to draw a polygon
     * @warning To get best performance don't mix this method with other method that don't draw lines (like triangles or
     *          points or quads or cubes, etc...).
     */
    virtual void beginPolygon();

    /**
     * @brief Call this method to add a point to the polygon
     * @warning To get best performance don't mix this method with other method that don't draw lines (like triangles or
     *          points or quads or cubes, etc...).
     */
    virtual void addPointToPolygon(const double &x, const double &y, const double &z);

    /**
     * @brief Call this method to finish the draw of the polygon
     */
    virtual void endPolygon();

    /**
     * @brief Draw a 2D cylinder (height is in Z axis)
     */
    virtual void drawCylinder(const double & x, const double & y, const double & z, const double & radius, const double & height);

    /**
     * @brief Draw a 3D cylinder
     */
    virtual void drawCylinder3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double & radius, const double & height);

    /**
     * @brief Draw a pyramid given its top point and its base
     */
    virtual void drawPyramid(const double &topX, const double &topY, const double &topZ,
                             const double &base1X, const double &base1Y, const double &base1Z,
                             const double &base2X, const double &base2Y, const double &base2Z,
                             const double &base3X, const double &base3Y, const double &base3Z,
                             const double &base4X, const double &base4Y, const double &base4Z);
    /**
     * @brief Draw a part of a sphere given the angles bounds
     */
    virtual void drawPartOfSphere (const double &centerX, const double &centerY, const double &centerZ,
                                   const double &radius,
                                   const double &initTheta, const double &endTheta,
                                   const double &initPhi, const double &endPhi,
                                   bool radians = true);

    /**
     * @brief Draw faces of a mesh
     * @warning To get best performance don't mix this method with other method that don't draw triangles or faces (like lines or
     *          points or quads or cubes, etc...).
     */
    virtual void drawMesh(const CT_AbstractMeshModel *mesh);

    /**
     * @brief Draw faces of a mesh
     * @warning To get best performance don't mix this method with other method that don't draw triangles or faces (like lines or
     *          points or quads or cubes, etc...).
     */
    virtual void drawFaces(const CT_AbstractMeshModel *mesh);

    /**
     * @brief Draw edges of a mesh
     * @warning To get best performance don't mix this method with other method that don't draw lines (like triangles or
     *          points or quads or cubes, etc...).
     */
    virtual void drawEdges(const CT_AbstractMeshModel *mesh);

    /**
     * @brief Draw points of a mesh
     *          To get best performance don't mix this method with other method that don't draw point (like triangles or
     *          cubes or lines, etc...)
     */
    virtual void drawPoints(const CT_AbstractMeshModel *mesh);

    // TODO : METHODS TO IMPLEMENT !
    virtual void enableSetPointSize(bool enable) {}
    virtual void setPointSize(float size) {}
    virtual void restoreDefaultPointSize() {}

    virtual void setPen(const QPen &pen) {}
    virtual void restoreDefaultPen() {}

private:
    typedef QHash<osg::PositionAttitudeTransform*, QHash<osg::PrimitiveSet::Mode, osg::Geometry*>* >            GeometryCollection;
    typedef QHashIterator<osg::PositionAttitudeTransform*, QHash<osg::PrimitiveSet::Mode, osg::Geometry*>* >    GeometryCollectionIterator;
    typedef QHash<uint, osg::PositionAttitudeTransform*>                                                        CoordinateSystemCollection;
    typedef osg::Vec3Array                                                                                      LocalVertexArray;

    class DM_OSGCylinder {
    public:
        DM_OSGCylinder(const QVector< QPair<double, double> > &cosSinAlpha);

        osg::Geometry* draw(DM_PainterToOsgElements &painter) const;
    private:
        QVector<Eigen::Vector4d>    m_v;
    };

    struct DM_PainterConfig {
        double                              m_pointSize;
        LocalColorArrayType::value_type     m_color;
        QPen                                m_pen;

    };

    DM_PainterToOsgElementsResult           m_result;
    QMap<std::string, osg::Group*>          m_groups;

    bool                                    m_cancel;

    bool                                    m_useDisplayList;

    /**
     * @brief Vertex array for elements that don't use the global point cloud
     */
    osg::Vec3Array                          *m_localVertexArray;

    /**
     * @brief Global vertex array or Global point cloud
     */
    osg::ref_ptr<osg::Array>                m_globalVertexArray;

    /**
     * @brief The geometries configuration (shaders, etc...)
     */
    DM_GeometriesConfiguration             *m_geometriesConfiguration;

    /**
     * @brief Collection of coordinate system already created for local vertex array
     */
    CoordinateSystemCollection              m_localCoordinateSystems;
    uint                                    m_localCoordinateSystemsLastUsedKey;
    osg::PositionAttitudeTransform*         m_localCoordinateSystemsLastUsedValue;

    /**
     * @brief Collection of coordinate system already created for global vertex array
     */
    CoordinateSystemCollection              m_globalCoordinateSystems;
    uint                                    m_globalCoordinateSystemsLastUsedKey;
    osg::PositionAttitudeTransform*         m_globalCoordinateSystemsLastUsedValue;

    /**
     * @brief Collection of geometries classed by transform and type of primitive (used for elements that don't use the global vertex array)
     */
    GeometryCollection                      m_localGeometries;

    /**
     * @brief Collection of geometries classed by transform and type of primitive (used for elements that use the global vertex array)
     */
    GeometryCollection                      m_globalGeometries;

    /**
     * @brief Used to access faces
     */
    CT_FaceAccessor                         m_faceAccessor;

    /**
     * @brief Used to access edges
     */
    CT_EdgeAccessor                         m_edgeAccessor;

    DM_PainterConfig                        m_currentConfig;
    QStack<DM_PainterConfig>                m_configStack;

    /**
     * @brief At 0 if setColor can change the color
     */
    int                                     m_nCallEnableSetColor;

    /**
     * @brief Color array of local geometries
     */
    LocalColorArrayType                     *m_localGeometriesColorArray;

    /**
     * @brief At true if at least one point was set for draw a polygon
     */
    bool                                    m_atLeastOnePointAddedForPolygon;

    /**
     * @brief The last point added to draw a polygon
     */
    osg::Vec3d                              m_polygonPreviousPointAdded;

    static QVector< QPair<double, double> > VECTOR_CIRCLE_FASTEST;
    static const int                        VECTOR_CIRCLE_FASTEST_SIZE = 20;

    static QVector< QPair<double, double> > VECTOR_CIRCLE_NORMAL;
    static const int                        VECTOR_CIRCLE_NORMAL_SIZE = 50;

    static DM_PainterToOsgElements::DM_OSGCylinder       CYLINDER_FASTEST;
    static DM_PainterToOsgElements::DM_OSGCylinder       CYLINDER_NORMAL;


    /**
     * @brief Init the cosinus and sinus for circle with "size" lines
     */
    static QVector< QPair<double, double> > staticInitCircleVector(int size);

    /**
     * @brief Init the cylinder with a list of cosinus and sinus
     */
    static DM_PainterToOsgElements::DM_OSGCylinder staticInitCylinder(const QVector< QPair<double, double> > &cosSinAlpha);

    /**
     * @brief Get or create the group associated with the name passed in parameter
     */
    osg::Group* getOrCreateGroup(const std::string &name);

    /**
     * @brief Return the vertex array that must be used by osg elements. If he don't already exist this method will create it before return it.
     * @param addToSize : number of elements that you want to add to the array
     * @param firstIndex : out parameters that will point to the first element you must modify
     */
    osg::Vec3Array* getOrCreateVertexArray_Local(size_t addToSize, size_t &firstIndex);

    /**
     * @brief Returns the transformation that must be used for a point that use the coordinate system passed in parameter
     */
    osg::PositionAttitudeTransform* getOrCreateTransformForCoordinateSystem_Global(CT_AbstractCoordinateSystem *cs);

    /**
     * @brief Returns the transformation that must be used for a point that is coordinates is x, y, z
     */
    osg::PositionAttitudeTransform* getOrCreateTransformForCoordinates_Local(const double &x, const double &y, const double &z);

    /**
     * @brief Returns the geometry that must be used for the type and transform passed in parameter
     */
    osg::Geometry* getOrCreateGeometryForTypeAndTransform_Global(osg::PrimitiveSet::Mode type, osg::PositionAttitudeTransform *transform);

    /**
     * @brief Returns the geometry that must be used for the type and transform passed in parameter
     */
    osg::Geometry* getOrCreateGeometryForTypeAndTransform_Local(osg::PrimitiveSet::Mode type, osg::PositionAttitudeTransform *transform);

    /**
     * @brief Returns the geometry that must be used for the type and transform passed in parameter
     */
    osg::Geometry* getOrCreateGeometryForTypeAndTransform_LocalAndGlobal(osg::PrimitiveSet::Mode type, osg::PositionAttitudeTransform *transform, GeometryCollection *geoCollectionToUse);

    /**
     * @brief Add the vertex index passed in parameter to the primitive set of the geometry passed in parameter
     */
    void addVertexIndexToGeometry_Global(const size_t &globalIndex, osg::Geometry *geo);

    /**
     * @brief Add the vertex passed in parameter to the vertex array and to the primitive set of the geometry passed in parameter
     */
    void addVertexToGeometryAndVertexArrayWithTransform_Local(const double &x, const double &y, const double &z, osg::Geometry *geo, osg::Vec3Array *vertexArray, const size_t &indexInVertexArray, osg::PositionAttitudeTransform *translation) const;

    /**
     * @brief Return the color array that must be used by osg elements. If he don't already exist this method will create it before return it.
     * @param addToSize : number of elements that you want to add to the array
     * @param firstIndex : out parameters that will point to the first element you must modify
     */
    LocalColorArrayType* getOrCreateColorArray_Local();
    LocalColorArrayType* getOrCreateColorArray_Local(size_t addToSize, size_t &firstIndex);

    /**
     * @brief Create the geometry and return it
     */
    osg::Geometry* createGeometry(osg::PrimitiveSet *primitiveSet, osg::PrimitiveSet::Mode primitiveSetMode, GeometryCollection *geoCollectionToUse);

    /**
     * @brief Add a new point to the geometry that draw points from global points cloud
     */
    void addNewPoint_Global(const size_t &globalIndex);

    /**
     * @brief Create or add a new line to the geometry that draw lines from global points cloud
     */
    void addNewLine_Global(const size_t &globalIndex1, const size_t &globalIndex2);

    /**
     * @brief Create or add a new triangle to the geometry that draw triangles from global points cloud
     */
    void addNewTriangle_Global(const size_t &globalIndex1, const size_t &globalIndex2, const size_t &globalIndex3);

    /**
     * @brief Create or add a new point to the geometry that draw points
     */
    void addNewPoint_Local(const double &x, const double &y, const double &z);

    /**
     * @brief Create or add a new line to the geometry that draw lines
     */
    void addNewLine_Local(const double &x1, const double &y1, const double &z1,
                          const double &x2, const double &y2, const double &z2);

    void addNewLine_Local(const double &x1, const double &y1, const double &z1,
                          const int &r1, const int &g1, const int &b1,
                          const double &x2, const double &y2, const double &z2,
                          const int &r2, const int &g2, const int &b2);

    void addNewLine_Local(const double &x1, const double &y1, const double &z1,
                          const LocalColorArrayType::value_type &c1,
                          const double &x2, const double &y2, const double &z2,
                          const LocalColorArrayType::value_type &c2);

    /**
     * @brief Create or add a new triangle to the geometry that draw triangles
     */
    void addNewTriangle_Local(const double &x1, const double &y1, const double &z1,
                              const double &x2, const double &y2, const double &z2,
                              const double &x3, const double &y3, const double &z3);

    void addNewTriangle_Local(const double &x1, const double &y1, const double &z1,
                              const int &r1, const int &g1, const int &b1, const int &a1,
                              const double &x2, const double &y2, const double &z2,
                              const int &r2, const int &g2, const int &b2, const int &a2,
                              const double &x3, const double &y3, const double &z3,
                              const int &r3, const int &g3, const int &b3, const int &a3);

    void addNewTriangle_Local(const double &x1, const double &y1, const double &z1,
                              const LocalColorArrayType::value_type &c1,
                              const double &x2, const double &y2, const double &z2,
                              const LocalColorArrayType::value_type &c2,
                              const double &x3, const double &y3, const double &z3,
                              const LocalColorArrayType::value_type &c3);

    /**
     * @brief Create or add a new arc to the geometry that draw arcs
     */
    void addNewArc_Local(const double &x, const double &y, const double &z,
                         const double &radius,
                         const double &startAngle, const double &arcAngle,
                         const Eigen::Vector3d &direction);

    /**
     * @brief Create or add a new arc to the geometry that draw arcs
     */
    void addNewEllipse_Local(const double &x, const double &y, const double &z,
                             const double &radius1, const double &radius2,
                             const Eigen::Vector3d &direction);

    /**
     * @brief Create or add a new filled or not quadface to the geometry that draw quads (with color = white)
     */
    void addNewQuadFace_Local(const double &x1, const double &y1, const double &z1,
                              const double &x2, const double &y2, const double &z2,
                              const double &x3, const double &y3, const double &z3,
                              const double &x4, const double &y4, const double &z4,
                              bool filled);

    /**
     * @brief Create or add a new colored and filled or not quad to the geometry that draw quads
     */
    void addNewQuadFace_Local(const double &x1, const double &y1, const double &z1,
                              const int &r1, const int &g1, const int &b1, const int &a1,
                              const double &x2, const double &y2, const double &z2,
                              const int &r2, const int &g2, const int &b2, const int &a2,
                              const double &x3, const double &y3, const double &z3,
                              const int &r3, const int &g3, const int &b3, const int &a3,
                              const double &x4, const double &y4, const double &z4,
                              const int &r4, const int &g4, const int &b4, const int &a4,
                              bool filled);

    void addNewQuadFace_Local(const double &x1, const double &y1, const double &z1,
                              const LocalColorArrayType::value_type &c1,
                              const double &x2, const double &y2, const double &z2,
                              const LocalColorArrayType::value_type &c2,
                              const double &x3, const double &y3, const double &z3,
                              const LocalColorArrayType::value_type &c3,
                              const double &x4, const double &y4, const double &z4,
                              const LocalColorArrayType::value_type &c4,
                              bool filled);

    /**
     * @brief Fill '?min' and '?max' with the minimum and maximum of values x1, y1, etc...
     */
    void getMinMax(double &xmin, double &xmax, double &ymin, double &ymax, double &zmin, double &zmax, double x1, double y1, double z1, double x2, double y2, double z2);

    /**
     * @brief Set the size of the vertex attrib array used by local geometries to the size of vertex array used by local geometries
     */
    void adjustSizeOfLocalVertexAttribArray();

    /**
     * @brief Check if the geometry passed in parameter has too many points and if yes create a new geometry, add it to
     *        the hashmap (m_globalGeometries) and return it. Otherwise return the geometry passed in parameter.
     */
    osg::Geometry* checkIfMustCreateANewDrawableForGlobalPointsAndDotItIfYes(osg::PositionAttitudeTransform *t, osg::Geometry *lastGeometry);

    /**
     * @brief Change the color in the first color array founded recursively in the group passed in parameter
     */
    static bool staticRecursiveChangeColorOfFirstColorArrayInGroup(osg::Group *node, const QColor &color, bool colorArrayAlreadyModified = false);

    // TODO
    static bool staticChangeColorOfCloudsOfFirstColorArrayInGroup(const QList<CT_AbstractCloudIndex*> &indexes, const QColor &color, osg::Group *node, bool colorArrayAlreadyModified = false);

    /**
     * @brief Dirty the display list of all drawable in the group recursively
     */
    static void staticRecursiveDirtyDisplayListOfDrawableInGroup(osg::Group *node);

    /**
     * @brief Search the firs vertex attrib array of drawable contained recursively in group
     */
    static osg::Array* staticRecursiveGetVertexAttribArray(osg::Group *node, const uint &index);

    /**
     * @brief Convert the internal color to float color
     */
    static osg::Vec4 staticInternalColorToFloatColor(const LocalColorArrayType::value_type &color);

    /**
     * @brief Convert the internal color to int color
     */
    static osg::Vec4ub staticInternalColorToIntColor(const LocalColorArrayType::value_type &color);

    /**
     * @brief Convert the int color to internal color
     */
    static LocalColorArrayType::value_type staticIntColorToInternalColor(const int &r, const int &g, const int &b, const int &a);

    static LocalColorArrayType::value_type staticQColorToInternalColor(const QColor &color);
    static QColor staticInternalColorToQColor(const LocalColorArrayType::value_type &color);
};

#endif // DM_PAINTERTOOSGELEMENTS_H
