/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QVariantList>
#include <QColor>
#include <QSharedPointer>
#include <QWidget>
#include <QPainter>
#include <QAbstractItemView>

#include <eigen/Eigen/Core>

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#include <GL/gl.h>
#else
#include <qopengl.h>
#endif

#include "interfacessettings.h"

class QPen;
class QRectF;

class DocumentInterface;
class GraphicsViewInterface;
class PainterInterface;
class ActionsManagerInterface;
class GraphicsViewInterface;
class ActionOptionsInterface;
class LogInterface;
class PluginManagerInterface;
class PluginEntryInterface;

class CT_VirtualAbstractStep;
class CT_AbstractStepPlugin;
class CT_AbstractResult;
class CT_AbstractItemDrawable;
class CT_AbstractAction;
class CT_AbstractLogListener;
class CT_AbstractMeshModel;
class CT_AbstractPointCloud;
class CT_AbstractCloudIndex;
class CT_OutAbstractModel;
class CT_StandardColorCloudRegistered;
class CT_StandardNormalCloudRegistered;
class CT_AbstractModifiableCloudIndexRegistered;
class CT_AbstractModifiableCloudIndex;

/**
 * @brief Represent an octree used by graphics views
 */
class OctreeInterface  : public QObject
{
    Q_OBJECT

public:
    virtual ~OctreeInterface() {}

    /**
     * @brief Returns number of cells between min and max
     */
    virtual int numberOfCells() const = 0;

    /**
     * @brief Returns true if the octree has points
     */
    virtual bool hasElements() const = 0;

    /**
     * @brief Returns true if the octree has elements in the cell at x/y/z
     */
    virtual bool hasElementsAt(int x, int y, int z) const = 0;

    /**
     * @brief Returns cloud index in cell at x/y/z. NULL if cell is empty.
     */
    virtual const CT_AbstractCloudIndex* at(int x, int y, int z) const = 0;

    /**
     * @brief Returns true if the cell at x, y, z is visible in frustrum
     */
    virtual bool isCellVisibleInFrustrum(int x, int y, int z, GLdouble m_planeCoefficients[6][4]) const = 0;

    /**
     * @brief Returns the cells size
     */
    virtual double cellsSize() const = 0;

    /**
     * @brief The min corner of the octree
     */
    virtual QVector3D octreeMinCorner() const = 0;

    /**
     * @brief The max corner of the octree
     */
    virtual QVector3D octreeMaxCorner() const = 0;

    /**
     * @brief Returns true if the octree must be reconstructed. (If new min or max is greather or lower than old min or max OR if number of cells has changed)
     */
    virtual bool mustBeReconstructed() const = 0;

signals:
    /**
     * @brief Emit when the octree must be reconstructed
     */
    void octreeMustBeReconstructed(bool val);

    /**
     * @brief Emit when the reconstruction progress changed
     */
    void constructionInProgress(int val);
};

/*!
 *  \brief Représente un objet qui doit être utilisé par les ItemDrawable
 *         pour dessiner leur élements 3D/2D.
 */
class PainterInterface
{
public:
    enum TYPE_VERTEX
    {
        VTYPE_FLOAT
    };

    enum DRAW_VERTEX_MODE
    {
        VDRAW_POINTS
    };

    enum TYPE_INDEX
    {
        ITYPE_UNSIGNED_INT
    };

    enum DrawOctreeMode {
        DrawOctree = 1,
        DrawElements = 2
    };

    Q_DECLARE_FLAGS(DrawOctreeModes, DrawOctreeMode)

    virtual ~PainterInterface() {}

    virtual bool drawFastest() const = 0;

    virtual void save() = 0;
    virtual void restore() = 0;

    virtual void startRestoreIdentityMatrix(GLdouble *matrix = NULL) = 0;
    virtual void stopRestoreIdentityMatrix() = 0;

    virtual void enableMultMatrix(bool e) = 0;

    virtual void pushMatrix() = 0;
    virtual void multMatrix(const QMatrix4x4 &matrix) = 0;
    virtual void popMatrix() = 0;

    virtual void setPointSize(double size) = 0;
    virtual void restoreDefaultPointSize() = 0;

    virtual void setPen(const QPen &pen) = 0;
    virtual void restoreDefaultPen() = 0;

    virtual void setColor(int r, int g, int b) = 0;
    virtual void setColor(QColor color) = 0;
    virtual void setForcedColor(int r, int g, int b) = 0;
    virtual void setForcedColor(QColor color) = 0;
    virtual QColor getColor() = 0;

    virtual void enableSetColor(bool enable) = 0;
    virtual void enableSetForcedColor(bool enable) = 0;

    virtual void enableSetPointSize(bool enable) = 0;
    virtual void enableSetForcedPointSize(bool enable) = 0;

    virtual void translate(double x, double y, double z) = 0;
    virtual void rotate(double alpha, double x, double y, double z) = 0;
    virtual void scale(double x, double y, double z) = 0;

    virtual void drawPoint(double x, double y, double z) = 0;
    virtual void drawPoint(double *p) = 0;
    virtual void drawPoint(float *p) = 0;

    virtual void beginMultiplePoints() = 0;
    virtual void addPoint(float *p) = 0;
    virtual void endMultiplePoints() = 0;

    virtual void drawOctreeOfPoints(const OctreeInterface *octree, DrawOctreeModes modes) = 0;

    virtual void drawPointCloud(const CT_AbstractPointCloud *pc,
                                const CT_AbstractCloudIndex *pci,
                                int fastestIncrement) = 0;

    virtual void drawMesh(const CT_AbstractMeshModel *mesh) = 0;
    virtual void drawFaces(const CT_AbstractMeshModel *mesh) = 0;
    virtual void drawEdges(const CT_AbstractMeshModel *mesh) = 0;
    virtual void drawPoints(const CT_AbstractMeshModel *mesh, int fastestIncrement) = 0;

    virtual void beginDrawMultipleLine() = 0;
    virtual void drawLine(double x1, double y1, double z1, double x2, double y2, double z2) = 0;
    virtual void drawLine(const float *p1, const float *p2) = 0;
    virtual void endDrawMultipleLine() = 0;

    virtual void drawCircle(double x, double y, double z, double radius) = 0;
    virtual void drawCircle3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, double radius) = 0;

    virtual void drawCylinder(double x, double y, double z, double radius, double height) = 0;
    virtual void drawCylinder3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, double radius, double height) = 0;

    virtual void drawEllipse(double x, double y, double z, double radiusA, double radiusB) = 0;

    virtual void beginDrawMultipleTriangle() = 0;
    virtual void drawTriangle(double x1, double y1, double z1,
                              double x2, double y2, double z2,
                              double x3, double y3, double z3) = 0;
    virtual void drawTriangle(const float *p1,
                              const float *p2,
                              const float *p3) = 0;
    virtual void endDrawMultipleTriangle() = 0;

    /**
     * @brief Draw a 3D cube defined by its bounding box (bottom left and top right corner).
     *
     */
    virtual void drawCube(double x1, double y1, double z1, double x2, double y2, double z2) = 0;

    /**
     * @brief Draw a 3D cube defined by its bounding box (bottom left and top right corner).
     * The drawing mode is given by the user (GL_POINT, GL_LINE or GL_FILL). Show glPolygonMode(...)
     */
    virtual void drawCube(double x1, double y1, double z1, double x2, double y2, double z2, GLenum faces, GLenum mode) = 0;

    /**
     * @brief Draw a rectangle in XY plane at level Z
     */
    virtual void drawRectXY(const QRectF &rectangle, double z) = 0;

    /**
     * @brief Draw a filled rectangle in XY plane at level Z
     */
    virtual void fillRectXY(const QRectF &rectangle, double z) = 0;


    /**
     * @brief Draw a rectangle in XZ plane at level Y
     */
    virtual void drawRectXZ(const QRectF &rectangle, double y) =0;

    /**
     * @brief Draw a filled rectangle in XZ plane at level Y
     */
    virtual void fillRectXZ(const QRectF &rectangle, double y) = 0;


    /**
     * @brief Draw a rectangle in YZ plane at level X
     */
    virtual void drawRectYZ(const QRectF &rectangle, double x) = 0;

    /**
     * @brief Draw a filled rectangle in YZ plane at level X
     */
    virtual void fillRectYZ(const QRectF &rectangle, double x) = 0;

    /**
     * @brief Draw a pyramid given its top point and its base
     */
    virtual void drawPyramid(double topX, double topY, double topZ,
                             double base1X, double base1Y, double base1Z,
                             double base2X, double base2Y, double base2Z,
                             double base3X, double base3Y, double base3Z,
                             double base4X, double base4Y, double base4Z) = 0;

    /**
          * @brief Draw a quad face in 3D
          */
    virtual void drawQuadFace( float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float x4, float y4, float z4 ) = 0;

    /**
          * @brief Draw a filled quad face in 3D
          */
    virtual void fillQuadFace( float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float x4, float y4, float z4 ) = 0;

    /**
          * @brief Draw a quad face in 3D with associated colors
          */
    virtual void drawQuadFace( float x1, float y1, float z1, int r1, int g1, int b1,
                               float x2, float y2, float z2, int r2, int g2, int b2,
                               float x3, float y3, float z3, int r3, int g3, int b3,
                               float x4, float y4, float z4, int r4, int g4, int b4 ) = 0;

    /**
          * @brief Draw a filled quad face in 3D with associated colors
          */
    virtual void fillQuadFace( float x1, float y1, float z1, int r1, int g1, int b1,
                               float x2, float y2, float z2, int r2, int g2, int b2,
                               float x3, float y3, float z3, int r3, int g3, int b3,
                               float x4, float y4, float z4, int r4, int g4, int b4 ) = 0;

    /**
     * @brief Draw a part of a sphere given the angles bounds
     */
    virtual void drawPartOfSphere ( double centerX, double centerY, double centerZ, double radius, double initTheta, double endTheta, double initPhi, double endPhi, bool radians = true ) = 0;

    virtual void beginPolygon() = 0;
    virtual void addPointToPolygon(float *p) = 0;
    virtual void endPolygon() = 0;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PainterInterface::DrawOctreeModes)

/*!
 *  \brief Représente les options de la vue
 */
class GraphicsViewOptionsInterface
{
public:
    virtual ~GraphicsViewOptionsInterface() {}

    virtual QColor getSelectedColor() const = 0;
    virtual QColor getBackgroundColor() const = 0;
    virtual float getPointSize() const = 0;
    virtual bool drawAxis() const = 0;
    virtual bool useColor() const = 0;
    virtual bool useTransparency() const = 0;
    virtual int getFastDrawTime() const = 0;
};

class CameraInterface : public QObject
{
    Q_OBJECT

public:
    enum CameraType {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    virtual ~CameraInterface() {}

    virtual double x() const = 0;
    virtual double y() const = 0;
    virtual double z() const = 0;
    virtual double cx() const = 0;
    virtual double cy() const = 0;
    virtual double cz() const = 0;
    virtual double rx() const = 0;
    virtual double ry() const = 0;
    virtual double rz() const = 0;

    virtual CameraInterface::CameraType type() const = 0;

    /**
     * @brief Returns the physical distance between the user's eyes and the screen (in meters).
     */
    virtual float focusDistance() const = 0;

    virtual const GLdouble* orientationMatrix() const = 0;
    virtual const GLdouble* orientationMatrixInversed() const = 0;

    virtual void modelViewMatrix(GLdouble m[16]) const = 0;

    // returns quaternion
    virtual void getOrientation(double &q0, double &q1, double &q2, double &q3) const = 0;
    virtual bool getCameraFrustumPlanesCoefficients(GLdouble coef[6][4]) const = 0;

    // Returns the screen projected coordinates of a point src defined in the world coordinate system.
    virtual QVector3D projectedCoordinatesOf(const QVector3D &src) const = 0;
    // same method as previous but use matrix of opengl instead of the matrix of camera.
    virtual QVector3D openGLProjectedCoordinatesOf(const QVector3D &src) const = 0;

    // Returns the world unprojected coordinates of a point src defined in the screen coordinate system.
    virtual QVector3D unprojectedCoordinatesOf(const QVector3D &src) const = 0;

    /**
     * @brief Returns the normalized up vector of the Camera, defined in the world coordinate system.
     *       It corresponds to the Y axis.
     */
    virtual QVector3D upVector() const = 0;

    /**
     * @brief Returns the normalized right vector of the Camera, defined in the world coordinate system.
     *       It corresponds to the X axis.
     */
    virtual QVector3D rightVector() const = 0;

public slots:
    virtual void setX(double x) = 0;
    virtual void setY(double y) = 0;
    virtual void setZ(double z) = 0;

    virtual void setCX(double cx) = 0;
    virtual void setCY(double cy) = 0;
    virtual void setCZ(double cz) = 0;

    virtual void setRX(double xRot) = 0;
    virtual void setRY(double yRot) = 0;
    virtual void setRZ(double zRot) = 0;

    virtual void setType(CameraInterface::CameraType type) = 0;
    virtual void setPosition(double x, double y, double z, bool redrawView = true) = 0;
    virtual void setSceneCenter(double cx, double cy, double cz, bool redrawView = true) = 0;
    virtual void setViewDirection(double rx, double ry, double rz, bool redrawView = true) = 0;
    virtual void setOrientation(double q0, double q1, double q2, double q3, bool redrawView = true) = 0;

    virtual void showEntireScene() = 0;
    virtual void alignCameraToXAxis() = 0;
    virtual void alignCameraToYAxis() = 0;
    virtual void alignCameraToZAxis() = 0;
    virtual void initCameraCenter() = 0;
    virtual void fixCameraCenterToItemsBarycenter() = 0;
    virtual void fixCameraCenterToSelectedItemsBarycenter() = 0;
    virtual void fitCameraToVisibleItems() = 0;


signals:
    void coordinatesChanged();
};

/**
 * @brief Represent a view that a document can contains
 */
class InDocumentViewInterface
{
public:
    virtual ~InDocumentViewInterface() {}

    /**
     * @brief Returns the document that contains this view
     */
    virtual DocumentInterface* document() const = 0;
};

/**
 * @brief Represent a manager of signals for the GraphicsViewInterface
 */
class GraphicsViewSignalEmitterInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~GraphicsViewSignalEmitterInterface() {}
    virtual GraphicsViewInterface* graphicsView() const = 0;

signals:
    void drawingStarted();
    void drawingFinished();
};

/*!
 *  \brief Représente la vue
 */
class GraphicsViewInterface : public InDocumentViewInterface
{
public:
    enum SelectionMode{
        // don't change the order !!! for a serie the first must always be SELECT_... and the last REMOVE_ONE_...

        NONE = 0,       // do nothing
        SELECT,         // make a new selection, all itemdrawable selected before was removed from list
        ADD,            // add new itemdrawable selected to list
        REMOVE,         // remove new itemdrawable selected from list
        SELECT_ONE,     // add first itemdrawable selected to the list and remove other
        ADD_ONE,        // add first itemdrawable selected to the list
        REMOVE_ONE,     // remove first itemdrawable selected from the list
        // same but for points
        SELECT_POINTS,
        ADD_POINTS,
        REMOVE_POINTS,
        SELECT_ONE_POINT,
        ADD_ONE_POINT,
        REMOVE_ONE_POINT,
        // same but for faces
        SELECT_FACES,
        ADD_FACES,
        REMOVE_FACES,
        SELECT_ONE_FACE,
        ADD_ONE_FACE,
        REMOVE_ONE_FACE,
        // same but for edges
        SELECT_EDGES,
        ADD_EDGES,
        REMOVE_EDGES,
        SELECT_ONE_EDGE,
        ADD_ONE_EDGE,
        REMOVE_ONE_EDGE
    };

    enum DrawMode{
        NORMAL,         // draw all itemdrawable normally
        FAST            // draw all itemdrawable fast (decrease per example the number of points or the definition of circles)
    };

    enum ColorCloudType {
        CPointCloud,
        CFaceCloud,
        CEdgeCloud
    };

    enum NormalCloudType {
        NPointCloud,
        NFaceCloud,
        NEdgeCloud
    };

    enum GraphicsViewType {
        GV2D,
        GV3D
    };

    virtual ~GraphicsViewInterface() {}

    /**
     * @brief Return the signal emitter of the graphics view
     */
    virtual GraphicsViewSignalEmitterInterface* signalEmitter() const = 0;

    /**
     * @brief Return the options of the graphics view
     */
    virtual GraphicsViewOptionsInterface& getOptions() = 0;

    /**
     * @brief Return the color cloud for the type passed in parameter. Can be NULL if it was not created.
     */
    virtual QSharedPointer<CT_StandardColorCloudRegistered> colorCloudOf(ColorCloudType type) const = 0;

    /**
     * @brief Return the normal cloud for the type passed in parameter. Can be NULL if it was not created.
     */
    virtual QSharedPointer<CT_StandardNormalCloudRegistered> normalCloudOf(NormalCloudType type) const = 0;

    /**
     * @brief Return the camera of the view
     */
    virtual CameraInterface* camera() const = 0;

    /**
     * @brief Return the type of the view
     */
    virtual GraphicsViewType type() const = 0;

    /**
     * @brief Set the draw mode. If you set the mode to FAST it will reset to NORMAL after "drawModeChangeTime()" ms.
     */
    virtual void setDrawMode(DrawMode dMode) = 0;

    /**
     * @brief Get the draw mode
     */
    virtual GraphicsViewInterface::DrawMode drawMode() const = 0;

    /**
     * @brief Set the time (in ms) the mode takes to change from FAST to NORMAL. If you set 0 the mode actually
     *        set don't change.
     */
    virtual void setDrawModeChangeTime(const int &msec) = 0;

    /**
     * @brief Get how many time the mode takes to change from FAST to NORMAL.
     */
    virtual int drawModeChangeTime() const = 0;

    /**
     * @brief Set the select buffer size (you must multiply the number of elements to select by 4 to know the buffer size to set)
     */
    virtual void setSelectBufferSize(size_t size) = 0;

    /**
     * @brief Return the select buffer size
     */
    virtual size_t selectBufferSize() const = 0;

    /**
     * @brief Set the selection width. Default is 3;
     */
    virtual void setSelectRegionWidth(int width) = 0;

    /**
     * @brief Set the selection height. Default is 3;
     */
    virtual void setSelectRegionHeight(int height) = 0;

    /**
     * @brief Set the selection mode
     */
    virtual void setSelectionMode(SelectionMode mode) = 0;

    /**
     * @brief Get the selection mode
     */
    virtual GraphicsViewInterface::SelectionMode selectionMode() const = 0;

    /**
     * @brief Returns true if the view must select points
     */
    virtual bool mustSelectPoints() const = 0;

    /**
     * @brief Returns true if the view must select edges
     */
    virtual bool mustSelectEdges() const = 0;

    /**
     * @brief Returns true if the view must select faces
     */
    virtual bool mustSelectFaces() const = 0;

    /**
     * @brief Returns true if the view must select items
     */
    virtual bool mustSelectItems() const = 0;

    /**
     * @brief Performs a selection in the scene from pixel coordinates.
     */
    virtual void select(const QPoint& point) = 0;

    /**
     * @brief Returns all points (indices) selected. Can be NULL if no points where selected.
     */
    virtual QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> getSelectedPoints() const = 0;

    /**
     * @brief Returns all faces (indices) selected. Can be NULL if no faces where selected.
     */
    virtual QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> getSelectedFaces() const = 0;

    /**
     * @brief Returns all edges (indices) selected. Can be NULL if no edges where selected.
     */
    virtual QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> getSelectedEdges() const = 0;

    /**
     * @brief Returns all ItemDrawable selected
     */
    virtual QList<CT_AbstractItemDrawable*> getSelectedItems() const = 0;

    /**
     * @brief Call this method if you want to remove multiple points from selection
     * @param n : number of points to remove
     * @warning dont forget to call the "endRemoveMultiplePointsFromSelection" method when you have finished
     */
    virtual void beginRemoveMultiplePointsFromSelection(const size_t &n) = 0;

    /**
     * @brief remove point from selection. If you have multiple index to remove call "beginRemoveMultiplePointsFromSelection"
     * @param globalIndex : the index of the point in the global points cloud
     */
    virtual void removePointFromSelection(const size_t &globalIndex) = 0;

    /**
     * @brief Call this method when you have finished to remove multiple points from selection
     */
    virtual void endRemoveMultiplePointsFromSelection() = 0;

    /**
     * @brief Call this method with true if you want to select all points, false if you want to unselect all points
     */
    virtual void setAllPointsSelected(bool select) = 0;

    /**
     * @brief Call this method if you want to remove multiple faces from selection
     * @param n : number of faces to remove
     * @warning dont forget to call the "endRemoveMultipleFacesFromSelection" method when you have finished
     */
    virtual void beginRemoveMultipleFacesFromSelection(const size_t &n) = 0;

    /**
     * @brief remove face from selection. If you have multiple index to remove call "beginRemoveMultipleFacesFromSelection"
     * @param globalIndex : the index of the face in the global points cloud
     */
    virtual void removeFaceFromSelection(const size_t &globalIndex) = 0;

    /**
     * @brief Call this method when you have finished to remove multiple faces from selection
     */
    virtual void endRemoveMultipleFacesFromSelection() = 0;

    /**
     * @brief Call this method with true if you want to select all faces, false if you want to unselect all faces
     */
    virtual void setAllFacesSelected(bool select) = 0;

    /**
     * @brief Call this method if you want to remove multiple edges from selection
     * @param n : number of edges to remove
     * @warning dont forget to call the "endRemoveMultipleEdgesFromSelection" method when you have finished
     */
    virtual void beginRemoveMultipleEdgesFromSelection(const size_t &n) = 0;

    /**
     * @brief remove edge from selection. If you have multiple index to remove call "beginRemoveMultipleEdgesFromSelection"
     * @param globalIndex : the index of the face in the global points cloud
     */
    virtual void removeEdgeFromSelection(const size_t &globalIndex) = 0;

    /**
     * @brief Call this method when you have finished to remove multiple edges from selection
     */
    virtual void endRemoveMultipleEdgesFromSelection() = 0;

    /**
     * @brief Call this method with true if you want to select all edges, false if you want to unselect all edges
     */
    virtual void setAllEdgesSelected(bool select) = 0;

    /**
     * @brief Returns how many points is drawn
     */
    virtual size_t countPoints() = 0;

    /**
     * @brief Returns how many edges is drawn
     */
    virtual size_t countEdges() = 0;

    /**
     * @brief Returns how many faces is drawn
     */
    virtual size_t countFaces() = 0;

    /**
     * @brief Returns how many items is drawn
     */
    virtual size_t countItems() = 0;

    /**
     * @brief Returns the 6 plane equations of the Camera frustum.
     *        The six 4-component vectors of coef respectively correspond
     *        to the left, right, near, far, top and bottom Camera frustum planes.
     *        Each vector holds a plane equation of the form:
     *
     *          a*x + b*y + c*z + d = 0
     *
     *        where a, b, c and d are the 4 components of each vector, in that order.
     */
    virtual bool getCameraFrustumPlanesCoefficients(GLdouble coef[6][4]) const = 0;

    /**
     * @brief Returns the distance of the point passed in parameter to the frustrum plane
     * @param index : index of the frustrum plane (between 0 and 6 excluded)
     * @param x/y/z : the point
     */
    virtual float distanceToFrustumPlane(int index, const double &x, const double &y, const double &z) const = 0;

    /**
     * @brief Returns true if the aaBox is visible by the camera
     * @param minCorner / maxCorner : corners of the box
     * @param entirely : set to true if the box is entirely visible
     */
    virtual bool aaBoxIsVisible(const QVector3D& minCorner, const QVector3D& maxCorner, bool *entirely = NULL) const = 0;

    /**
     * @brief Returns true if the sphere is visible by the camera
     * @param center : center of the sphere
     * @param radius : radius of the sphere
     */
    virtual bool sphereIsVisible(const QVector3D& center, float radius) const = 0;

    /**
     * @brief Returns the coordinates of the 3D point located at pixel (x,y) on screen.
     *
     * Calls a glReadPixel to get the pixel depth and applies an unprojectedCoordinatesOf() to
     * the result. found indicates whether a point was found or not (i.e. background pixel, result's
     * depth is zFar() in that case).
     *
     * x and y are expressed in pixel units with an origin in the upper left corner.
     */
    virtual QVector3D pointUnderPixel(const QPoint &pixel, bool &found) const = 0;

    /**
     * @brief Gives the coefficients of a 3D half-line passing through the Camera eye and pixel (x,y).
     *
     * The origin of the half line (eye position) is stored in orig, while dir contains the properly oriented and normalized direction of the half line.
     *
     * x and y are expressed in Qt format (origin in the upper left corner).
     *
     * This method is useful for analytical intersection in a selection method.
     */
    virtual void convertClickToLine(const QPoint &pixel, Eigen::Vector3d &orig, Eigen::Vector3d &dir) const = 0;

    virtual void convert3DPositionToPixel(const QVector3D &position, QPoint &pixel) const = 0;

    /**
     * @brief Add option of a action to the GraphicsView
     *
     *        If you want to remove the widget from the view you can delete it or
     *        call method "removeActionOptions"
     *
     * @warning You must be in the GUI thread to call this method !
     */
    virtual void addActionOptions(ActionOptionsInterface *options) = 0;

    /**
     * @brief Delete the widget. If you want to hide them call
     *        method "QWidget::hide()".
     */
    virtual void removeActionOptions(ActionOptionsInterface *options) = 0;

    /**
     * @brief Return the width of the graphics
     */
    virtual int width() const = 0;

    /**
     * @brief Return the height of the graphics
     */
    virtual int height() const = 0;
};

/**
 * @brief Represent a view that show a tree model
 */
class TreeViewInterface : public InDocumentViewInterface
{
public:
    virtual ~TreeViewInterface() {}

    /**
     * @brief Convert a QModelIndex to a CT_AbstractItemDrawable. Can return NULL if the QModelIndex is not valid.
     */
    virtual CT_AbstractItemDrawable* itemDrawableFromIndex(const QModelIndex &index) const = 0;

    /**
     * @brief Returns the model index of the item at the viewport coordinates point.
     */
    virtual QModelIndex indexAt(const QPoint &point) const = 0;

    /**
     * @brief Returns the viewport widget.
     */
    virtual QWidget* treeViewport() const = 0;

    /**
     * @brief This method controls whether the user can select one or many items
     *        and, in many-item selections, whether the selection must
     *        be a continuous range of items.
     */
    virtual void setSelectionMode(QAbstractItemView::SelectionMode mode) = 0;

    /**
     * @brief Returns the current selection mode
     */
    virtual QAbstractItemView::SelectionMode selectionMode() const = 0;

    /**
     * @brief Returns selected indexes. The list contains no duplicates, and is not sorted.
     */
    virtual QModelIndexList selectedIndexes () const = 0;

    /**
     * @brief Returns the current selection model.
     */
    virtual QItemSelectionModel* selectionModel() const = 0;

    /**
     * @brief Refresh the tree view
     */
    virtual void refreshAll() = 0;

    /**
     * @brief Refresh only children of the list of QModelIndex passed in parameter
     */
    virtual void refreshItems(const QList<QModelIndex> &indexes) = 0;

    /**
     * @brief Search the QModelIndex corresponding to the ItemDrawable passed in parameter
     *        and refresh children for all ItemDrawable in the list
     * @warning slow ! prefer the method "refreshItems" with QModelIndex or "refreshAll"
     */
    virtual void refreshItems(const QList<CT_AbstractItemDrawable*> &items) = 0;
};

/*!
 *  \brief Représente un document.
 *
 *  Un document contient des ItemDrawable. Celui-ci peut être représenté par plusieurs vues (GraphicsViewInterface).
 */
class DocumentInterface : public QObject
{
    Q_OBJECT

public:

    virtual ~DocumentInterface() {}

    /**
      * \brief Title of the document
      */
    virtual QString getTitle() const = 0;

    /**
     * @brief Lock the document to disable the access of ItemDrawable (exemple : lock the draw of ItemDrawable, lock the loading of ItemDrawable, etc...)
     */
    virtual void lock() = 0;

    /**
     * @brief Unlock the document to enable the access of ItemDrawable
     */
    virtual void unlock() = 0;

    /**
     * @brief Call this method if you plan to add multiple ItemDrawable in one times
     */
    virtual void beginAddMultipleItemDrawable() = 0;

    /**
      * \brief Ajout d'un ItemDrawable au document
      */
    virtual void addItemDrawable(CT_AbstractItemDrawable &item) = 0;

    /**
     * @brief Call this method after you add multiple ItemDrawable and if you have called the method 'beginAddMultipleItemDrawable'
     */
    virtual void endAddMultipleItemDrawable() = 0;

    /**
     * @brief Call this method if you plan to remove multiple ItemDrawable in one times
     */
    virtual void beginRemoveMultipleItemDrawable() = 0;

    /**
      * \brief Supprime l'ItemDrawable du document
      */
    virtual void removeItemDrawable(CT_AbstractItemDrawable &item) = 0;

    /**
     * @brief Call this method after you removed multiple ItemDrawable and if you have called the method 'beginRemoveMultipleItemDrawable'
     */
    virtual void endRemoveMultipleItemDrawable() = 0;

    /**
      * \brief Removes all ItemDrawable in the document that was in the result 'res'. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawableOfResult(const CT_AbstractResult &res) = 0;

    /**
      * \brief Removes all ItemDrawable in the document that have the model 'model'. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawableOfModel(const CT_OutAbstractModel &model) = 0;

    /**
      * \brief Removes all selected ItemDrawable in the document. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllSelectedItemDrawable() = 0;

    /**
      * \brief Removes all ItemDrawable in the document. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawable() = 0;

    /**
      * \brief Select/DeSelect of ItemDrawable of the document
      */
    virtual void setSelectAllItemDrawable(bool select) = 0;

    /**
      * \brief Select/DeSelect of ItemDrawable of the document that have the model 'model'
      */
    virtual void setSelectAllItemDrawableOfModel(bool select, const CT_OutAbstractModel &model) = 0;

    /**
      * \brief Returns all ItemDrawable contained in this document
      */
    virtual const QList<CT_AbstractItemDrawable*>& getItemDrawable() const = 0;

    /**
      * \brief Returns all ItemDrawable selected
      */
    virtual QList<CT_AbstractItemDrawable*> getSelectedItemDrawable() const = 0;

    /**
     * @brief Returns true if this document use item color.
     */
    virtual bool useItemColor() const = 0;

    /**
     * @brief Set the color of the item passed in parameter. Do nothing if useItemColor() return false.
     */
    virtual void setColor(const CT_AbstractItemDrawable *item, const QColor &color) = 0;

    /**
     * @brief Return the color of the item passed in parameter. Returns an invalid color if useItemColor() return false.
     */
    virtual QColor getColor(const CT_AbstractItemDrawable *item) = 0;

    /**
     * @brief Returns true if this document use octree for points
     */
    virtual bool useOctreeOfPoints() const = 0;

    /**
     * @brief Returns the octree of points or NULL if usePointsOctree() return false.
     */
    virtual OctreeInterface* octreeOfPoints() const = 0;

public slots:
    /**
     * @brief (Re)construct the octree
     */
    virtual void constructOctreeOfPoints() = 0;

public:
    /**
      * \brief Returns the number of ItemDrawable
      */
    virtual size_t nItemDrawable() const = 0;

    /**
      * \brief Return the ItemDrawable 'i' in the list
      */
    virtual CT_AbstractItemDrawable* getItemDrawable(int i) const = 0;

    /**
      * \brief Recherche des ItemDrawable dans la liste  partir du model pass en paramtre
      * \return une liste vide si aucun ItemDrawable n'est de ce type
      */
    virtual QList<CT_AbstractItemDrawable*> findItemDrawable(const CT_OutAbstractModel &model) const = 0;
    virtual void findItemDrawable(const CT_OutAbstractModel &model, QList<CT_AbstractItemDrawable*> &outList) const = 0;

    /**
      * \brief Recherche le premier ItemDrawable dans la liste a partir du modele
      * \return NULL si aucun ItemDrawable n'est de ce type
      */
    virtual CT_AbstractItemDrawable* findFirstItemDrawable(const CT_OutAbstractModel &model) const = 0;

    /**
     * @brief Return true if the document accept this type of action
     */
    virtual bool acceptAction(const CT_AbstractAction *action) const = 0;

    /**
     * @brief Set the current action to the document. If the document contains graphics, each of
     *        them receives a copy of the action. If the document contains treeview, each of them
     *        receives a copy of the action. etc...
     *
     *        If the action don't exist in the actions manager it will be added automatically.
     *
     * @param action : action to set current
     * @param deleteAction : true if the action must be deleted if she exist in the actions manager (true by default)
     *
     * @return false if the document don't accept this type of action.
     */
    virtual bool setCurrentAction(CT_AbstractAction *action, bool deleteAction = true) = 0;

    /**
     * @brief Return the list of views in the document
     */
    virtual QList<InDocumentViewInterface*> views() const = 0;

    /**
      * \brief Refresh all graphics in the document (if he contains graphicsView)
      */
    virtual void redrawGraphics() = 0;
};

/**
 * @brief Représente le gestionnaire des documents
 */
class DocumentManagerInterface
{
public:
    virtual ~DocumentManagerInterface() {}

    /**
     * @brief Lock all documents to disable the access of ItemDrawable (exemple : lock the draw of ItemDrawable, lock the loading of ItemDrawable, etc...)
     *
     *        Don't forget to call the method "unlockAllDocuments()"
     */
    virtual void lockAllDocuments() = 0;

    /**
     * @brief Unlock all documents to enable the access to ItemDrawable
     *
     *        Call only this method after "lockAllDocuments()"
     */
    virtual void unlockAllDocuments() = 0;

    /**
     * @brief Create a new document with 3D View
     */
    virtual DocumentInterface* new3DDocument() = 0;

    /**
     * @brief Create a new document with 3D View
     */
    virtual DocumentInterface* new3DDocument(const QMap<QString, QVariant> &param) = 0;


    /**
     * @brief Create a new document with 2D View
     */
    virtual DocumentInterface* new2DDocument() = 0;

    /**
     * @brief Create a new document with TreeView
     */
    virtual DocumentInterface* newTreeViewDocument() = 0;

    /**
     * @brief Return the number of documents
     */
    virtual int nDocuments() const = 0;

    /**
     * @brief Return the document at index passed in parameter
     */
    virtual DocumentInterface* documentAt(const int &index) const = 0;

    /**
     * @brief Return the documents collection
     */
    virtual QList<DocumentInterface*> documents() const = 0;

    /**
     * @brief Return the active document
     */
    virtual DocumentInterface* activeDocument() = 0;

    /**
     * @brief Activate the document passed in parameter
     */
    virtual void activateDocument(DocumentInterface *doc) = 0;

    /**
     * @brief Close the document passed in parameter.
     *
     * @return false if it is the last document. One document must always be present.
     */
    virtual bool closeDocument(DocumentInterface *doc) = 0;

    /**
     * @brief Close the document at index passed in parameter.
     *
     * @return false if it is the last document. One document must always be present.
     */
    virtual bool closeDocument(const int &index) = 0;
};

/**
 * @brief Représente le gestionnaire de l'application (la vue)
 */
class GuiManagerInterface
{
public:
    virtual ~GuiManagerInterface() {}

    /**
     * @return le gestionnaire des documents. NULL si il n'y en a pas.
     */
    virtual DocumentManagerInterface* documentManager() const = 0;
};

/**
 * @brief Represent the core of the application
 */
class CoreInterface
{
public:
    virtual ~CoreInterface() {}

    /**
     * @brief get the log
     */
    virtual LogInterface* log() const = 0;

    /**
     * @brief get the plugin manager
     */
    virtual PluginManagerInterface* pluginManager() const = 0;
};

/**
 * @brief The log interface
 */
class LogInterface
{
public:

    enum DefaultSeverity
    {
        trace = 0,
        debug,
        info,
        warning,
        error,
        fatal
    };

    enum DefaultType
    {
        core = 0,
        gui,
        plugin,
        step,
        result,
        itemdrawable,
        action,
        reader,
        exporter,
        unknow
    };

    virtual ~LogInterface() {}

    /**
     * @brief add a message
     *
     * @param severity : the severity (trace, debug, info, etc...)
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addMessage(const int &severity, const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a trace message (hte severity is automatically set to "trace")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addTraceMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a debug message (hte severity is automatically set to "debug")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addDebugMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a info message (hte severity is automatically set to "info")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addInfoMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a warning message (hte severity is automatically set to "warning")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addWarningMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a error message (hte severity is automatically set to "error")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addErrorMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a fatal message (hte severity is automatically set to "fatal")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addFatalMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a message
     *
     * @param severity : the severity (trace, debug, info, etc...)
     * @param type : when you call this method pass "this" for the type and the type is automatically detected
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addMessage(const int &severity, CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addMessage(const int &severity, CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addMessage(const int &severity, PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addMessage(const int &severity, CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addMessage(const int &severity, CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addMessage(const int &severity, CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addTraceMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addTraceMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addTraceMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addTraceMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addTraceMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addTraceMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addDebugMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addDebugMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addDebugMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addDebugMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addDebugMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addDebugMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addInfoMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addInfoMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addInfoMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addInfoMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addInfoMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addInfoMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addWarningMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addWarningMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addWarningMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addWarningMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addWarningMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addWarningMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addErrorMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addErrorMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addErrorMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addErrorMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addErrorMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addErrorMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addFatalMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addFatalMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addFatalMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addFatalMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addFatalMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addFatalMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief Add log listener that must receive message in prioritary and immediately (Elements like File, std::cout, etc...)
     * @warning Add a log listener to this method can reduce the performance of your application if the message is not compute fast
     */
    virtual void addPrioritaryLogListener(const CT_AbstractLogListener *logl) = 0;

    /**
     * @brief Add a log listener that must receive message in normal mode (delayed) (Elements of ui like QTextEdit, etc...)
     */
    virtual void addNormalLogListener(const CT_AbstractLogListener *logl) = 0;

    /**
     * @brief Remove a log listener (prioritary or normal)
     */
    virtual void removeLogListener(CT_AbstractLogListener *logl) = 0;
};

/**
 * @brief Represent a gui context
 */
class GuiContextInterface
{
public:
    virtual ~GuiContextInterface() {}

    /**
     * @brief Return the document manager
     */
    virtual DocumentManagerInterface* documentManager() const = 0;

    /**
     * @brief Return the action manager
     */
    virtual ActionsManagerInterface* actionsManager() const = 0;
};

class ActionsManagerInterface
{
public:
    virtual ~ActionsManagerInterface() {}

    /**
     * @brief Add a new action. The manager is responsible of the life of the action. You must not delete
     *        your action ! The action is deleted if you remove it or if the manager is destroyed or if the
     *        manual or debug mode is finished.
     *
     *        Before set the current action to a document you must add it in the actions manager by this method.
     *
     * @return return false if the action can't be added because already exist in a plugins.
     */
    virtual bool addAction(CT_AbstractAction *action) = 0;

    /**
     * @brief Remove the action and the associated button from the view. Action is deleted after it has been removed.
     *
     * @warning if you remove a action that is activate in a document it will be finished and deleted.
     *
     * @return return false if it can't be removed because it's a action from a plugin or if she don't exist in the list.
     */
    virtual bool removeAction(CT_AbstractAction *action) = 0;
    virtual bool removeAction(const QString &uniqueName) = 0;

    /**
     * @brief Return true if the action exist in the actions added by a step or available in a plugin
     */
    virtual bool existAction(const CT_AbstractAction *action) const = 0;
    virtual bool existAction(const QString &uniqueName) const = 0;

    /**
     * @brief Return the action that have this unique name. NULL if not exist.
     */
    virtual CT_AbstractAction* action(const QString &uniqueName) const = 0;
};

/**
 * @brief Represent a options widget of a action
 */
class ActionOptionsInterface : public QWidget
{
public:
    virtual ~ActionOptionsInterface() {}

    /**
     * @brief Return the action that created this widget
     */
    virtual CT_AbstractAction* action() const = 0;
};

/**
 * @brief Represent a format of file for read or export
 */
class FileFormat
{
public:
    FileFormat(const QList<QString> &suffixes, QString description) { _suffixes = suffixes; _description = description; }
    FileFormat(const QString &suffix, QString description) { _suffixes << suffix; _description = description; }

    /**
     * @brief suffixes of file
     */
    const QList<QString>& suffixes() const { return _suffixes; }

    /**
     * @brief description
     */
    QString description() const { return _description; }

private:
    QList<QString> _suffixes;
    QString _description;
};

/**
 * @brief Represent the manager of plugins
 */
class PluginManagerInterface
{
public:
    virtual ~PluginManagerInterface() {}

    /*!
     *  \brief return true if at least one plugin is loaded
     */
    virtual bool isAPluginLoaded() const = 0;

    /*!
     *  \brief return the number of plugins loaded
     */
    virtual int countPluginLoaded() const = 0;

    /*!
     *  \brief return the name of the plugin 'i'
     */
    virtual QString getPluginName(int i = 0) const = 0;

    /*!
     *  \brief return the search path for plugins
     */
    virtual QString getSearchPath() const = 0;

    /*!
     *  \brief return the plugin 'i'
     */
    virtual CT_AbstractStepPlugin* getPlugin(int i) const = 0;

    /*!
     *  \brief return the plugin with name 'pluginName'
     */
    virtual CT_AbstractStepPlugin* getPlugin(QString pluginName) const = 0;
};

/*!
 *  \brief Représente le point d'entrée du Plugin.
 */
class PluginEntryInterface : public QObject
{
    Q_OBJECT

public:

    virtual ~PluginEntryInterface() {}

    /*!
     *  \brief Retourne le numro de version du plugin.
     */
    virtual QString getVersion() const = 0;

    /*!
     *  \brief Retourne le gestionnaire de plugin d'étapes
     */
    virtual CT_AbstractStepPlugin* getPlugin() const = 0;
};

#define CoreInterface_iid "com.krebs.michael.ONF.PluginSharedV2.CoreInterface"
#define LogInterface_iid "com.krebs.michael.ONF.PluginSharedV2.LogInterface"
#define PluginManagerInterface_iid "com.krebs.michael.ONF.PluginSharedV2.PluginManagerInterface"

#define GuiManagerInterface_iid "com.krebs.michael.ONF.PluginSharedV2.GuiManagerInterface"
#define GuiContextInterface_iid "com.krebs.michael.ONF.PluginSharedV2.GuiContextInterface"
#define DocumentManagerInterface_iid "com.krebs.michael.ONF.PluginSharedV2.DocumentManagerInterface"
#define DocumentInterface_iid "com.krebs.michael.ONF.PluginSharedV2.DocumentInterface"
#define InDocumentViewInterface_iid "com.krebs.michael.ONF.PluginSharedV2.InDocumentViewInterface"
#define GraphicsViewInterface_iid "com.krebs.michael.ONF.PluginSharedV2.GraphicsViewInterface"
#define GraphicsViewOptionsInterface_iid "com.krebs.michael.ONF.PluginSharedV2.GraphicsViewOptionsInterface"
#define GraphicsViewSignalEmitterInterface_iid "com.krebs.michael.ONF.PluginSharedV2.GraphicsViewSignalEmitterInterface"
#define CameraInterface_iid "com.krebs.michael.ONF.PluginSharedV2.CameraInterface"
#define OctreeInterface_iid "com.krebs.michael.ONF.PluginSharedV2.OctreeInterface"
#define PainterInterface_iid "com.krebs.michael.ONF.PluginSharedV2.PainterInterface"
#define TreeViewInterface_iid "com.krebs.michael.ONF.PluginSharedV2.TreeViewInterface"
#define ActionsManagerInterface_iid "com.krebs.michael.ONF.PluginSharedV2.ActionsManagerInterface"
#define ActionOptionsInterface_iid "com.krebs.michael.ONF.PluginSharedV2.ActionOptionsInterface"

#define PluginEntryInterface_iid "com.krebs.michael.ONF.PluginSharedV2.PluginEntryInterface"

// CORE
Q_DECLARE_INTERFACE(CoreInterface, CoreInterface_iid)
Q_DECLARE_INTERFACE(LogInterface, LogInterface_iid)
Q_DECLARE_INTERFACE(PluginManagerInterface, PluginManagerInterface_iid)

// GUI
Q_DECLARE_INTERFACE(GuiManagerInterface, GuiManagerInterface_iid)
Q_DECLARE_INTERFACE(GuiContextInterface, GuiContextInterface_iid)
Q_DECLARE_INTERFACE(DocumentManagerInterface, DocumentManagerInterface_iid)
Q_DECLARE_INTERFACE(DocumentInterface, DocumentInterface_iid)
Q_DECLARE_INTERFACE(InDocumentViewInterface, InDocumentViewInterface_iid)
Q_DECLARE_INTERFACE(GraphicsViewInterface, GraphicsViewInterface_iid)
Q_DECLARE_INTERFACE(GraphicsViewOptionsInterface, GraphicsViewOptionsInterface_iid)
Q_DECLARE_INTERFACE(GraphicsViewSignalEmitterInterface, GraphicsViewSignalEmitterInterface_iid)
Q_DECLARE_INTERFACE(CameraInterface, CameraInterface_iid)
Q_DECLARE_INTERFACE(OctreeInterface, OctreeInterface_iid)
Q_DECLARE_INTERFACE(PainterInterface, PainterInterface_iid)
Q_DECLARE_INTERFACE(TreeViewInterface, TreeViewInterface_iid)
Q_DECLARE_INTERFACE(ActionsManagerInterface, ActionsManagerInterface_iid)
Q_DECLARE_INTERFACE(ActionOptionsInterface, ActionOptionsInterface_iid)

// PLUGINS
Q_DECLARE_INTERFACE(PluginEntryInterface, PluginEntryInterface_iid)

#endif // INTERFACES_H
