#ifndef G3DFAKEPAINTER_H
#define G3DFAKEPAINTER_H

#include "view/DocumentView/GraphicsViews/3D/g3dpainter.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"

/**
 * @brief A painter that can be used to :
 *           - count points/faces/edges
 *        OR - backup all point/face/edge cloud index that items draw
 *        OR - apply a color to points/faces/edges
 *        OR - draw points/faces/edges with glPushName(index of the points/faces/edges)
 */
class G3DFakePainter : public G3DPainter
{
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

    /**
     * @brief Set what must be draw or count or backup, etc...
     * @warning Call this method after "beginNewDraw()"
     */
    void setDrawMode(DrawModes mode);

    /**
     * @brief Set the color to apply to points/faces/edges (depend on the mode ApplyColorXXX used)
     * @warning Call this method after "beginNewDraw()"
     */
    void setApplyColor(const QColor &color);

    /**
     * @brief Set the points color cloud to use when you want to apply color on points
     * @warning Call this method after "beginNewDraw()"
     */
    void setPointsColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> pColors);

    /**
     * @brief Set the edges color cloud to use when you want to apply color on edges
     * @warning Call this method after "beginNewDraw()"
     */
    void setEdgesColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> eColors);

    /**
     * @brief Set the faces color cloud to use when you want to apply color on faces
     * @warning Call this method after "beginNewDraw()"
     */
    void setFacesColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> fColors);

    /**
     * @brief Returns number of points drawed if DrawModes == CountPoints
     */
    size_t nPoints() const;

    /**
     * @brief Returns number of edges drawed if DrawModes == CountEdges
     */
    size_t nEdges() const;

    /**
     * @brief Returns number of faces drawed if DrawModes == CountFaces
     */
    size_t nFaces() const;

    /**
     * @brief Returns the list of point cloud index that was drawed if DrawModes == BackupPointCloudIndex
     */
    const QList<CT_AbstractCloudIndex*>& pointCloudIndexBackup() const;

    /**
     * @brief Returns the list of edge cloud index that was drawed if DrawModes == BackupEdgeCloudIndex
     */
    const QList<CT_AbstractCloudIndex*>& edgeCloudIndexBackup() const;

    /**
     * @brief Returns the list of face cloud index that was drawed if DrawModes == BackupFaceCloudIndex
     */
    const QList<CT_AbstractCloudIndex*>& faceCloudIndexBackup() const;

    void beginNewDraw();
    void endNewDraw();

    void save();
    void restore();

    void startRestoreIdentityMatrix(GLdouble *matrix = NULL);
    void stopRestoreIdentityMatrix();

    void pushMatrix();
    void multMatrix(const Eigen::Matrix4d &matrix);
    void popMatrix();

    void setPointSize(float size);
    void setDefaultPointSize(float size);
    void restoreDefaultPointSize();

    void translate(const double &x, const double &y, const double &z);
    void rotate(const double &alpha, const double &x, const double &y, const double &z);
    void translateThenRotateToDirection(const Eigen::Vector3d &translation, const Eigen::Vector3d &direction);
    void scale(const double &x, const double &y, const double &z);

    ///////// GL_POINTS //////////

    void drawPoint(const size_t &globalIndex);
    void drawPoints(const CT_AbstractMeshModel *mesh);
    void drawPointCloud(const CT_AbstractCloudIndex *pci);
    void drawOctreeOfPoints(const OctreeInterface *octree, DrawOctreeModes modes);

    ///////// GL_LINES //////////

    void drawEdges(const CT_AbstractMeshModel *mesh);

    void drawLine(const size_t &p1GlobalIndex,
                  const size_t &p2GlobalIndex);

    ///////// GL_TRIANGLES //////////

    void drawMesh(const CT_AbstractMeshModel *mesh);
    void drawFaces(const CT_AbstractMeshModel *mesh);

    void drawTriangle(const size_t &p1GlobalIndex,
                      const size_t &p2GlobalIndex,
                      const size_t &p3GlobalIndex);

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

    void drawPoint(const double &x, const double &y, const double &z) { Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(z) }
    void drawPoint(double *p) { Q_UNUSED(p) }


    void drawLine(const double &x1, const double &y1, const double &z1,
                  const double &x2, const double &y2, const double &z2) {
        Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1)
        Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2)
    }

    void drawCircle(const double &x, const double &y, const double &z, const double &radius) { Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(z) Q_UNUSED(radius) }
    void drawCircle3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius) { Q_UNUSED(center) Q_UNUSED(direction) Q_UNUSED(radius) }

    void drawCylinder(const double &x, const double &y, const double &z, const double &radius, const double &height) { Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(z) Q_UNUSED(radius) Q_UNUSED(height) }
    void drawCylinder3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius, const double &height) { Q_UNUSED(center) Q_UNUSED(direction) Q_UNUSED(radius) Q_UNUSED(height) }

    void drawEllipse(const double &x, const double &y, const double &z, const double &radiusA, const double &radiusB) { Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(z) Q_UNUSED(radiusA) Q_UNUSED(radiusB) }

    void drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2) { Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1) Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2) }
    void drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2, GLenum faces, GLenum mode ) { Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1) Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2) Q_UNUSED(faces) Q_UNUSED(mode) }

    void drawPyramid(const double &topX, const double &topY, const double &topZ,
                     const double &base1X, const double &base1Y, const double &base1Z,
                     const double &base2X, const double &base2Y, const double &base2Z,
                     const double &base3X, const double &base3Y, const double &base3Z,
                     const double &base4X, const double &base4Y, const double &base4Z) {
         Q_UNUSED(topX) Q_UNUSED(topY) Q_UNUSED(topZ)
         Q_UNUSED(base1X) Q_UNUSED(base1Y) Q_UNUSED(base1Z)
         Q_UNUSED(base2X) Q_UNUSED(base2Y) Q_UNUSED(base2Z)
         Q_UNUSED(base3X) Q_UNUSED(base3Y) Q_UNUSED(base3Z)
         Q_UNUSED(base4X) Q_UNUSED(base4Y) Q_UNUSED(base4Z)
    }

    void drawPartOfSphere ( const double &centerX, const double &centerY, const double &centerZ,
                            const double &radius, const double &initTheta, const double &endTheta,
                            const double &initPhi, const double &endPhi, bool radians = true ) {
        Q_UNUSED(centerX) Q_UNUSED(centerY) Q_UNUSED(centerZ)
        Q_UNUSED(radius) Q_UNUSED(initTheta) Q_UNUSED(endTheta)
        Q_UNUSED(initPhi) Q_UNUSED(endPhi) Q_UNUSED(radians)
    }

    void drawRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z) { Q_UNUSED(topLeft) Q_UNUSED(bottomRight) Q_UNUSED(z) }
    void fillRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z) { Q_UNUSED(topLeft) Q_UNUSED(bottomRight) Q_UNUSED(z) }

    void drawRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y) { Q_UNUSED(topLeft) Q_UNUSED(bottomRight) Q_UNUSED(y) }
    void fillRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y) { Q_UNUSED(topLeft) Q_UNUSED(bottomRight) Q_UNUSED(y) }

    void drawRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x) { Q_UNUSED(topLeft) Q_UNUSED(bottomRight) Q_UNUSED(x) }
    void fillRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x) { Q_UNUSED(topLeft) Q_UNUSED(bottomRight) Q_UNUSED(x) }

    void beginPolygon() {}
    void addPointToPolygon(const double &x, const double &y, const double &z) { Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(z) }
    void endPolygon() {}

    void drawTriangle(const double &x1, const double &y1, const double &z1,
                      const double &x2, const double &y2, const double &z2,
                      const double &x3, const double &y3, const double &z3) {

        Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1)
        Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2)
        Q_UNUSED(x3) Q_UNUSED(y3) Q_UNUSED(z3)
    }

    void drawQuadFace( const double &x1, const double &y1, const double &z1,
                               const double &x2, const double &y2, const double &z2,
                               const double &x3, const double &y3, const double &z3,
                               const double &x4, const double &y4, const double &z4 ) {
        Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1)
        Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2)
        Q_UNUSED(x3) Q_UNUSED(y3) Q_UNUSED(z3)
        Q_UNUSED(x4) Q_UNUSED(y4) Q_UNUSED(z4)
    }

    void fillQuadFace( const double &x1, const double &y1, const double &z1,
                               const double &x2, const double &y2, const double &z2,
                               const double &x3, const double &y3, const double &z3,
                               const double &x4, const double &y4, const double &z4 ) {

        Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1)
        Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2)
        Q_UNUSED(x3) Q_UNUSED(y3) Q_UNUSED(z3)
        Q_UNUSED(x4) Q_UNUSED(y4) Q_UNUSED(z4)
    }

    void drawQuadFace( const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                               const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                               const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                               const double &x4, const double &y4, const double &z4, int r4, int g4, int b4 ) {

        Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1) Q_UNUSED(r1) Q_UNUSED(g1) Q_UNUSED(g1) Q_UNUSED(b1)
        Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2) Q_UNUSED(r2) Q_UNUSED(g2) Q_UNUSED(g2) Q_UNUSED(b2)
        Q_UNUSED(x3) Q_UNUSED(y3) Q_UNUSED(z3) Q_UNUSED(r3) Q_UNUSED(g3) Q_UNUSED(g3) Q_UNUSED(b3)
        Q_UNUSED(x4) Q_UNUSED(y4) Q_UNUSED(z4) Q_UNUSED(r4) Q_UNUSED(g4) Q_UNUSED(g4) Q_UNUSED(b4)
    }

    void fillQuadFace( const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                               const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                               const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                               const double &x4, const double &y4, const double &z4, int r4, int g4, int b4 ) {
        Q_UNUSED(x1) Q_UNUSED(y1) Q_UNUSED(z1) Q_UNUSED(r1) Q_UNUSED(g1) Q_UNUSED(g1) Q_UNUSED(b1)
        Q_UNUSED(x2) Q_UNUSED(y2) Q_UNUSED(z2) Q_UNUSED(r2) Q_UNUSED(g2) Q_UNUSED(g2) Q_UNUSED(b2)
        Q_UNUSED(x3) Q_UNUSED(y3) Q_UNUSED(z3) Q_UNUSED(r3) Q_UNUSED(g3) Q_UNUSED(g3) Q_UNUSED(b3)
        Q_UNUSED(x4) Q_UNUSED(y4) Q_UNUSED(z4) Q_UNUSED(r4) Q_UNUSED(g4) Q_UNUSED(g4) Q_UNUSED(b4)
    }

private:
    DrawModes                                       m_drawMode;
    bool                                            m_drawEnabled;
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
};

Q_DECLARE_OPERATORS_FOR_FLAGS(G3DFakePainter::DrawModes)

#endif // G3DFAKEPAINTER_H
