/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
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

#include "g3dpainter.h"

#include "dm_guimanager.h"

#include "ct_global/ct_context.h"
#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_normalcloud/registered/ct_standardnormalcloudregistered.h"
#include "ct_normalcloud/abstract/ct_abstractnormalcloud.h"
#include "ct_itemdrawable/ct_meshmodel.h"
#include "ct_cloudindex/ct_cloudindexlessmemoryt.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

#include <QQuaternion>

#define M_PI_MULT_2 6.28318530717958647692

QVector< QPair<double, double> > G3DPainter::VECTOR_CIRCLE_FASTEST = G3DPainter::staticInitCircleVector(G3DPainter::VECTOR_CIRCLE_FASTEST_SIZE);
QVector< QPair<double, double> > G3DPainter::VECTOR_CIRCLE_NORMAL = G3DPainter::staticInitCircleVector(G3DPainter::VECTOR_CIRCLE_NORMAL_SIZE);

G3DPainter::G3DPainter()
{
    m_currentItem = NULL;
    m_gv = NULL;

    _quadric = gluNewQuadric();
    _color = Qt::white;
    _forcedColor = Qt::white;
    _defaultPointSize = 1.0;
    _drawFastest = false;

    beginNewDraw();
}

G3DPainter::~G3DPainter()
{
    gluDeleteQuadric(_quadric);
}

void G3DPainter::setGraphicsView(const GraphicsViewInterface *gv)
{
    m_gv = (GraphicsViewInterface*)gv;
}

void G3DPainter::setCurrentItemDrawable(const CT_AbstractItemDrawable *item)
{
    m_currentItem = (CT_AbstractItemDrawable*)item;
}

void G3DPainter::setCurrentPointCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc)
{
    m_pColorCloud = cc;
}

void G3DPainter::setCurrentFaceCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc)
{
    m_fColorCloud = cc;
}

void G3DPainter::setCurrentEdgeCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc)
{
    m_eColorCloud = cc;
}

void G3DPainter::setCurrentPointCloudNormal(QSharedPointer<CT_StandardNormalCloudRegistered> nn)
{
    m_pNormalCloud = nn;
}

void G3DPainter::setCurrentFaceCloudNormal(QSharedPointer<CT_StandardNormalCloudRegistered> nn)
{
    m_fNormalCloud = nn;
}

QSharedPointer<CT_StandardColorCloudRegistered> G3DPainter::currentPointCloudColor() const
{
    return m_pColorCloud;
}

QSharedPointer<CT_StandardColorCloudRegistered> G3DPainter::currentFaceCloudColor() const
{
    return m_fColorCloud;
}

QSharedPointer<CT_StandardColorCloudRegistered> G3DPainter::currentEdgeCloudColor() const
{
    return m_eColorCloud;
}

QSharedPointer<CT_StandardNormalCloudRegistered> G3DPainter::currentPointCloudNormal() const
{
    return m_pNormalCloud;
}

QSharedPointer<CT_StandardNormalCloudRegistered> G3DPainter::currentFaceCloudNormal() const
{
    return m_fNormalCloud;
}

QSharedPointer<CT_StandardNormalCloudRegistered> G3DPainter::currentEdgeCloudNormal() const
{
    return QSharedPointer<CT_StandardNormalCloudRegistered>(NULL);
}

void G3DPainter::setPointFastestIncrement(size_t inc)
{
    m_fastestIncrementPoint = inc;
}

int G3DPainter::nOctreeCellsDrawed() const
{
    return m_octreeCellsDraw;
}

bool G3DPainter::drawFastest() const
{
    return _drawFastest;
}

void G3DPainter::setDrawFastest(bool enable)
{
    _drawFastest = enable;
}

void G3DPainter::beginNewDraw()
{
    _nCallEnableSetColor = 0;
    _nCallEnableSetForcedColor = 0;

    _nCallEnableSetPointSize = 0;
    _nCallEnableSetForcedPointSize = 0;

    _nCallEnablePushMatrix = 0;

    m_useColorCloud = true;
    m_useNormalCloud = true;
    m_useFColorCloud = true;
    m_useFNormalCloud = true;
    m_useEColorCloud = true;

    m_drawMultipleLine = false;
    m_drawMultipleTriangle = false;

    m_drawPointCloudEnabled = true;
    m_fastestIncrementPoint = 0;

    m_octreeCellsDraw = 0;
}

void G3DPainter::endNewDraw()
{
}

void G3DPainter::save()
{
    glPushMatrix();
    glGetIntegerv(GL_POLYGON_MODE, &m_polygonMode);
}

void G3DPainter::restore()
{
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
}

void G3DPainter::startRestoreIdentityMatrix(GLdouble *matrix)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    double aspectRatio = m_gv->width() / ((double)m_gv->height());

    if(m_gv->width() <= ((double)m_gv->height()))
        glOrtho(-1, 1, -1/aspectRatio, 1/aspectRatio, -1, 1);
    else
        glOrtho(-1*aspectRatio, 1*aspectRatio, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    if(matrix != NULL)
        glMultMatrixd(matrix);
}

void G3DPainter::stopRestoreIdentityMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void G3DPainter::enableMultMatrix(bool e)
{
    _nCallEnablePushMatrix += (e ? 1 : -1);
}

void G3DPainter::pushMatrix()
{
    if(_nCallEnablePushMatrix == 0)
        glPushMatrix();
}

// add overloaded functions which call the underlying OpenGL function
inline void glMultMatrix(const GLfloat  *m) { glMultMatrixf(m); }
inline void glMultMatrix(const GLdouble *m) { glMultMatrixd(m); }

// add an overload for QMatrix4x4 for convenience
inline void glMultMatrix(const QMatrix4x4 &m) { glMultMatrix(m.constData()); }

void G3DPainter::multMatrix(const QMatrix4x4 &matrix)
{
    if(_nCallEnablePushMatrix == 0)
        glMultMatrix(matrix);
}

void G3DPainter::popMatrix()
{
    if(_nCallEnablePushMatrix == 0)
        glPopMatrix();
}

void G3DPainter::setPointSize(double size)
{
    if(_nCallEnableSetPointSize == 0)
    {
        glPointSize(size);
    }
}

void G3DPainter::setDefaultPointSize(double size)
{
    if(_nCallEnableSetPointSize == 0)
    {
        _defaultPointSize = size;
    }
}

void G3DPainter::restoreDefaultPointSize()
{
    if(_nCallEnableSetPointSize == 0)
    {
        glPointSize(_defaultPointSize);
    }
}

void G3DPainter::restoreDefaultPen()
{
    glLineWidth(1);
    glDisable(GL_LINE_STIPPLE);

    _color = Qt::white;
    setCurrentColor();
}

void G3DPainter::setPen(const QPen &pen)
{
    if(_nCallEnableSetColor == 0)
    {
        _color = pen.color();
        setCurrentColor();

        glLineWidth(pen.width());

        switch (pen.style())
        {
        case Qt::DashLine:
            glLineStipple(1, 0x00FF);
            glEnable(GL_LINE_STIPPLE);
            break;
        case Qt::DotLine:
            glLineStipple(2, 0xAAAA);
            glEnable(GL_LINE_STIPPLE);
            break;
        case Qt::DashDotLine:
            glLineStipple(1, 0xFF18);
            glEnable(GL_LINE_STIPPLE);
            break;
        case Qt::DashDotDotLine:
            glLineStipple(1, 0xF44F);
            glEnable(GL_LINE_STIPPLE);
            break;
        default:
            glDisable(GL_LINE_STIPPLE);
            break;
        }
    }
}

void G3DPainter::setColor(int r, int g, int b)
{
    if(_nCallEnableSetColor == 0)
    {
        _color.setRed(r);
        _color.setGreen(g);
        _color.setBlue(b);

        setCurrentColor();
    }
}

QColor G3DPainter::getColor()
{
    return _color;
}

void G3DPainter::setColor(QColor color)
{
    if(_nCallEnableSetColor == 0)
    {
        _color = color;

        setCurrentColor();
    }
}

void G3DPainter::setForcedColor(int r, int g, int b)
{
    if(_nCallEnableSetForcedColor == 0)
    {
        _forcedColor.setRed(r);
        _forcedColor.setGreen(g);
        _forcedColor.setBlue(b);

        setCurrentForcedColor();
    }
}

void G3DPainter::setForcedColor(QColor color)
{
    if(_nCallEnableSetForcedColor == 0)
    {
        _forcedColor = color;

        setCurrentForcedColor();
    }
}

void G3DPainter::setUseColorCloudForPoints(bool enable)
{
    m_useColorCloud = enable;
}

void G3DPainter::setUseNormalCloudForPoints(bool enable)
{
    m_useNormalCloud = enable;
}

void G3DPainter::setUseColorCloudForFaces(bool enable)
{
    m_useFColorCloud = enable;
}

void G3DPainter::setUseNormalCloudForFaces(bool enable)
{
    m_useFNormalCloud = enable;
}

void G3DPainter::setUseColorCloudForEdges(bool enable)
{
    m_useEColorCloud = enable;
}

void G3DPainter::enableSetColor(bool enable)
{
    _nCallEnableSetColor += (enable ? 1 : -1);
}

void G3DPainter::enableSetForcedColor(bool enable)
{
    _nCallEnableSetForcedColor += (enable ? 1 : -1);
}

void G3DPainter::enableSetPointSize(bool enable)
{
    _nCallEnableSetPointSize += (enable ? 1 : -1);
}

void G3DPainter::enableSetForcedPointSize(bool enable)
{
    _nCallEnableSetForcedPointSize += (enable ? 1 : -1);
}

void G3DPainter::enableDrawPointCloud(bool enable)
{
    m_drawPointCloudEnabled = enable;
}

void G3DPainter::translate(double x, double y, double z)
{
    glTranslated(x, y, z);
}

void G3DPainter::rotate(double alpha, double x, double y, double z)
{
    glRotated(alpha, x, y, z);
}

void G3DPainter::translateThenRotateToDirection(const QVector3D &translation, const QVector3D &direction)
{
    //direction is the direction you want the object to point at
    //up- first guess
    QVector3D up;

    //If x and z are really small
    if((fabs(direction.x())< 0.00001) && (fabs(direction.z()) < 0.00001))
    {
        if(direction.y() > 0)
            up = QVector3D(0.0, 0.0, -1.0); //if direction points in +y direction
        else
            up = QVector3D(0.0, 0.0, 1.0); //if direction points in -y direction
    }
    else
    {
        up = QVector3D(0.0, 1.0, 0.0); //y-axis is the general up direction
    }

    //left
    QVector3D left = QVector3D::crossProduct(direction, up);
    left.normalize();

    //final up
    up = QVector3D::crossProduct(left, direction);
    up.normalize();

    float matrix[]={left.x(), left.y(), left.z(), 0.0f,     //LEFT
                    up.x(), up.y(), up.z(), 0.0f,                       //UP
                    direction.x(), direction.y(), direction.z(), 0.0f,  //FORWARD
                    translation.x(), translation.y(), translation.z(), 1.0f};    //TRANSLATION TO WHERE THE OBJECT SHOULD BE PLACED

    glMultMatrixf(matrix);
}

void G3DPainter::scale(double x, double y, double z)
{
    glScaled(x, y, z);
}

void G3DPainter::drawOctreeOfPoints(const OctreeInterface *octree, DrawOctreeModes modes)
{
    m_octreeCellsDraw = 0;

    if((modes == 0) || (m_gv == NULL))
        return;

    GLdouble planesCoefficients[6][4];
    m_gv->getCameraFrustumPlanesCoefficients(planesCoefficients);

    QVector3D min = octree->octreeMinCorner();
    QVector3D max = octree->octreeMaxCorner();

    double cellSize = octree->cellsSize();

    if(modes.testFlag(DrawOctree))
        drawCube(min.x(), min.y(), min.z(), max.x(), max.y(), max.z(), GL_FRONT_AND_BACK, GL_LINE);

    int s = octree->numberOfCells();

    for(int x=0; x<s; ++x)
    {
        for(int y=0; y<s; ++y)
        {
            for(int z=0; z<s; ++z)
            {
                const CT_AbstractCloudIndexT<CT_Point> *indexes = dynamic_cast<const CT_AbstractCloudIndexT<CT_Point>*>(octree->at(x, y, z));

                if(indexes != NULL)
                {
                    if(octree->isCellVisibleInFrustrum(x, y, z, planesCoefficients))
                    {
                        if(modes.testFlag(DrawOctree))
                        {
                            QVector3D p1(min.x()+(x*cellSize), min.y()+(y*cellSize), min.z()+(z*cellSize));
                            QVector3D p2(min.x()+((x+1)*cellSize), min.y()+((y+1)*cellSize), min.z()+((z+1)*cellSize));
                            drawCube(p1.x(), p1.y(), p1.z(), p2.x(), p2.y(), p2.z(), GL_FRONT_AND_BACK, GL_LINE);
                        }

                        if(modes.testFlag(DrawElements))
                        {
                            drawPointCloud(PS_REPOSITORY->globalPointCloud(), indexes, 10);
                            glColor4ub(_color.red(), _color.green(), _color.blue(), _color.alpha());
                        }

                        ++m_octreeCellsDraw;
                    }
                }
            }
        }
    }
}

void G3DPainter::drawPoint(double x, double y, double z)
{
    glBegin(GL_POINTS);

    glVertex3d(x, y, z);

    glEnd();
}

void G3DPainter::drawPoint(double *p)
{
    drawPoint(p[0], p[1], p[2]);
}

void G3DPainter::drawPoint(float *p)
{
    glBegin(GL_POINTS);

    glVertex3f(p[0], p[1], p[2]);

    glEnd();
}

void G3DPainter::drawPointCloud(const CT_AbstractPointCloud *pc,
                                const CT_AbstractCloudIndex *pci,
                                int fastestIncrement)
{
    if((pc == NULL) || (pci == NULL) || !m_drawPointCloudEnabled)
        return;

    const CT_AbstractPointCloudIndex *indexes = dynamic_cast<const CT_AbstractPointCloudIndex*>(pci);

    if(!m_gv->getOptions().useColor())
        setCurrentColor();

    size_t n = 0;
    size_t indexCount = pci->size();
    size_t increment = 1;

    if((m_fastestIncrementPoint == 0) && (fastestIncrement > 0) && drawFastest())
        increment = fastestIncrement;
    else if((m_fastestIncrementPoint != 0) && drawFastest())
        increment = m_fastestIncrementPoint;

    CT_AbstractPointCloudIndex::ConstIterator end = indexes->constEnd();

    // FAST
    if(increment > 1)
    {
        if(m_useNormalCloud
                && (m_pNormalCloud.data() != NULL))
        {
            CT_AbstractNormalCloud *nn = m_pNormalCloud->abstractNormalCloud();
            CT_AbstractPointCloudIndex::ConstIterator it = indexes->constBegin();

            if(it != end) {
                n = 0;
                glBegin(GL_LINES);
                while(n < indexCount)
                {
                    glArrayElement(it.cIndex());
                    glVertex3fv(nn->normalAt(it.cIndex()).vertex());
                    it += increment;
                    n += increment;
                }
                glEnd();
            }
        }

        CT_AbstractPointCloudIndex::ConstIterator it = indexes->constBegin();

        if(it != end) {
            n = 0;
            glBegin(GL_POINTS);
            while(n < indexCount)
            {
                glArrayElement(it.cIndex());
                it += increment;
                n += increment;
            }
            glEnd();
        }
    }
    // NORMAL
    else
    {
        if(m_useNormalCloud
                && (m_pNormalCloud.data() != NULL))
        {
            CT_AbstractNormalCloud *nn = m_pNormalCloud->abstractNormalCloud();
            CT_AbstractPointCloudIndex::ConstIterator it = indexes->constBegin();

            if(it != end) {
                n = 0;
                glBegin(GL_LINES);
                while(n < indexCount)
                {
                    glArrayElement(it.cIndex());
                    glVertex3fv(nn->normalAt(it.cIndex()).vertex());
                    ++it;
                    ++n;
                }
                glEnd();
            }
        }

        if(dynamic_cast<const CT_CloudIndexLessMemoryT<CT_Point>*>(pci) != NULL) {
            size_t fn = pci->first();
            size_t completeSize = pci->size();
            size_t fnSize = completeSize;
            size_t maxVertices = 10000000;

            if(maxVertices < completeSize)
                fnSize = completeSize / ((size_t)maxVertices);

            while(fnSize > 0) {
                glDrawArrays(GL_POINTS, fn, fnSize);
                fn += fnSize;

                if((fn + fnSize) >= completeSize)
                    fnSize = completeSize-fn;
            }

        } else {

            CT_AbstractPointCloudIndex::ConstIterator it = indexes->constBegin();

            if(it != end) {
                glBegin(GL_POINTS);
                while(it != end)
                {
                    glArrayElement(it.cIndex());
                    ++it;
                }
                glEnd();
            }
        }
    }
}

void G3DPainter::drawMesh(const CT_AbstractMeshModel *mesh)
{
    drawFaces(mesh);
}

void G3DPainter::drawFaces(const CT_AbstractMeshModel *mesh)
{
    if(mesh == NULL)
        return;

    if(!m_gv->getOptions().useColor())
        setCurrentColor();

    // W/ colors cloud
    if(m_useFColorCloud
            && (m_fColorCloud.data() != NULL))
    {
        const CT_AbstractCloudIndex *fIndex = mesh->getFaceCloudIndex();

        if(fIndex != NULL)
        {
            ((CT_AbstractMeshModel*)mesh)->beginDrawMultipleFace(*m_gv, *this);

            CT_AbstractColorCloud *cc = m_fColorCloud->abstractColorCloud();

            // W/ normals cloud
            if(m_useFNormalCloud
                    && (m_fNormalCloud.data() != NULL))
            {
                CT_AbstractNormalCloud *nn = m_fNormalCloud->abstractNormalCloud();

                size_t size = fIndex->size();
                size_t index;

                for(size_t i=0; i<size; ++i)
                {
                    fIndex->indexAt(i, index);
                    const CT_Color &color = cc->constColorAt(index);
                    glColor4ub(color.r, color.g, color.b, color.a);
                    glNormal3fv(nn->normalAt(index).vertex());

                    ((CT_AbstractMeshModel*)mesh)->drawFaceAt(i, *m_gv, *this);
                }
            }
            // W/O normals cloud
            else
            {
                size_t size = fIndex->size();

                for(size_t i=0; i<size; ++i)
                {
                    const CT_Color &color = cc->constColorAt(fIndex->indexAt(i));
                    glColor4ub(color.r, color.g, color.b, color.a);

                    ((CT_AbstractMeshModel*)mesh)->drawFaceAt(i, *m_gv, *this);
                }
            }

            ((CT_AbstractMeshModel*)mesh)->endDrawMultipleFace(*m_gv, *this);
        }
    }
    else
    {
        // W/ normals cloud
        if(m_useFNormalCloud
                && (m_fNormalCloud.data() != NULL))
        {
            const CT_AbstractCloudIndex *fIndex = mesh->getFaceCloudIndex();

            if(fIndex != NULL)
            {
                ((CT_AbstractMeshModel*)mesh)->beginDrawMultipleFace(*m_gv, *this);

                CT_AbstractNormalCloud *nn = m_fNormalCloud->abstractNormalCloud();

                size_t size = fIndex->size();

                for(size_t i=0; i<size; ++i)
                {
                    glNormal3fv(nn->normalAt(fIndex->indexAt(i)).vertex());

                    ((CT_AbstractMeshModel*)mesh)->drawFaceAt(i, *m_gv, *this);
                }

                ((CT_AbstractMeshModel*)mesh)->endDrawMultipleFace(*m_gv, *this);
            }
        }
        // W/O normals cloud
        else
        {
            ((CT_AbstractMeshModel*)mesh)->drawFaces(*m_gv, *this);
        }
    }
}

void G3DPainter::drawEdges(const CT_AbstractMeshModel *mesh)
{
    if(mesh == NULL)
        return;

    if(!m_gv->getOptions().useColor())
        setCurrentColor();

    if(m_useEColorCloud
            && (m_eColorCloud.data() != NULL))
    {
        const CT_AbstractCloudIndex *eIndex = mesh->getEdgeCloudIndex();

        if(eIndex != NULL)
        {
            ((CT_AbstractMeshModel*)mesh)->beginDrawMultipleEdge(*m_gv, *this);

            CT_AbstractColorCloud *cc = m_eColorCloud->abstractColorCloud();

            size_t size = eIndex->size();

            for(size_t i=0; i<size; ++i)
            {
                const CT_Color &color = cc->constColorAt(eIndex->indexAt(i));
                glColor4ub(color.r, color.g, color.b, color.a);

                ((CT_AbstractMeshModel*)mesh)->drawEdgeAt(i, *m_gv, *this);
            }

            ((CT_AbstractMeshModel*)mesh)->endDrawMultipleEdge(*m_gv, *this);
        }
    }
    else
    {
        ((CT_AbstractMeshModel*)mesh)->drawEdges(*m_gv, *this);
    }
}

void G3DPainter::drawPoints(const CT_AbstractMeshModel *mesh, int fastestIncrement)
{
    drawPointCloud(PS_REPOSITORY->globalPointCloud(), mesh->getPointCloudIndex(), fastestIncrement);
}

void G3DPainter::beginMultiplePoints()
{
   glBegin(GL_POINTS);
}

void G3DPainter::addPoint(float *p)
{
    glVertex3fv(p);
}

void G3DPainter::endMultiplePoints()
{
    glEnd();
}

void G3DPainter::drawRectXY(const QRectF &rectangle, double z)
{
    glBegin(GL_LINE_LOOP);

        glVertex3d(rectangle.left(), rectangle.top(), z);
        glVertex3d(rectangle.right(), rectangle.top(), z);
        glVertex3d(rectangle.right(), rectangle.bottom(), z);
        glVertex3d(rectangle.left(), rectangle.bottom(), z);

    glEnd();
}

void G3DPainter::fillRectXY(const QRectF &rectangle, double z)
{
    glBegin(GL_QUADS);

        glVertex3d(rectangle.left(), rectangle.top(), z);
        glVertex3d(rectangle.right(), rectangle.top(), z);
        glVertex3d(rectangle.right(), rectangle.bottom(), z);
        glVertex3d(rectangle.left(), rectangle.bottom(), z);

    glEnd();
}

void G3DPainter::drawRectXZ(const QRectF &rectangle, double y)
{
    glBegin(GL_LINE_LOOP);

        glVertex3d(rectangle.left(), y, rectangle.top());
        glVertex3d(rectangle.right(), y, rectangle.top());
        glVertex3d(rectangle.right(), y, rectangle.bottom());
        glVertex3d(rectangle.left(), y, rectangle.bottom());

    glEnd();
}

void G3DPainter::fillRectXZ(const QRectF &rectangle, double y)
{
    glBegin(GL_QUADS);

    glVertex3d(rectangle.left(), y, rectangle.top());
    glVertex3d(rectangle.right(), y, rectangle.top());
    glVertex3d(rectangle.right(), y, rectangle.bottom());
    glVertex3d(rectangle.left(), y, rectangle.bottom());

    glEnd();
}

void G3DPainter::drawRectYZ(const QRectF &rectangle, double x)
{
    glBegin(GL_LINE_LOOP);

        glVertex3d(x, rectangle.left(), rectangle.top());
        glVertex3d(x, rectangle.right(), rectangle.top());
        glVertex3d(x, rectangle.right(), rectangle.bottom());
        glVertex3d(x, rectangle.left(), rectangle.bottom());

    glEnd();
}

void G3DPainter::fillRectYZ(const QRectF &rectangle, double x)
{
    glBegin(GL_QUADS);

    glVertex3d(x, rectangle.left(), rectangle.top());
    glVertex3d(x, rectangle.right(), rectangle.top());
    glVertex3d(x, rectangle.right(), rectangle.bottom());
    glVertex3d(x, rectangle.left(), rectangle.bottom());
    glEnd();
}

void G3DPainter::beginPolygon()
{
    glBegin(GL_LINE_STRIP);
}

void G3DPainter::addPointToPolygon(float *p)
{
    glVertex3fv(p);
}

void G3DPainter::endPolygon()
{
    glEnd();
}

void G3DPainter::beginDrawMultipleLine()
{
    glBegin(GL_LINES);
    m_drawMultipleLine = true;
}

void G3DPainter::drawLine(double x1, double y1, double z1, double x2, double y2, double z2)
{
    if(m_drawMultipleLine)
    {
        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y2, z2);
    }
    else
    {
        glBegin(GL_LINES);

        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y2, z2);

        glEnd();
    }
}

void G3DPainter::drawLine(const float *p1, const float *p2)
{
    if(m_drawMultipleLine)
    {
        glVertex3fv(p1);
        glVertex3fv(p2);
    }
    else
    {
        glVertex3fv(p1);
        glVertex3fv(p2);
    }
}

void G3DPainter::endDrawMultipleLine()
{
    glEnd();
    m_drawMultipleLine = false;
}

void G3DPainter::drawCircle(double x, double y, double z, double radius)
{
    QVector< QPair<double, double> > *cosSinA = &G3DPainter::VECTOR_CIRCLE_NORMAL;

    if(_drawFastest)
    {
        cosSinA = &G3DPainter::VECTOR_CIRCLE_FASTEST;
    }

    int size = cosSinA->size();

    glBegin(GL_LINE_LOOP);

    float inc = M_PI_MULT_2/((double)size);
    float a = -M_PI;

    for(int i=0; i<size; ++i)
    {
        const QPair<double, double> &pair = (*cosSinA)[i];

        glVertex3d((pair.first*radius) + x, (pair.second*radius) + y, z);
        a += inc;
    }

    glEnd();
}

void G3DPainter::drawCircle3D(const QVector3D &center, const QVector3D &direction, double radius)
{
    QVector3D u(0,0,0);

    if(fabs(direction.x()) >= fabs(direction.y()))
    {
        double factor = 1.0/sqrt(direction.x()*direction.x()+direction.z()*direction.z());
        u.setX(-direction.z()*factor);
        u.setZ(direction.x()*factor);
    }
    else
    {
        double factor = 1.0/sqrt(direction.y()*direction.y()+direction.z()*direction.z());
        u.setY(direction.z()*factor);
        u.setZ(-direction.y()*factor);
    }

    QVector3D v = QVector3D::crossProduct(direction, u);
    v.normalize();

    QVector< QPair<double, double> > *cosSinA = &G3DPainter::VECTOR_CIRCLE_NORMAL;

    if(_drawFastest)
    {
        cosSinA = &G3DPainter::VECTOR_CIRCLE_FASTEST;
    }

    int size = cosSinA->size();

    glBegin(GL_LINE_LOOP);

    for(int i=0; i<size; ++i)
    {
        const QPair<double, double> &pair = (*cosSinA)[i];

        glVertex3d(center.x() + (radius*pair.first)*u.x() + (radius*pair.second)*v.x(),
                   center.y() + (radius*pair.first)*u.y() + (radius*pair.second)*v.y(),
                   center.z() + (radius*pair.first)*u.z() + (radius*pair.second)*v.z());
    }

    glEnd();
}

void G3DPainter::drawCylinder(double x, double y, double z, double radius, double height)
{
    save();

    translate(x, y, z);

    gluCylinder(_quadric, radius, radius, height, 20, 1);

    restore();
}

void G3DPainter::drawCylinder3D(const QVector3D &center, const QVector3D &direction, double radius, double height)
{
    save();

    float delta = - height/2.0;
    translateThenRotateToDirection(QVector3D(center.x() + delta*direction.x(), center.y() + delta*direction.y(), center.z() + delta*direction.z()), direction);

    gluCylinder(_quadric, radius, radius, height, 20, 1);

    restore();
}

void G3DPainter::drawEllipse(double x, double y, double z, double radiusA, double radiusB)
{
    QVector< QPair<double, double> > *cosSinA = &G3DPainter::VECTOR_CIRCLE_NORMAL;

    if(_drawFastest)
    {
        cosSinA = &G3DPainter::VECTOR_CIRCLE_FASTEST;
    }

    int size = cosSinA->size();

    glBegin(GL_LINE_LOOP);

    for(int i=0; i<size; ++i)
    {
        const QPair<double, double> &pair = (*cosSinA)[i];

        glVertex3d((pair.first*radiusA)+x,
                   (pair.second*radiusB)+y,
                   z);
    }

    glEnd();
}

void G3DPainter::beginDrawMultipleTriangle()
{
    glBegin( GL_TRIANGLES );
    m_drawMultipleTriangle = true;
}

void G3DPainter::drawTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3)
{
    if(m_drawMultipleTriangle)
    {
        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y2, z2);
        glVertex3d(x3, y3, z3);
    }
    else
    {
        glBegin( GL_TRIANGLES );
        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y2, z2);
        glVertex3d(x3, y3, z3);
        glEnd();
    }
}

void G3DPainter::drawTriangle(const float *p1, const float *p2, const float *p3)
{
    if(m_drawMultipleTriangle)
    {
        glVertex3fv(p1);
        glVertex3fv(p2);
        glVertex3fv(p3);
    }
    else
    {
        glBegin( GL_TRIANGLES );
        glVertex3fv(p1);
        glVertex3fv(p2);
        glVertex3fv(p3);
        glEnd();
    }
}

void G3DPainter::endDrawMultipleTriangle()
{
    glEnd();
    m_drawMultipleTriangle = false;
}

void G3DPainter::drawCube(double x1, double y1, double z1, double x2, double y2, double z2)
{
    glBegin(GL_QUADS);
        // Bottom
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y1, z1);
        glVertex3f(x2, y2, z1);
        glVertex3f(x1, y2, z1);

        // Top
        glVertex3f(x1, y1, z2);
        glVertex3f(x2, y1, z2);
        glVertex3f(x2, y2, z2);
        glVertex3f(x1, y2, z2);

        // Left
        glVertex3f(x1, y1, z1);
        glVertex3f(x1, y2, z1);
        glVertex3f(x1, y2, z2);
        glVertex3f(x1, y1, z2);

        // Right
        glVertex3f(x2, y1, z1);
        glVertex3f(x2, y2, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x2, y1, z2);

        // Front
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y1, z1);
        glVertex3f(x2, y1, z2);
        glVertex3f(x1, y1, z2);

        // Back
        glVertex3f(x1, y2, z1);
        glVertex3f(x2, y2, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x1, y2, z2);
    glEnd();
}

void G3DPainter::drawPyramid(double topX, double topY, double topZ, double base1X, double base1Y, double base1Z, double base2X, double base2Y, double base2Z, double base3X, double base3Y, double base3Z, double base4X, double base4Y, double base4Z)
{
    glBegin( GL_TRIANGLE_FAN );
        glVertex3f(topX, topY, topZ);

        glVertex3f(base1X, base1Y, base1Z);
        glVertex3f(base2X, base2Y, base2Z);
        glVertex3f(base3X, base3Y, base3Z);
        glVertex3f(base4X, base4Y, base4Z);

        glVertex3f(base1X, base1Y, base1Z);
    glEnd();

    glBegin( GL_QUADS );
        glVertex3f(base1X, base1Y, base1Z);
        glVertex3f(base2X, base2Y, base2Z);
        glVertex3f(base3X, base3Y, base3Z);
        glVertex3f(base4X, base4Y, base4Z);
    glEnd();
}

void G3DPainter::drawPartOfSphere(double centerX, double centerY, double centerZ, double radius, double initTheta, double endTheta, double initPhi, double endPhi, bool radians)
{
    if ( !radians )
    {
        initTheta *= M_PI/180.0;
        endTheta *= M_PI/180.0;
        initPhi *= M_PI/180.0;
        endPhi *= M_PI/180.0;
    }

    float nbSteps = 60;
    float stepTheta = fabs(endTheta-initTheta) / nbSteps;
    float stepPhi = fabs(endPhi-initPhi) / nbSteps;

    float cosPhi, sinPhi, cosTheta, sinTheta;

    for ( float currentTheta = initTheta ; currentTheta <= endTheta ;  currentTheta += stepTheta )
    {
        glBegin( GL_LINE_STRIP );
        for ( float currentPhi = initPhi ; currentPhi <= endPhi ; currentPhi += stepPhi )
        {
                sinPhi = sin (currentPhi);
                cosPhi = cos (currentPhi);
                sinTheta = sin (currentTheta);
                cosTheta = cos (currentTheta);

                glVertex3f( radius*sinPhi*cosTheta + centerX,
                            radius*sinPhi*sinTheta + centerY,
                            radius*cosPhi + centerZ);
        }
        glEnd();
    }

    for ( float currentPhi = initPhi ; currentPhi <= endPhi ; currentPhi += stepPhi )
    {
        glBegin( GL_LINE_STRIP );
        for ( float currentTheta = initTheta ; currentTheta <= endTheta ;  currentTheta += stepTheta )
        {
                sinPhi = sin (currentPhi);
                cosPhi = cos (currentPhi);
                sinTheta = sin (currentTheta);
                cosTheta = cos (currentTheta);

                glVertex3f( radius*sinPhi*cosTheta + centerX,
                            radius*sinPhi*sinTheta + centerY,
                            radius*cosPhi + centerZ);
        }
        glEnd();
    }
}

void G3DPainter::fillQuadFace(float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float x4, float y4, float z4)
{
     glBegin(GL_QUADS);

         glVertex3d(x1, y1, z1 );
         glVertex3d(x2, y2, z2 );
         glVertex3d(x3, y3, z3 );
         glVertex3d(x4, y4, z4 );

     glEnd();
}

void G3DPainter::fillQuadFace(float x1, float y1, float z1, int r1, int g1, int b1,
                               float x2, float y2, float z2, int r2, int g2, int b2,
                               float x3, float y3, float z3, int r3, int g3, int b3,
                               float x4, float y4, float z4, int r4, int g4, int b4)
{
     glBegin(GL_QUADS);

         glColor3ub(r1, g1, b1);
         glVertex3d(x1, y1, z1 );

         glColor3ub(r2, g2, b2);
         glVertex3d(x2, y2, z2 );

         glColor3ub(r3, g3, b3);
         glVertex3d(x3, y3, z3 );

         glColor3ub(r4, g4, b4);
         glVertex3d(x4, y4, z4 );

     glEnd();
}

void G3DPainter::drawQuadFace(float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float x4, float y4, float z4)
{
     glBegin(GL_LINE_STRIP);

         glVertex3d(x1, y1, z1 );
         glVertex3d(x2, y2, z2 );
         glVertex3d(x3, y3, z3 );
         glVertex3d(x4, y4, z4 );

     glEnd();
}

void G3DPainter::drawQuadFace(float x1, float y1, float z1, int r1, int g1, int b1,
                               float x2, float y2, float z2, int r2, int g2, int b2,
                               float x3, float y3, float z3, int r3, int g3, int b3,
                               float x4, float y4, float z4, int r4, int g4, int b4)
{
     glBegin(GL_LINE_STRIP);

         glColor3ub(r1, g1, b1);
         glVertex3d(x1, y1, z1 );

         glColor3ub(r2, g2, b2);
         glVertex3d(x2, y2, z2 );

         glColor3ub(r3, g3, b3);
         glVertex3d(x3, y3, z3 );

         glColor3ub(r4, g4, b4);
         glVertex3d(x4, y4, z4 );

     glEnd();
}

//////////// PRIVATE ///////////

void G3DPainter::setCurrentColor()
{
    glColor4ub(_color.red(), _color.green(), _color.blue(), _color.alpha());
}

void G3DPainter::setCurrentForcedColor()
{
    glColor4ub(_forcedColor.red(), _forcedColor.green(), _forcedColor.blue(), _forcedColor.alpha());
}

QVector< QPair<double, double> > G3DPainter::staticInitCircleVector(int size)
{
    QVector< QPair<double, double> > vector(size);

    float inc = M_PI_MULT_2/((double)size);
    float a = -M_PI;

    for(int i=0; i<size; ++i)
    {
        QPair<double, double> &pair = vector[i];
        pair.first = cos(a);
        pair.second = sin(a);

        a += inc;
    }

    return vector;
}
