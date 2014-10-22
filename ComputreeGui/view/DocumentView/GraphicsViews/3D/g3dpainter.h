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

#include <QtOpenGL>

// GLU was removed from Qt in version 4.8
#ifdef Q_OS_MAC
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif

class G3DPainter : public PainterInterface
{
    Q_INTERFACES(PainterInterface)

public:

    G3DPainter();
    virtual ~G3DPainter();

    void setGraphicsView(const GraphicsViewInterface *gv);
    void setCurrentItemDrawable(const CT_AbstractItemDrawable *item);
    void setCurrentPointCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc);
    void setCurrentFaceCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc);
    void setCurrentEdgeCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc);
    void setCurrentPointCloudNormal(QSharedPointer<CT_StandardNormalCloudRegistered> nn);
    void setCurrentFaceCloudNormal(QSharedPointer<CT_StandardNormalCloudRegistered> nn);

    QSharedPointer<CT_StandardColorCloudRegistered> currentPointCloudColor() const;
    QSharedPointer<CT_StandardColorCloudRegistered> currentFaceCloudColor() const;
    QSharedPointer<CT_StandardColorCloudRegistered> currentEdgeCloudColor() const;
    QSharedPointer<CT_StandardNormalCloudRegistered> currentPointCloudNormal() const;
    QSharedPointer<CT_StandardNormalCloudRegistered> currentFaceCloudNormal() const;
    QSharedPointer<CT_StandardNormalCloudRegistered> currentEdgeCloudNormal() const;

    /**
     * @brief Set the fastest increment to use in drawPointCloud method. If 0 use the fastest increment passed in parameter;
     */
    void setPointFastestIncrement(size_t inc);

    /**
     * @brief Returns the number of cells of the octree drawed
     */
    int nOctreeCellsDrawed() const;

    bool drawFastest() const;
    void setDrawFastest(bool enable);

    void beginNewDraw();
    void endNewDraw();

    void save();
    void restore();

    void startRestoreIdentityMatrix(GLdouble *matrix = NULL);
    void stopRestoreIdentityMatrix();

    void enableMultMatrix(bool e);

    void pushMatrix();
    void multMatrix(const QMatrix4x4 &matrix);
    void popMatrix();

    void setPointSize(double size);
    void setDefaultPointSize(double size);
    void restoreDefaultPointSize();

    void setPen(const QPen &pen);
    void restoreDefaultPen();

    void setColor(int r, int g, int b);
    void setColor(QColor color);
    void setForcedColor(int r, int g, int b);
    void setForcedColor(QColor color);
    void setUseColorCloudForPoints(bool enable);
    void setUseNormalCloudForPoints(bool enable);
    void setUseColorCloudForFaces(bool enable);
    void setUseNormalCloudForFaces(bool enable);
    void setUseColorCloudForEdges(bool enable);
    QColor getColor();

    void enableSetColor(bool enable);
    void enableSetForcedColor(bool enable);

    void enableSetPointSize(bool enable);
    void enableSetForcedPointSize(bool enable);

    void enableDrawPointCloud(bool enable);

    void translate(double x, double y, double z);
    void rotate(double alpha, double x, double y, double z);
    void translateThenRotateToDirection(const QVector3D &translation, const QVector3D &direction);
    void scale(double x, double y, double z);

    void drawOctreeOfPoints(const OctreeInterface *octree, DrawOctreeModes modes);

    void drawPoint(double x, double y, double z);
    void drawPoint(double *p);
    void drawPoint(float *p);

    void drawPointCloud(const CT_AbstractPointCloud *pc,
                        const CT_AbstractCloudIndex *pci,
                        int fastestIncrement);

    void drawMesh(const CT_AbstractMeshModel *mesh);
    void drawFaces(const CT_AbstractMeshModel *mesh);
    void drawEdges(const CT_AbstractMeshModel *mesh);
    void drawPoints(const CT_AbstractMeshModel *mesh, int fastestIncrement);

    void beginMultiplePoints();
    void addPoint(float *p);
    void endMultiplePoints();

    void beginDrawMultipleLine();
    void drawLine(double x1, double y1, double z1, double x2, double y2, double z2);
    void drawLine(const float *p1, const float *p2);
    void endDrawMultipleLine();

    void drawCircle(double x, double y, double z, double radius);
    void drawCircle3D(const QVector3D &center, const QVector3D &direction, double radius);

    void drawCylinder(double x, double y, double z, double radius, double height);
    void drawCylinder3D(const QVector3D &center, const QVector3D &direction, double radius, double height);

    void drawEllipse(double x, double y, double z, double radiusA, double radiusB);

    void beginDrawMultipleTriangle();
    void drawTriangle(double x1, double y1, double z1,
                                  double x2, double y2, double z2,
                                  double x3, double y3, double z3);
    void drawTriangle(const float *p1,
                              const float *p2,
                              const float *p3);
    void endDrawMultipleTriangle();


    /**  \brief Draw a 3D cube defined by its bounding box (bottom left and top right corner).
      *
      */
    void drawCube(double x1, double y1, double z1, double x2, double y2, double z2);

    /**  \brief Draw a 3D cube defined by its bounding box (bottom left and top right corner).
      *  The drawing mode is given by the user (GL_POINT, GL_LINE or GL_FILL)
      */
    inline void drawCube(double x1, double y1, double z1, double x2, double y2, double z2, GLenum faces, GLenum mode ) { glPolygonMode(faces, mode); drawCube(x1, y1, z1, x2, y2, z2); }

    /**  \brief Draw a pyramid given its top point and its base
      */
    void drawPyramid(double topX, double topY, double topZ,
                     double base1X, double base1Y, double base1Z,
                     double base2X, double base2Y, double base2Z,
                     double base3X, double base3Y, double base3Z,
                     double base4X, double base4Y, double base4Z);

    /**  \brief Draw a part of a sphere given the angles bounds
      */
    void drawPartOfSphere ( double centerX, double centerY, double centerZ, double radius, double initTheta, double endTheta, double initPhi, double endPhi, bool radians = true );

    void drawRectXY(const QRectF &rectangle, double z);
    void fillRectXY(const QRectF &rectangle, double z);

    void drawRectXZ(const QRectF &rectangle, double y);
    void fillRectXZ(const QRectF &rectangle, double y);

    void drawRectYZ(const QRectF &rectangle, double x);
    void fillRectYZ(const QRectF &rectangle, double x);

    void beginPolygon();
    void addPointToPolygon(float *p);
    void endPolygon();

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

private:
    CT_AbstractItemDrawable                             *m_currentItem;

    GraphicsViewInterface                               *m_gv;

    QSharedPointer<CT_StandardColorCloudRegistered>     m_pColorCloud;
    QSharedPointer<CT_StandardColorCloudRegistered>     m_fColorCloud;
    QSharedPointer<CT_StandardColorCloudRegistered>     m_eColorCloud;
    QSharedPointer<CT_StandardNormalCloudRegistered>    m_pNormalCloud;
    QSharedPointer<CT_StandardNormalCloudRegistered>    m_fNormalCloud;

    QColor          _color;
    QColor          _forcedColor;
    GLUquadric      *_quadric;
    double          _defaultPointSize;
    bool            _drawFastest;
    bool            m_drawMultipleLine;
    bool            m_drawMultipleTriangle;
    bool            m_useColorCloud;
    bool            m_useNormalCloud;
    bool            m_useFColorCloud;
    bool            m_useFNormalCloud;
    bool            m_useEColorCloud;
    bool            m_drawPointCloudEnabled;

    int             _nCallEnableSetColor;
    int             _nCallEnableSetForcedColor;

    int             _nCallEnableSetPointSize;
    int             _nCallEnableSetForcedPointSize;

    int             _nCallEnablePushMatrix;
    GLint           m_polygonMode;

    size_t          m_fastestIncrementPoint;
    int             m_octreeCellsDraw;

    static QVector< QPair<double, double> > VECTOR_CIRCLE_FASTEST;
    static const int                        VECTOR_CIRCLE_FASTEST_SIZE = 5;

    static QVector< QPair<double, double> > VECTOR_CIRCLE_NORMAL;
    static const int                        VECTOR_CIRCLE_NORMAL_SIZE = 100;

    void setCurrentColor();
    void setCurrentForcedColor();

    static QVector< QPair<double, double> > staticInitCircleVector(int size);
};

#endif // G3DPAINTER_H
