/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#ifndef G3DPAINTER_H
#define G3DPAINTER_H

#include "dm_graphicsviewoptions.h"

#include "ct_point.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

#include <QtOpenGL>

// GLU was removed from Qt in version 4.8
#ifdef Q_OS_MAC
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtOpenGL/QGLShaderProgram>
#include <QGLFunctions>
#define QT_GL_SHADERPROGRAM QGLShaderProgram
#define QT_GL_SHADER QGLShader
#define QT_GL_CONTEXT QGLContext
#define QT_GL_FUNCTIONS QGLFunctions
#define QT_GL_INIT_FUNCTIONS initializeGLFunctions
#else
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#define QT_GL_SHADERPROGRAM QOpenGLShaderProgram
#define QT_GL_SHADER QOpenGLShader
#define QT_GL_CONTEXT QOpenGLContext
#define QT_GL_FUNCTIONS QOpenGLFunctions
#define QT_GL_INIT_FUNCTIONS initializeOpenGLFunctions
#endif

class G3DGraphicsView;
template<typename T> class CT_AbstractCloudT;

class G3DPainter : public PainterInterface, public QT_GL_FUNCTIONS
{
    Q_INTERFACES(PainterInterface)

public:

    /**
     * @brief Element in this painter that can be draw with
     *        no call to glBegin multiple times.
     */
    enum MultiDrawableType {
        POINT_CLOUD = 0,    // is at hand because it uses a shader      (draw only point cloud)
        FACE,               // is at hand because it CAN uses a shader  (draw only face)
        EDGE,               // is at hand because it CAN uses a shader  (draw only edge)

        POINT,              // (draw only point)
        LINE,               // (draw only line)
        TRIANGLE,           // (draw only triangle)
        QUADS,              // (draw only quads)

        NO_MULTI_AVAILABLE, // (draw only othe elements)

        DRAW_ALL            // (draw all)
    };

    G3DPainter();
    virtual ~G3DPainter();

    /**
     * @brief Set the graphics view to access to point/face/edge cloud
     *        color, point/face/edge cloud informations, etc...
     */
    void setGraphicsView(const G3DGraphicsView *gv);
    G3DGraphicsView* graphicsView() const;

    /**
     * @brief Call this functions in your method initializeOpenGL
     */
    void initializeGl();

    /**
     * @brief Begin a new draw (call this functions in beginning of your draw method)
     *        Restore value to default
     */
    void beginNewDraw();

    /**
     * @brief End new draw (call this functions in ending of your draw method)
     *        Do nothing by default
     */
    void endNewDraw();

    /**
     * @brief Call this function to draw only type passed in parameter. This function
     *        optimize call to glBegin to improve the speed of draw for all itemdrawable to draw.
     *
     *        If you want to draw ALL don't call this method.
     *        If you want to draw only element that was not multi drawable call this method with NO_MULTI_AVAILABLE
     */
    void setDrawOnly(G3DPainter::MultiDrawableType type);

    /**
     * @brief Set the fastest increment to use in drawPointCloud method. If 0 use the fastest increment passed in parameter;
     */
    void setPointFastestIncrement(size_t inc);
    size_t pointFastestIncrement() const;

    /**
     * @brief Returns the number of cells of the octree drawed
     */
    int nOctreeCellsDrawed() const;

    /**
     * @brief Set if must draw fastest (with the fastest increment)
     */
    void setDrawFastest(bool enable);
    bool drawFastest() const;

    virtual void save();
    virtual void restore();

    virtual void startRestoreIdentityMatrix(GLdouble *matrix = NULL);
    virtual void stopRestoreIdentityMatrix();

    virtual void enableMultMatrix(bool e);

    virtual void pushMatrix();
    virtual void multMatrix(const QMatrix4x4 &matrix);
    virtual void popMatrix();

    virtual void setPointSize(double size);
    virtual void setDefaultPointSize(double size);
    virtual void restoreDefaultPointSize();

    virtual void setPen(const QPen &pen);
    virtual void restoreDefaultPen();

    virtual void setColor(int r, int g, int b);
    virtual void setColor(QColor color);
    virtual void setForcedColor(int r, int g, int b);
    virtual void setForcedColor(QColor color);
    virtual void setUseColorCloudForPoints(bool enable);
    virtual void setUseNormalCloudForPoints(bool enable);
    virtual void setUseColorCloudForFaces(bool enable);
    virtual void setUseNormalCloudForFaces(bool enable);
    virtual void setUseColorCloudForEdges(bool enable);
    virtual QColor getColor();

    virtual void enableSetColor(bool enable);
    virtual void enableSetForcedColor(bool enable);

    virtual void enableSetPointSize(bool enable);
    virtual void enableSetForcedPointSize(bool enable);

    virtual void translate(double x, double y, double z);
    virtual void rotate(double alpha, double x, double y, double z);
    virtual void translateThenRotateToDirection(const QVector3D &translation, const QVector3D &direction);
    virtual void scale(double x, double y, double z);

    virtual void drawOctreeOfPoints(const OctreeInterface *octree, DrawOctreeModes modes);

    virtual void drawPoint(double x, double y, double z);
    virtual void drawPoint(double *p);
    virtual void drawPoint(float *p);

    virtual void drawPointCloud(const CT_AbstractPointCloud *pc,
                        const CT_AbstractCloudIndex *pci,
                        int fastestIncrement);

    virtual void drawMesh(const CT_AbstractMeshModel *mesh);
    virtual void drawFaces(const CT_AbstractMeshModel *mesh);
    virtual void drawEdges(const CT_AbstractMeshModel *mesh);
    virtual void drawPoints(const CT_AbstractMeshModel *mesh, int fastestIncrement);

    virtual void beginMultiplePoints();
    virtual void addPoint(float *p);
    virtual void endMultiplePoints();

    virtual void beginDrawMultipleLine();
    virtual void drawLine(double x1, double y1, double z1, double x2, double y2, double z2);
    virtual void drawLine(const float *p1, const float *p2);
    virtual void endDrawMultipleLine();

    virtual void drawCircle(double x, double y, double z, double radius);
    virtual void drawCircle3D(const QVector3D &center, const QVector3D &direction, double radius);

    virtual void drawCylinder(double x, double y, double z, double radius, double height);
    virtual void drawCylinder3D(const QVector3D &center, const QVector3D &direction, double radius, double height);

    virtual void drawEllipse(double x, double y, double z, double radiusA, double radiusB);

    virtual void beginDrawMultipleTriangle();
    virtual void drawTriangle(double x1, double y1, double z1,
                                  double x2, double y2, double z2,
                                  double x3, double y3, double z3);
    virtual void drawTriangle(const float *p1,
                              const float *p2,
                              const float *p3);
    virtual void endDrawMultipleTriangle();


    /**  \brief Draw a 3D cube defined by its bounding box (bottom left and top right corner).
      *
      */
    virtual void drawCube(double x1, double y1, double z1, double x2, double y2, double z2);

    /**  \brief Draw a 3D cube defined by its bounding box (bottom left and top right corner).
      *  The drawing mode is given by the user (GL_POINT, GL_LINE or GL_FILL)
      */
    virtual void drawCube(double x1, double y1, double z1, double x2, double y2, double z2, GLenum faces, GLenum mode ) { glPolygonMode(faces, mode); drawCube(x1, y1, z1, x2, y2, z2); }

    /**  \brief Draw a pyramid given its top point and its base
      */
    virtual void drawPyramid(double topX, double topY, double topZ,
                     double base1X, double base1Y, double base1Z,
                     double base2X, double base2Y, double base2Z,
                     double base3X, double base3Y, double base3Z,
                     double base4X, double base4Y, double base4Z);

    /**  \brief Draw a part of a sphere given the angles bounds
      */
    virtual void drawPartOfSphere ( double centerX, double centerY, double centerZ, double radius, double initTheta, double endTheta, double initPhi, double endPhi, bool radians = true );

    virtual void drawRectXY(const QRectF &rectangle, double z);
    virtual void fillRectXY(const QRectF &rectangle, double z);

    virtual void drawRectXZ(const QRectF &rectangle, double y);
    virtual void fillRectXZ(const QRectF &rectangle, double y);

    virtual void drawRectYZ(const QRectF &rectangle, double x);
    virtual void fillRectYZ(const QRectF &rectangle, double x);

    virtual void beginPolygon();
    virtual void addPointToPolygon(float *p);
    virtual void endPolygon();

    virtual void drawQuadFace( float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float x4, float y4, float z4 );

    virtual void fillQuadFace( float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float x4, float y4, float z4 );

    virtual void drawQuadFace( float x1, float y1, float z1, int r1, int g1, int b1,
                               float x2, float y2, float z2, int r2, int g2, int b2,
                               float x3, float y3, float z3, int r3, int g3, int b3,
                               float x4, float y4, float z4, int r4, int g4, int b4 );

    virtual void fillQuadFace( float x1, float y1, float z1, int r1, int g1, int b1,
                               float x2, float y2, float z2, int r2, int g2, int b2,
                               float x3, float y3, float z3, int r3, int g3, int b3,
                               float x4, float y4, float z4, int r4, int g4, int b4 );

protected:

    /**
     * @brief Init shader for points if is not already initialized
     */
    void initPointShader();

    /**
     * @brief Init shader for faces if is not already initialized
     */
    //void initFaceShader();

    /**
     * @brief Init shader for edges if is not already initialized
     */
    //void initEdgeShader();

    /**
     * @brief Bind the point shader
     * @return false bind is ok
     */
    bool bindPointShader(bool force);

    /**
     * @brief Release the point shader
     */
    void releasePointShader(bool bindOk);

    /**
     * @brief Bind the face shader
     * @return false bind is ok
     */
    //bool bindFaceShader();

    /**
     * @brief Release the face shader
     */
    //void releaseFaceShader(bool bindOk);

    /**
     * @brief Bind the edge shader
     * @return false bind is ok
     */
    //bool bindEdgeShader();

    /**
     * @brief Release the edge shader
     */
    //void releaseEdgeShader(bool bindOk);

private:
    G3DGraphicsView                     *m_gv;                  // Graphics View that used the painter

    CT_AbstractCloudT<CT_Point>         *m_pointCloud;
    CT_AbstractCloudT<CT_Face>          *m_faceCloud;
    CT_AbstractCloudT<CT_Edge>          *m_edgeCloud;

    G3DPainter::MultiDrawableType       m_drawOnly;

    QT_GL_SHADERPROGRAM                 *m_shaderProgPoint;     // Shader program used for points
    QT_GL_SHADER                        *m_ShaderPoint;
    bool                                m_shaderProgPointError;
    bool                                m_shaderProgPointSet;
    bool                                m_shaderPointError;
    bool                                m_bindShaderPointOK;

    QColor                              _color;                 // Color used in setCurrentColor method
    QColor                              _forcedColor;           // Color used in setCurrentForcedColor method
    GLUquadric                          *_quadric;              // Quadric used to draw cylinder with gluCylinder
    double                              _defaultPointSize;      // Default point size
    bool                                _drawFastest;           // True if we must draw fastest
    bool                                m_usePColorCloud;       // True if we must use color cloud for points
    bool                                m_usePNormalCloud;      // True if we must use normal cloud for points
    bool                                m_useFColorCloud;       // True if we must use color cloud for faces
    bool                                m_useFNormalCloud;      // True if we must use normal cloud for faces
    bool                                m_useEColorCloud;       // True if we must use color cloud for edges

    bool                                m_drawMultipleLine;
    bool                                m_drawMultipleTriangle;
    bool                                m_drawMultiplePoint;

    bool                                m_beginMultipleEnable;

    int                                 _nCallEnableSetColor;       // count how many times the enableSetColor was called
    int                                 _nCallEnableSetForcedColor; // count how many times the enableSetForcedColor was called

    int                                 _nCallEnableSetPointSize;       // count how many times the enableSetPointSize was called
    int                                 _nCallEnableSetForcedPointSize; // count how many times the enableSetForcedPointSize was called

    int                                 _nCallEnablePushMatrix;         // count how many times the enablePushMatrix was called
    GLint                               m_polygonMode;                  // Polygon Mode backup

    size_t                              m_fastestIncrementPoint;        // fastest increment to use if drawFastest() return true
    int                                 m_octreeCellsDraw;              // count how many octree cells was draw

    static QVector< QPair<double, double> > VECTOR_CIRCLE_FASTEST;
    static const int                        VECTOR_CIRCLE_FASTEST_SIZE = 25;

    static QVector< QPair<double, double> > VECTOR_CIRCLE_NORMAL;
    static const int                        VECTOR_CIRCLE_NORMAL_SIZE = 100;

    void setCurrentColor();
    void setCurrentForcedColor();

    static QVector< QPair<double, double> > staticInitCircleVector(int size);

    void startDrawMultiple();
    void stopDrawMultiple();
    void resetDrawMultiple();
};

#endif // G3DPAINTER_H
