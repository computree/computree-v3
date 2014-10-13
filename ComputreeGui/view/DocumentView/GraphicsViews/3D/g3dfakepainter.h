#ifndef G3DFAKEPAINTER_H
#define G3DFAKEPAINTER_H

#include "interfaces.h"
#include "dm_graphicsviewoptions.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"

#include <QtOpenGL>

// GLU was removed from Qt in version 4.8
#ifdef Q_OS_MAC
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif

/**
 * @brief A painter that draw only points or faces or edges with names (the name is the index of the point/face/edge)
 */
class G3DFakePainter : public PainterInterface
{
    Q_INTERFACES(PainterInterface)

public:
    enum DrawMode {
        DrawNone = 0,
        DrawPointsWithName = 1,             // use this mode if you want to enable draw points and draw it with name
        DrawFacesWithName = 2,              // use this mode if you want to enable draw faces and draw it with name
        DrawEdgesWithName = 4,              // use this mode if you want to enable draw edges and draw it with name
        CountPoints = 8,                    // use this mode if you want to count points
        CountEdges = 16,                    // use this mode if you want to count edges
        CountFaces = 32,                    // use this mode if you want to count faces
        BackupPointCloudIndex = 64,         // use this mode if you want to backup all points index in a list
        BackupFaceCloudIndex = 128,         // use this mode if you want to backup all faces index in a list
        BackupEdgeCloudIndex = 256,         // use this mode if you want to backup all edges index in a list
        ApplyColorPoints = 512,             // use this mode if you want to apply a color to points
        ApplyColorFaces = 1024,             // use this mode if you want to apply a color to faces
        ApplyColorEdges = 2048              // use this mode if you want to apply a color to edges
    };

    Q_DECLARE_FLAGS(DrawModes, DrawMode)

    G3DFakePainter();

    void setGraphicsView(const GraphicsViewInterface *gv);

    /**
     * @brief Set the color to apply to points/faces/edges (depend on the mode ApplyColorXXX used)
     */
    void setApplyColor(const QColor &color);

    /**
     * @brief Set the points color cloud to use when you want to apply color on points
     */
    void setPointsColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> pColors);

    /**
     * @brief Set the edges color cloud to use when you want to apply color on edges
     */
    void setEdgesColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> eColors);

    /**
     * @brief Set the faces color cloud to use when you want to apply color on faces
     */
    void setFacesColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> fColors);

    void beginNewDraw();
    void endNewDraw() {}

    void setDrawMode(DrawModes mode);

    bool drawFastest() const;
    void setDrawFastest(bool enable);

    size_t nPoints() const;
    size_t nEdges() const;
    size_t nFaces() const;

    const QList<CT_AbstractCloudIndex*>& pointCloudIndexBackup() const;
    const QList<CT_AbstractCloudIndex*>& edgeCloudIndexBackup() const;
    const QList<CT_AbstractCloudIndex*>& faceCloudIndexBackup() const;

    void save();
    void restore();

    void startRestoreIdentityMatrix(GLdouble *matrix = NULL);
    void stopRestoreIdentityMatrix();

    void setPointSize(double size);
    void setDefaultPointSize(double size);
    void restoreDefaultPointSize();

    void enableSetPointSize(bool enable);
    void enableSetForcedPointSize(bool enable);

    void translate(double x, double y, double z);
    void rotate(double alpha, double x, double y, double z);
    void scale(double x, double y, double z);

    void drawOctreeOfPoints(const OctreeInterface *octree, DrawOctreeModes modes);

    void drawPointCloud(const CT_AbstractPointCloud *pc,
                        const CT_AbstractCloudIndex *pci,
                        int fastestIncrement);

    void drawMesh(const CT_AbstractMeshModel *mesh);
    void drawFaces(const CT_AbstractMeshModel *mesh);
    void drawEdges(const CT_AbstractMeshModel *mesh);
    void drawPoints(const CT_AbstractMeshModel *mesh, int fastestIncrement);

    // called from a mesh to draw Faces
    void beginDrawMultipleTriangle();
    void drawTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3);
    void drawTriangle(const float *p1, const float *p2, const float *p3);
    void endDrawMultipleTriangle();

    // called from a mesh to draw Edges
    void beginDrawMultipleLine();
    void drawLine(double x1, double y1, double z1, double x2, double y2, double z2);
    void drawLine(const float *p1, const float *p2);
    void endDrawMultipleLine();

    /*********** METHOD NOT USED **********/

    void setPen(const QPen &pen) { Q_UNUSED(pen) }
    void restoreDefaultPen() {}
    void setColor(int r, int g, int b) { Q_UNUSED(r) Q_UNUSED(g) Q_UNUSED(b) }
    void setColor(QColor color) { Q_UNUSED(color) }
    void setForcedColor(int r, int g, int b) { Q_UNUSED(r) Q_UNUSED(g) Q_UNUSED(b) }
    void setForcedColor(QColor color) { Q_UNUSED(color) }
    QColor getColor() { return Qt::white; }

    void enableSetColor(bool enable) { Q_UNUSED(enable) }
    void enableSetForcedColor(bool enable) { Q_UNUSED(enable) }

    void drawPoint(double x, double y, double z) { Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(z) }
    void drawPoint(double *p) { Q_UNUSED(p) }
    void drawPoint(float *p) { Q_UNUSED(p) }

    void beginMultiplePoints() {}
    void addPoint(float *p) { Q_UNUSED(p) }
    void endMultiplePoints() {}

    void drawCircle(double x, double y, double z, double radius) { Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(z) Q_UNUSED(radius) }
    void drawCircle3D(const QVector3D &center, const QVector3D &direction, double radius) { Q_UNUSED(center) Q_UNUSED(direction) Q_UNUSED(radius) }

    void drawCylinder(double x, double y, double z, double radius, double height) { Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(z) Q_UNUSED(radius) Q_UNUSED(height) }
    void drawCylinder3D(const QVector3D &center, const QVector3D &direction, double radius, double height) { Q_UNUSED(center) Q_UNUSED(direction) Q_UNUSED(radius) Q_UNUSED(height) }

    void drawEllipse(double x, double y, double z, double radiusA, double radiusB) { Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(z) Q_UNUSED(radiusA) Q_UNUSED(radiusB) }

    void drawCube(double x1, double y1, double z1, double x2, double y2, double z2) { Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1) Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2) }
    void drawCube(double x1, double y1, double z1, double x2, double y2, double z2, GLenum faces, GLenum mode ) { Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1) Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2) Q_UNUSED(faces) Q_UNUSED(mode) }

    void drawPyramid(double topX, double topY, double topZ,
                     double base1X, double base1Y, double base1Z,
                     double base2X, double base2Y, double base2Z,
                     double base3X, double base3Y, double base3Z,
                     double base4X, double base4Y, double base4Z) {
         Q_UNUSED(topX) Q_UNUSED(topY) Q_UNUSED(topZ)
         Q_UNUSED(base1X) Q_UNUSED(base1Y) Q_UNUSED(base1Z)
         Q_UNUSED(base2X) Q_UNUSED(base2Y) Q_UNUSED(base2Z)
         Q_UNUSED(base3X) Q_UNUSED(base3Y) Q_UNUSED(base3Z)
         Q_UNUSED(base4X) Q_UNUSED(base4Y) Q_UNUSED(base4Z)
    }

    void drawPartOfSphere ( double centerX, double centerY, double centerZ,
                            double radius, double initTheta, double endTheta,
                            double initPhi, double endPhi, bool radians = true ) {
        Q_UNUSED(centerX) Q_UNUSED(centerY) Q_UNUSED(centerZ)
        Q_UNUSED(radius) Q_UNUSED(initTheta) Q_UNUSED(endTheta)
        Q_UNUSED(initPhi) Q_UNUSED(endPhi) Q_UNUSED(radians)
    }

    void drawRectXY(const QRectF &rectangle, double z) { Q_UNUSED(rectangle) Q_UNUSED(z) }
    void fillRectXY(const QRectF &rectangle, double z) { Q_UNUSED(rectangle) Q_UNUSED(z) }

    void drawRectXZ(const QRectF &rectangle, double y) { Q_UNUSED(rectangle) Q_UNUSED(y) }
    void fillRectXZ(const QRectF &rectangle, double y) { Q_UNUSED(rectangle) Q_UNUSED(y) }

    void drawRectYZ(const QRectF &rectangle, double x) { Q_UNUSED(rectangle) Q_UNUSED(x) }
    void fillRectYZ(const QRectF &rectangle, double x) { Q_UNUSED(rectangle) Q_UNUSED(x) }

    void beginPolygon() {}
    void addPointToPolygon(float *p) { Q_UNUSED(p) }
    void endPolygon() {}

    void drawQuadFace( float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float x4, float y4, float z4 ) {
         Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1)
                Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2)
                Q_UNUSED(x3) Q_UNUSED(y3) Q_UNUSED(z3)
                Q_UNUSED(x4) Q_UNUSED(y4) Q_UNUSED(z4)
    }

    void fillQuadFace( float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float x4, float y4, float z4 ) {

        Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1)
               Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2)
               Q_UNUSED(x3) Q_UNUSED(y3) Q_UNUSED(z3)
               Q_UNUSED(x4) Q_UNUSED(y4) Q_UNUSED(z4)
    }

    void drawQuadFace( float x1, float y1, float z1, int r1, int g1, int b1,
                               float x2, float y2, float z2, int r2, int g2, int b2,
                               float x3, float y3, float z3, int r3, int g3, int b3,
                               float x4, float y4, float z4, int r4, int g4, int b4 ) {

        Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1) Q_UNUSED(r1) Q_UNUSED(g1) Q_UNUSED(g1) Q_UNUSED(b1)
               Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2) Q_UNUSED(r2) Q_UNUSED(g2) Q_UNUSED(g2) Q_UNUSED(b2)
               Q_UNUSED(x3) Q_UNUSED(y3) Q_UNUSED(z3) Q_UNUSED(r3) Q_UNUSED(g3) Q_UNUSED(g3) Q_UNUSED(b3)
               Q_UNUSED(x4) Q_UNUSED(y4) Q_UNUSED(z4) Q_UNUSED(r4) Q_UNUSED(g4) Q_UNUSED(g4) Q_UNUSED(b4)
    }

    void fillQuadFace( float x1, float y1, float z1, int r1, int g1, int b1,
                               float x2, float y2, float z2, int r2, int g2, int b2,
                               float x3, float y3, float z3, int r3, int g3, int b3,
                               float x4, float y4, float z4, int r4, int g4, int b4 ) {
        Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1) Q_UNUSED(r1) Q_UNUSED(g1) Q_UNUSED(g1) Q_UNUSED(b1)
               Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2) Q_UNUSED(r2) Q_UNUSED(g2) Q_UNUSED(g2) Q_UNUSED(b2)
               Q_UNUSED(x3) Q_UNUSED(y3) Q_UNUSED(z3) Q_UNUSED(r3) Q_UNUSED(g3) Q_UNUSED(g3) Q_UNUSED(b3)
               Q_UNUSED(x4) Q_UNUSED(y4) Q_UNUSED(z4) Q_UNUSED(r4) Q_UNUSED(g4) Q_UNUSED(g4) Q_UNUSED(b4)
    }

private:
    DrawModes                                       m_drawMode;
    bool                                            m_drawEnabled;
    bool                                            m_drawFastest;
    double                                          m_defaultPointSize;
    uint                                            m_nCallEnableSetPointSize;
    uint                                            m_nCallEnableSetForcedPointSize;
    bool                                            m_drawMultipleLine;
    bool                                            m_drawMultipleTriangle;
    size_t                                          m_nPoints;
    size_t                                          m_nEdges;
    size_t                                          m_nFaces;
    QList<CT_AbstractCloudIndex*>                   m_pCloudIndexBackup;
    QList<CT_AbstractCloudIndex*>                   m_fCloudIndexBackup;
    QList<CT_AbstractCloudIndex*>                   m_eCloudIndexBackup;
    QColor                                          m_applyColor;
    QSharedPointer<CT_StandardColorCloudRegistered> m_pColors;
    QSharedPointer<CT_StandardColorCloudRegistered> m_eColors;
    QSharedPointer<CT_StandardColorCloudRegistered> m_fColors;

    GraphicsViewInterface                           *m_gv;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(G3DFakePainter::DrawModes)

#endif // G3DFAKEPAINTER_H
