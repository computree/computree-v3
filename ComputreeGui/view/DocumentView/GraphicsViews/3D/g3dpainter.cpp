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

#include "view/DocumentView/GraphicsViews/3D/g3dgraphicsview.h"

#include "tools/graphicsview/dm_elementinfomanager.h"

#include "ct_global/ct_context.h"
#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_normalcloud/registered/ct_standardnormalcloudregistered.h"
#include "ct_normalcloud/abstract/ct_abstractnormalcloud.h"
#include "ct_itemdrawable/ct_meshmodel.h"
#include "ct_cloudindex/ct_cloudindexlessmemoryt.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

#include <QQuaternion>

#define M_PI_MULT_2 6.28318530717958647692

QVector< QPair<double, double> > G3DPainter::VECTOR_CIRCLE_FASTEST = G3DPainter::staticInitCircleVector(G3DPainter::VECTOR_CIRCLE_FASTEST_SIZE);
QVector< QPair<double, double> > G3DPainter::VECTOR_CIRCLE_NORMAL = G3DPainter::staticInitCircleVector(G3DPainter::VECTOR_CIRCLE_NORMAL_SIZE);

G3DPainter::G3DPainterCylinder G3DPainter::CYLINDER_FASTEST = G3DPainter::staticInitCylinder(G3DPainter::staticInitCircleVector(G3DPainter::VECTOR_CIRCLE_FASTEST_SIZE));
G3DPainter::G3DPainterCylinder G3DPainter::CYLINDER_NORMAL = G3DPainter::staticInitCylinder(G3DPainter::staticInitCircleVector(G3DPainter::VECTOR_CIRCLE_NORMAL_SIZE));

G3DPainter::G3DPainter()
{
    m_gv = NULL;

    _color = Qt::white;
    _forcedColor = Qt::white;
    _defaultPointSize = 1.0;
    _drawFastest = false;

    m_shaderProgPoint = NULL;
    m_shaderProgPointError = false;
    m_shaderProgPointSet = false;
    m_ShaderPoint = NULL;
    m_shaderPointError = false;
    m_shaderLocInitialized = false;
    m_shaderLocCsIndex = -1;
    m_shaderLocInfo = -1;
    m_shaderLocCsMatrix = -1;
    m_shaderLocSelectionColor = -1;
    m_shaderLocCheckSelected = -1;

    m_shaderDeProg = NULL;
    m_shaderDeProgError = false;
    m_shaderDe = NULL;
    m_shaderDeError = false;
    m_shaderDeLocInitialized = false;
    m_shaderDeLocPMatrix1 = -1;
    m_shaderDeLocPMatrix2 = -1;
    m_shaderDeLocPMatrix3 = -1;
    m_shaderDeLocPMatrix4 = -1;

    m_csMatrix.resize(64, Eigen::Matrix4f::Identity()); // 64 is the maximum number of matrix in the shader

    beginNewDraw();
}

G3DPainter::~G3DPainter()
{
    delete m_shaderProgPoint;
    delete m_ShaderPoint;
}

void G3DPainter::setGraphicsView(const G3DGraphicsView *gv)
{
    m_gv = (G3DGraphicsView*)gv;
}

G3DGraphicsView* G3DPainter::graphicsView() const
{
    return m_gv;
}

void G3DPainter::initializeGl()
{
    QT_GL_INIT_FUNCTIONS();

    m_maxVertexUniformVec4 = 0;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &m_maxVertexUniformVec4);

    m_maxMatrix = (m_maxVertexUniformVec4/4)-4;

    GLint majV = 0;
    GLint minV = 0;

    GLenum err;
    if((err = glGetError()) != GL_NO_ERROR)
        glGetIntegerv(GL_MAJOR_VERSION, &majV);
    else
        majV = 1;

    m_openglVersion = majV + ((double)minV)/10.0;
}

void G3DPainter::setPointFastestIncrement(size_t inc)
{
    m_fastestIncrementPoint = inc;
}

size_t G3DPainter::pointFastestIncrement() const
{
    return m_fastestIncrementPoint;
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
    m_polygonMode = 0;

    _nCallEnableSetColor = 0;
    _nCallEnableSetForcedColor = 0;

    _nCallEnableSetPointSize = 0;
    _nCallEnableSetForcedPointSize = 0;

    _nCallEnablePushMatrix = 0;

    m_usePColorCloud = true;
    m_usePNormalCloud = true;
    m_useFColorCloud = true;
    m_useFNormalCloud = true;
    m_useEColorCloud = true;

    m_fastestIncrementPoint = 0;

    m_octreeCellsDraw = 0;

    m_shaderProgPointSet = false;
    m_bindShaderPointOK = false;

    m_bindShaderDeOK = false;

    m_firstPolygonPointValid = false;

    // DRAW ALL elements
    m_drawOnly = DRAW_ALL;

    // not current "glBegin" called
    m_currentGlBeginType = GL_END_CALLED;

    m_matrixStack.clear();

    // Model/View Matrix of the camera
    if((m_gv != NULL) && (m_gv->camera() != NULL)) {
        GLdouble modelViewMatrix[16];
        m_gv->camera()->modelViewMatrix(modelViewMatrix);

        // convert to eigen matrix4d
        m_modelViewMatrix4d << modelViewMatrix[0], modelViewMatrix[4], modelViewMatrix[8], modelViewMatrix[12],
                               modelViewMatrix[1], modelViewMatrix[5], modelViewMatrix[9], modelViewMatrix[13],
                               modelViewMatrix[2], modelViewMatrix[6], modelViewMatrix[10], modelViewMatrix[14],
                               modelViewMatrix[3], modelViewMatrix[7], modelViewMatrix[11], modelViewMatrix[15];

    }
}

void G3DPainter::endNewDraw()
{
    stopDrawMultiple();
}

void G3DPainter::setDrawOnly(G3DPainter::MultiDrawableType type)
{
    // the new type to draw
    m_drawOnly = type;
}

void G3DPainter::save()
{
    pushMatrix();

    stopDrawMultiple();
    glGetIntegerv(GL_POLYGON_MODE, &m_savePolygonMode);
}

void G3DPainter::restore()
{
    popMatrix();

    setPolygonMode(GL_FRONT_AND_BACK, m_savePolygonMode);
}

void G3DPainter::startRestoreIdentityMatrix(GLdouble *matrix)
{
    stopDrawMultiple();

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
    stopDrawMultiple();

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
        m_matrixStack.push(m_modelViewMatrix4d);
}

void G3DPainter::multMatrix(const Eigen::Matrix4d &matrix)
{
    if(_nCallEnablePushMatrix == 0)
        m_modelViewMatrix4d = m_modelViewMatrix4d * matrix;
}

void G3DPainter::popMatrix()
{
    if((_nCallEnablePushMatrix == 0) && (m_matrixStack.size() > 0))
        m_modelViewMatrix4d = m_matrixStack.pop();
}

void G3DPainter::setPointSize(float size)
{
    if(_nCallEnableSetPointSize == 0)
    {
        stopDrawMultiple();
        glPointSize(size);
    }
}

void G3DPainter::setDefaultPointSize(float size)
{
    if(_nCallEnableSetPointSize == 0)
        _defaultPointSize = size;
}

void G3DPainter::restoreDefaultPointSize()
{
    if(_nCallEnableSetPointSize == 0)
    {
        stopDrawMultiple();
        glPointSize(_defaultPointSize);
    }
}

void G3DPainter::restoreDefaultPen()
{
    stopDrawMultiple();

    glLineWidth(1);
    glDisable(GL_LINE_STIPPLE);

    _color = Qt::white;
    setCurrentColor();
}

void G3DPainter::setPen(const QPen &pen)
{
    if(_nCallEnableSetColor == 0)
    {
        stopDrawMultiple();

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
    m_usePColorCloud = enable;
}

void G3DPainter::setUseNormalCloudForPoints(bool enable)
{
    m_usePNormalCloud = enable;
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

void G3DPainter::translate(const double &x, const double &y, const double &z)
{
    Eigen::Matrix4d t = Eigen::Matrix4d::Identity();
    t(0,3) = x;
    t(1,3) = y;
    t(2,3) = z;

    multMatrix(t);
}

void G3DPainter::rotate(const double &alpha, const double &x, const double &y, const double &z)
{
    Eigen::Vector3d v(x, y ,z);
    v.normalize();

    Eigen::Affine3d m = Eigen::Affine3d::Identity();
    m.rotate(Eigen::AngleAxisd(alpha, v));

    multMatrix(m.matrix());
}

void G3DPainter::translateThenRotateToDirection(const Eigen::Vector3d &translation, const Eigen::Vector3d &direction)
{
    //direction is the direction you want the object to point at
    //up- first guess
    Eigen::Vector3d up;

    //If x and z are really small
    if((fabs(direction.x())< 0.00001) && (fabs(direction.z()) < 0.00001))
    {
        if(direction.y() > 0)
            up = Eigen::Vector3d(0.0, 0.0, -1.0); //if direction points in +y direction
        else
            up = Eigen::Vector3d(0.0, 0.0, 1.0); //if direction points in -y direction
    }
    else
    {
        up = Eigen::Vector3d(0.0, 1.0, 0.0); //y-axis is the general up direction
    }

    Eigen::Vector3d dn = direction.normalized();

    //left
    Eigen::Vector3d left = dn.cross(up);
    left.normalize();

    //final up
    up = left.cross(dn);
    up.normalize();

    Eigen::Matrix4d res;
    res <<  left.x(), up.x(), dn.x(), translation.x(),
            left.y(), up.y(), dn.y(), translation.y(),
            left.z(), up.z(), dn.z(), translation.z(),
            0.0     , 0.0   , 0.0           , 1.0;

    multMatrix(res);
}

void G3DPainter::scale(const double &x, const double &y, const double &z)
{
    Eigen::Matrix4d sc = Eigen::Matrix4d::Identity();
    sc(0,0) = x;
    sc(1,1) = y;
    sc(2,2) = z;

    multMatrix(sc);
}

void G3DPainter::drawPoint(const double &x, const double &y, const double &z)
{
    // if we can points
    if(canDraw(GL_BEGIN_POINT))
    {
        // Call this method to call "glBegin(GL_POINTS)" if it was not already called
        startDrawMultiple(GL_BEGIN_POINT);

        Eigen::Vector4d v = m_modelViewMatrix4d * Eigen::Vector4d(x, y, z, 1);

        glVertex3dv(v.data());
    }
}

void G3DPainter::drawPoint(double *p)
{
    drawPoint(p[0], p[1], p[2]);
}

void G3DPainter::drawPoint(const size_t &globalIndex)
{
    if(canDraw(GL_BEGIN_POINT_FROM_PC))
    {
        // Call this method to call "glBegin(GL_POINTS)" if it was not already called
        startDrawMultiple(GL_BEGIN_POINT_FROM_PC);

        DM_ElementInfoManager *infos = m_gv->pointsInformationManager();

        if(!infos->inlineIsInvisible(globalIndex))
            glArrayElement(globalIndex);
    }
}

void G3DPainter::drawPoints(const CT_AbstractMeshModel *mesh)
{
    drawPointCloud(mesh->getPointCloudIndex());
}

void G3DPainter::drawPointCloud(const CT_AbstractCloudIndex *pci)
{
    if(pci == NULL)
        return;

    size_t indexCount = pci->size();

    if(indexCount == 0)
        return;

    if(canDraw(GL_BEGIN_POINT_FROM_PC) || canDraw(GL_BEGIN_LINE_FROM_PC))
    {
        if(!m_gv->getOptions().useColor())
            setCurrentColor();

        size_t n = 0;
        size_t increment = 1;
        size_t globalIndex;

        if((m_fastestIncrementPoint != 0) && drawFastest())
            increment = m_fastestIncrementPoint;

        QSharedPointer<CT_StandardNormalCloudRegistered> normalCloud = m_gv->normalCloudOf(GraphicsViewInterface::NPointCloud);

        DM_ElementInfoManager *infos = m_gv->pointsInformationManager();

        // FAST
        if(increment > 1)
        {
            if(m_usePNormalCloud
                    && (normalCloud.data() != NULL)
                    && canDraw(GL_BEGIN_LINE_FROM_PC))
            {
                startDrawMultiple(GL_BEGIN_LINE_FROM_PC);

                CT_AbstractNormalCloud *nn = normalCloud->abstractNormalCloud();

                n = 0;
                while(n < indexCount)
                {
                    pci->indexAt(n, globalIndex);

                    if(!infos->inlineIsInvisible(globalIndex)) {
                        glArrayElement(globalIndex);
                        glVertex3fv(nn->normalAt(globalIndex).vertex());
                    }

                    n += increment;
                }
            }

            if(canDraw(GL_BEGIN_POINT_FROM_PC))
            {
                startDrawMultiple(GL_BEGIN_POINT_FROM_PC);

                n = 0;
                while(n < indexCount)
                {
                    pci->indexAt(n, globalIndex);

                    if(!infos->inlineIsInvisible(globalIndex))
                        glArrayElement(globalIndex);

                    n += increment;
                }
            }
        }
        // NORMAL
        else
        {
            if(m_usePNormalCloud
                    && (normalCloud.data() != NULL)
                    && canDraw(GL_BEGIN_LINE_FROM_PC))
            {
                startDrawMultiple(GL_BEGIN_LINE_FROM_PC);

                CT_AbstractNormalCloud *nn = normalCloud->abstractNormalCloud();

                n = 0;
                while(n < indexCount)
                {
                    pci->indexAt(n, globalIndex);

                    if(!infos->inlineIsInvisible(globalIndex)) {
                        glArrayElement(globalIndex);
                        glVertex3fv(nn->normalAt(globalIndex).vertex());
                    }

                    ++n;
                }
            }

            /*if(dynamic_cast<const CT_PointCloudIndexLessMemory*>(pci) != NULL) {
                startDrawMultiple(GL_BEGIN_POINT_FROM_PC);

                size_t fn = pci->first();
                size_t fnSize = pci->size();
                size_t completeSize = fn + fnSize;
                size_t maxVertices = 10000000;

                if(maxVertices < fnSize)
                    fnSize = maxVertices;

                while(fnSize > 0) {
                    glDrawArrays(GL_POINTS, fn, fnSize);
                    fn += fnSize;

                    if((fn + fnSize) >= completeSize)
                        fnSize = completeSize-fn;
                }

            } else {*/

                if(canDraw(GL_BEGIN_POINT_FROM_PC))
                {
                    startDrawMultiple(GL_BEGIN_POINT_FROM_PC);

                    n = 0;
                    while(n < indexCount)
                    {
                        pci->indexAt(n, globalIndex);

                        if(!infos->inlineIsInvisible(globalIndex))
                            glArrayElement(globalIndex);

                        ++n;
                    }
                }
            //}
        }
    }
}

void G3DPainter::drawOctreeOfPoints(const OctreeInterface *octree, DrawOctreeModes modes)
{
    m_octreeCellsDraw = 0;

    if((modes == 0) || (m_gv == NULL))
        return;

    if((modes.testFlag(DrawOctree) && canDraw(GL_BEGIN_QUAD))
            || (modes.testFlag(DrawElements) && canDraw(GL_BEGIN_POINT_FROM_PC)))
    {
        GLdouble planesCoefficients[6][4];
        m_gv->getCameraFrustumPlanesCoefficients(planesCoefficients);

        Eigen::Vector3d min = octree->octreeMinCorner();
        Eigen::Vector3d max = octree->octreeMaxCorner();

        double cellSize = octree->cellsSize();

        if(modes.testFlag(DrawOctree))
            drawCube(min(0), min(1), min(2), max(0), max(1), max(2), GL_FRONT_AND_BACK, GL_LINE);

        QList<CT_AbstractCloudIndex*> indexesToDraw;

        int s = octree->numberOfCells();

        for(int x=0; x<s; ++x)
        {
            for(int y=0; y<s; ++y)
            {
                for(int z=0; z<s; ++z)
                {
                    const CT_AbstractCloudIndex *indexes = octree->at(x, y, z);

                    if(indexes != NULL)
                    {
                        if(octree->isCellVisibleInFrustrum(x, y, z, planesCoefficients))
                        {
                            if(modes.testFlag(DrawOctree))
                            {
                                Eigen::Vector3d p1(min(0)+(x*cellSize), min(1)+(y*cellSize), min(2)+(z*cellSize));
                                Eigen::Vector3d p2(min(0)+((x+1)*cellSize), min(1)+((y+1)*cellSize), min(2)+((z+1)*cellSize));
                                drawCube(p1(0), p1(1), p1(2), p2(0), p2(1), p2(2), GL_FRONT_AND_BACK, GL_LINE);
                            }

                            if(modes.testFlag(DrawElements))
                                indexesToDraw.append((CT_AbstractCloudIndex*)indexes);

                            ++m_octreeCellsDraw;
                        }
                    }
                }
            }
        }

        QListIterator<CT_AbstractCloudIndex*> it(indexesToDraw);

        while(it.hasNext())
        {
            drawPointCloud(it.next());
            glColor4ub(_color.red(), _color.green(), _color.blue(), _color.alpha());
        }
    }
}

void G3DPainter::drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2)
{
    drawCube(x1, y1, z1, x2, y2, z2, GL_FRONT_AND_BACK, GL_LINE);
}

void G3DPainter::drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2, GLenum faces, GLenum mode)
{
    if(canDraw(GL_BEGIN_QUAD))
    {
        setPolygonMode(faces, mode);
        startDrawMultiple(GL_BEGIN_QUAD);

        // Bottom
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y1, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y2, z1, 1)).data());

        // Top
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z2, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y1, z2, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z2, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y2, z2, 1)).data());

        // Left
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y2, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y2, z2, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z2, 1)).data());

        // Right
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y1, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z2, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y1, z2, 1)).data());

        // Front
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y1, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y1, z2, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z2, 1)).data());

        // Back
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y2, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z2, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y2, z2, 1)).data());
    }
}

void G3DPainter::drawQuadFace(const double &x1, const double &y1, const double &z1,
                              const double &x2, const double &y2, const double &z2,
                              const double &x3, const double &y3, const double &z3,
                              const double &x4, const double &y4, const double &z4)
{
    if(canDraw(GL_BEGIN_QUAD))
    {
        setPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        startDrawMultiple(GL_BEGIN_QUAD);

        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z2, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x3, y3, z3, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x4, y4, z4, 1)).data());
    }
}


void G3DPainter::fillQuadFace(const double &x1, const double &y1, const double &z1,
                               const double &x2, const double &y2, const double &z2,
                               const double &x3, const double &y3, const double &z3,
                               const double &x4, const double &y4, const double &z4)
{
    if(canDraw(GL_BEGIN_QUAD))
    {
        setPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        startDrawMultiple(GL_BEGIN_QUAD);

        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z2, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x3, y3, z3, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x4, y4, z4, 1)).data());
    }
}

void G3DPainter::drawQuadFace(const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                              const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                              const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                              const double &x4, const double &y4, const double &z4, int r4, int g4, int b4)
{
    if(canDraw(GL_BEGIN_QUAD))
    {
        setPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        startDrawMultiple(GL_BEGIN_QUAD);

        glColor3ub(r1, g1, b1);
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z1, 1)).data());
        glColor3ub(r2, g2, b2);
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z2, 1)).data());
        glColor3ub(r3, g3, b3);
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x3, y3, z3, 1)).data());
        glColor3ub(r4, g4, b4);
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x4, y4, z4, 1)).data());
    }
}

void G3DPainter::fillQuadFace(const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                              const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                              const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                              const double &x4, const double &y4, const double &z4, int r4, int g4, int b4)
{
    if(canDraw(GL_BEGIN_QUAD))
    {
        setPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        startDrawMultiple(GL_BEGIN_QUAD);

        glColor3ub(r1, g1, b1);
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z1, 1)).data());
        glColor3ub(r2, g2, b2);
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z2, 1)).data());
        glColor3ub(r3, g3, b3);
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x3, y3, z3, 1)).data());
        glColor3ub(r4, g4, b4);
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x4, y4, z4, 1)).data());
     }
}

void G3DPainter::drawRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z)
{
    if(canDraw(GL_BEGIN_QUAD))
    {
        setPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        startDrawMultiple(GL_BEGIN_QUAD);

        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(topLeft(0), topLeft(1), z, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(bottomRight(0), topLeft(1), z, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(bottomRight(0), bottomRight(1), z, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(topLeft(0), bottomRight(1), z, 1)).data());
    }
}

void G3DPainter::fillRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z)
{
    if(canDraw(GL_BEGIN_QUAD))
    {
        setPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        startDrawMultiple(GL_BEGIN_QUAD);

        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(topLeft(0), topLeft(1), z, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(bottomRight(0), topLeft(1), z, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(bottomRight(0), bottomRight(1), z, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(topLeft(0), bottomRight(1), z, 1)).data());
    }
}

void G3DPainter::drawRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y)
{
    if(canDraw(GL_BEGIN_QUAD))
    {
        setPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        startDrawMultiple(GL_BEGIN_QUAD);

        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(topLeft(0), y, topLeft(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(bottomRight(0), y, topLeft(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(bottomRight(0), y, bottomRight(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(topLeft(0), y, bottomRight(1), 1)).data());
    }
}

void G3DPainter::fillRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y)
{
    if(canDraw(GL_BEGIN_QUAD))
    {
        setPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        startDrawMultiple(GL_BEGIN_QUAD);

        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(topLeft(0), y, topLeft(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(bottomRight(0), y, topLeft(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(bottomRight(0), y, bottomRight(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(topLeft(0), y, bottomRight(1), 1)).data());
    }
}

void G3DPainter::drawRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x)
{
    if(canDraw(GL_BEGIN_QUAD))
    {
        setPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        startDrawMultiple(GL_BEGIN_QUAD);

        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x, topLeft(0), topLeft(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x, bottomRight(0), topLeft(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x, bottomRight(0), bottomRight(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x, topLeft(0), bottomRight(1), 1)).data());
    }
}

void G3DPainter::fillRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x)
{
    if(canDraw(GL_BEGIN_QUAD))
    {
        setPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        startDrawMultiple(GL_BEGIN_QUAD);

        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x, topLeft(0), topLeft(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x, bottomRight(0), topLeft(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x, bottomRight(0), bottomRight(1), 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x, topLeft(0), bottomRight(1), 1)).data());
    }
}

void G3DPainter::drawLine(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2)
{
    if(canDraw(GL_BEGIN_LINE))
    {
        startDrawMultiple(GL_BEGIN_LINE);

        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z2, 1)).data());
    }
}

void G3DPainter::drawLine(const size_t &p1GlobalIndex, const size_t &p2GlobalIndex)
{
    if(canDraw(GL_BEGIN_LINE_FROM_PC))
    {
        startDrawMultiple(GL_BEGIN_LINE_FROM_PC);

        glArrayElement(p1GlobalIndex);
        glArrayElement(p2GlobalIndex);
    }
}

void G3DPainter::drawEdges(const CT_AbstractMeshModel *mesh)
{
    if(mesh == NULL)
        return;

    const CT_AbstractCloudIndex *eIndex = mesh->getEdgeCloudIndex();

    if(eIndex == NULL)
        return;

    if(canDraw(GL_BEGIN_LINE_FROM_PC))
    {
        startDrawMultiple(GL_BEGIN_LINE_FROM_PC);

        if(!m_gv->getOptions().useColor())
            setCurrentColor();

        QSharedPointer<CT_StandardColorCloudRegistered> colorCloud = m_gv->colorCloudOf(GraphicsViewInterface::CEdgeCloud);
        //QSharedPointer<CT_StandardNormalCloudRegistered> normalCloud = m_gv->normalCloudOf(GraphicsViewInterface::NEdgeCloud);
        QColor selColor = m_gv->getOptions().getSelectedColor();
        DM_ElementInfoManager *infos = m_gv->edgesInformationManager();
        size_t size = eIndex->size();
        size_t globalIndex;

        if(m_useEColorCloud
                && (colorCloud.data() != NULL))
        {
            CT_AbstractColorCloud *cc = colorCloud->abstractColorCloud();

            for(size_t i=0; i<size; ++i)
            {
                eIndex->indexAt(i, globalIndex);

                if(!infos->inlineIsInvisible(globalIndex))
                {
                    if(infos->inlineIsSelected(globalIndex)) {
                        glColor4ub(selColor.red(), selColor.green(), selColor.blue(), selColor.alpha());
                    } else {
                        const CT_Color &color = cc->constColorAt(globalIndex);
                        glColor4ub(color.r, color.g, color.b, color.a);
                    }

                    const CT_Edge &edge = m_eAccess.constEdgeAt(globalIndex);
                    glArrayElement(edge.iPointAt(0));
                    glArrayElement(edge.iPointAt(1));
                }
            }
        }
        else
        {
            for(size_t i=0; i<size; ++i)
            {
                eIndex->indexAt(i, globalIndex);

                if(!infos->inlineIsInvisible(globalIndex))
                {
                    if(infos->inlineIsSelected(globalIndex)) {
                        glColor4ub(selColor.red(), selColor.green(), selColor.blue(), selColor.alpha());
                    } else {
                        setCurrentColor();
                    }

                    const CT_Edge &edge = m_eAccess.constEdgeAt(globalIndex);
                    glArrayElement(edge.iPointAt(0));
                    glArrayElement(edge.iPointAt(1));
                }
            }
        }
    }
}

void G3DPainter::drawCircle(const double &x, const double &y, const double &z, const double &radius)
{
    if(canDraw(GL_BEGIN_LINE))
    {
        startDrawMultiple(GL_BEGIN_LINE);

        QVector< QPair<double, double> > *cosSinA = &G3DPainter::VECTOR_CIRCLE_NORMAL;

        if(drawFastest())
            cosSinA = &G3DPainter::VECTOR_CIRCLE_FASTEST;

        int size = cosSinA->size();

        const QPair<double, double> &fPair = (*cosSinA)[0];
        Eigen::Vector4d lastV = m_modelViewMatrix4d * Eigen::Vector4d((fPair.first*radius) + x, (fPair.second*radius) + y, z, 1);
        Eigen::Vector4d firstV = lastV;

        for(int i=1; i<size; ++i)
        {
            const QPair<double, double> &pair = (*cosSinA)[i];

            Eigen::Vector4d newV = m_modelViewMatrix4d * Eigen::Vector4d((pair.first*radius) + x, (pair.second*radius) + y, z, 1);

            glVertex3dv(lastV.data());
            glVertex3dv(newV.data());

            lastV = newV;
        }

        glVertex3dv(lastV.data());
        glVertex3dv(firstV.data());
    }
}

void G3DPainter::drawCircle3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius)
{
    if(canDraw(GL_BEGIN_LINE))
    {
        startDrawMultiple(GL_BEGIN_LINE);

        Eigen::Vector3d u(0,0,0);

        if(fabs(direction(0)) >= fabs(direction(1)))
        {
            double factor = 1.0/sqrt(direction(0)*direction(0)+direction(2)*direction(2));
            u(0) = (-direction(2)*factor);
            u(2) = (direction(0)*factor);
        }
        else
        {
            double factor = 1.0/sqrt(direction(1)*direction(1)+direction(2)*direction(2));
            u(1) = (direction(2)*factor);
            u(2) = (-direction(1)*factor);
        }

        Eigen::Vector3d v = direction.cross(u);
        v.normalize();

        QVector< QPair<double, double> > *cosSinA = &G3DPainter::VECTOR_CIRCLE_NORMAL;

        if(drawFastest())
            cosSinA = &G3DPainter::VECTOR_CIRCLE_FASTEST;

        int size = cosSinA->size();

        const QPair<double, double> &fPair = (*cosSinA)[0];
        Eigen::Vector4d lastV = m_modelViewMatrix4d * Eigen::Vector4d(center(0) + (radius*fPair.first)*u(0) + (radius*fPair.second)*v(0),
                                                                      center(1) + (radius*fPair.first)*u(1) + (radius*fPair.second)*v(1),
                                                                      center(2) + (radius*fPair.first)*u(2) + (radius*fPair.second)*v(2), 1);
        Eigen::Vector4d firstV = lastV;

        for(int i=1; i<size; ++i)
        {
            const QPair<double, double> &pair = (*cosSinA)[i];

            Eigen::Vector4d newV = m_modelViewMatrix4d * Eigen::Vector4d(center(0) + (radius*pair.first)*u(0) + (radius*pair.second)*v(0),
                                                                         center(1) + (radius*pair.first)*u(1) + (radius*pair.second)*v(1),
                                                                         center(2) + (radius*pair.first)*u(2) + (radius*pair.second)*v(2), 1);

            glVertex3dv(lastV.data());
            glVertex3dv(newV.data());

            lastV = newV;
        }

        glVertex3dv(lastV.data());
        glVertex3dv(firstV.data());
    }
}

void G3DPainter::drawEllipse(const double &x, const double &y, const double &z, const double &radiusA, const double &radiusB)
{
    if(canDraw(GL_BEGIN_LINE))
    {
        startDrawMultiple(GL_BEGIN_LINE);

        QVector< QPair<double, double> > *cosSinA = &G3DPainter::VECTOR_CIRCLE_NORMAL;

        if(drawFastest())
            cosSinA = &G3DPainter::VECTOR_CIRCLE_FASTEST;

        int size = cosSinA->size();

        const QPair<double, double> &fPair = (*cosSinA)[0];
        Eigen::Vector4d lastV = m_modelViewMatrix4d * Eigen::Vector4d((fPair.first*radiusA) + x, (fPair.second*radiusB) + y, z, 1);
        Eigen::Vector4d firstV = lastV;

        for(int i=1; i<size; ++i)
        {
            const QPair<double, double> &pair = (*cosSinA)[i];

            Eigen::Vector4d newV = m_modelViewMatrix4d * Eigen::Vector4d((pair.first*radiusA) + x, (pair.second*radiusB) + y, z, 1);

            glVertex3dv(lastV.data());
            glVertex3dv(newV.data());

            lastV = newV;
        }

        glVertex3dv(lastV.data());
        glVertex3dv(firstV.data());
    }
}

void G3DPainter::drawTriangle(const double &x1, const double &y1, const double &z1,
                              const double &x2, const double &y2, const double &z2,
                              const double &x3, const double &y3, const double &z3)
{
    if(canDraw(GL_BEGIN_TRIANGLE))
    {
        startDrawMultiple(GL_BEGIN_TRIANGLE);

        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x1, y1, z1, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x2, y2, z2, 1)).data());
        glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(x3, y3, z3, 1)).data());
    }
}

void G3DPainter::drawTriangle(const size_t &p1GlobalIndex, const size_t &p2GlobalIndex, const size_t &p3GlobalIndex)
{
    if(canDraw(GL_BEGIN_TRIANGLE_FROM_PC))
    {
        startDrawMultiple(GL_BEGIN_TRIANGLE_FROM_PC);

        glArrayElement(p1GlobalIndex);
        glArrayElement(p2GlobalIndex);
        glArrayElement(p3GlobalIndex);
    }
}

void G3DPainter::drawFaces(const CT_AbstractMeshModel *mesh)
{
    if(mesh == NULL)
        return;

    const CT_AbstractCloudIndex *fIndex = mesh->getFaceCloudIndex();

    if(fIndex == NULL)
        return;

    if(canDraw(GL_BEGIN_TRIANGLE_FROM_PC))
    {
        startDrawMultiple(GL_BEGIN_TRIANGLE_FROM_PC);

        if(!m_gv->getOptions().useColor())
            setCurrentColor();

        QSharedPointer<CT_StandardColorCloudRegistered> colorCloud = m_gv->colorCloudOf(GraphicsViewInterface::CFaceCloud);
        QSharedPointer<CT_StandardNormalCloudRegistered> normalCloud = m_gv->normalCloudOf(GraphicsViewInterface::NFaceCloud);
        QColor selColor = m_gv->getOptions().getSelectedColor();
        DM_ElementInfoManager *infos = m_gv->facesInformationManager();
        size_t globalIndex;
        size_t size = fIndex->size();

        // W/ colors cloud
        if(m_useFColorCloud
                && (colorCloud.data() != NULL))
        {
            CT_AbstractColorCloud *cc = colorCloud->abstractColorCloud();

            // W/ normals cloud
            if(m_useFNormalCloud
                    && (normalCloud.data() != NULL))
            {
                CT_AbstractNormalCloud *nn = normalCloud->abstractNormalCloud();

                for(size_t i=0; i<size; ++i)
                {
                    fIndex->indexAt(i, globalIndex);

                    if(!infos->inlineIsInvisible(globalIndex))
                    {
                        if(infos->inlineIsSelected(globalIndex)) {
                            glColor4ub(selColor.red(), selColor.green(), selColor.blue(), selColor.alpha());
                        } else {
                            const CT_Color &color = cc->constColorAt(globalIndex);
                            glColor4ub(color.r, color.g, color.b, color.a);
                        }

                        glNormal3fv(nn->normalAt(globalIndex).vertex());

                        const CT_Face &face = m_fAccess.constFaceAt(globalIndex);
                        glArrayElement(face.iPointAt(0));
                        glArrayElement(face.iPointAt(1));
                        glArrayElement(face.iPointAt(2));
                    }
                }
            }
            // W/O normals cloud
            else
            {
                for(size_t i=0; i<size; ++i)
                {
                    fIndex->indexAt(i, globalIndex);

                    if(!infos->inlineIsInvisible(globalIndex))
                    {
                        if(infos->inlineIsSelected(globalIndex)) {
                            glColor4ub(selColor.red(), selColor.green(), selColor.blue(), selColor.alpha());
                        } else {
                            const CT_Color &color = cc->constColorAt(globalIndex);
                            glColor4ub(color.r, color.g, color.b, color.a);
                        }

                        const CT_Face &face = m_fAccess.constFaceAt(globalIndex);
                        glArrayElement(face.iPointAt(0));
                        glArrayElement(face.iPointAt(1));
                        glArrayElement(face.iPointAt(2));
                    }
                }
            }
        }
        else
        {
            // W/ normals cloud
            if(m_useFNormalCloud
                    && (normalCloud.data() != NULL))
            {
                CT_AbstractNormalCloud *nn = normalCloud->abstractNormalCloud();

                for(size_t i=0; i<size; ++i)
                {
                    fIndex->indexAt(i, globalIndex);

                    if(!infos->inlineIsInvisible(globalIndex))
                    {
                        if(infos->inlineIsSelected(globalIndex)) {
                            glColor4ub(selColor.red(), selColor.green(), selColor.blue(), selColor.alpha());
                        } else {
                            setCurrentColor();
                        }

                        glNormal3fv(nn->normalAt(globalIndex).vertex());

                        const CT_Face &face = m_fAccess.constFaceAt(globalIndex);
                        glArrayElement(face.iPointAt(0));
                        glArrayElement(face.iPointAt(1));
                        glArrayElement(face.iPointAt(2));
                    }
                }
            }
            // W/O normals cloud
            else
            {
                for(size_t i=0; i<size; ++i)
                {
                    fIndex->indexAt(i, globalIndex);

                    if(!infos->inlineIsInvisible(globalIndex))
                    {
                        if(infos->inlineIsSelected(globalIndex)) {
                            glColor4ub(selColor.red(), selColor.green(), selColor.blue(), selColor.alpha());
                        } else {
                            setCurrentColor();
                        }

                        const CT_Face &face = m_fAccess.constFaceAt(globalIndex);
                        glArrayElement(face.iPointAt(0));
                        glArrayElement(face.iPointAt(1));
                        glArrayElement(face.iPointAt(2));
                    }
                }
            }
        }
    }
}

void G3DPainter::drawMesh(const CT_AbstractMeshModel *mesh)
{
    drawFaces(mesh);
}

void G3DPainter::beginPolygon()
{
    if(canDraw(GL_BEGIN_LINE)) {
        startDrawMultiple(GL_BEGIN_LINE);
        m_firstPolygonPointValid = false;
    }
}

void G3DPainter::addPointToPolygon(const double &x, const double &y, const double &z)
{
    if(canDraw(GL_BEGIN_LINE) && (m_currentGlBeginType == GL_BEGIN_LINE)) {

        Eigen::Vector4d newV = m_modelViewMatrix4d * Eigen::Vector4d(x, y, z, 1);

        if(m_firstPolygonPointValid) {
            glVertex3dv(m_firstPolygonPoint.data());
            glVertex3dv(newV.data());
        }

        m_firstPolygonPoint = newV;
        m_firstPolygonPointValid = true;
    }
}

void G3DPainter::endPolygon()
{
    m_firstPolygonPointValid = false;
}

void G3DPainter::drawCylinder(const double &x, const double &y, const double &z, const double &radius, const double &height)
{
    if(canDraw(GL_BEGIN_TRIANGLE))
    {
        pushMatrix();

        translate(x, y, z);
        scale(radius, radius, height);

        if(drawFastest())
            CYLINDER_FASTEST.draw(*this);
        else
            CYLINDER_NORMAL.draw(*this);

        popMatrix();
    }
}

void G3DPainter::drawCylinder3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius, const double &height)
{
    if(canDraw(GL_BEGIN_TRIANGLE))
    {
        pushMatrix();

        Eigen::Vector3d dn = direction.normalized();

        double delta = -height/2.0;

        translateThenRotateToDirection(Eigen::Vector3d(center.x() + delta*dn.x(), center.y() + delta*dn.y(), center.z() + delta*dn.z()), direction);

        drawCylinder(0, 0, 0, radius, height);

        popMatrix();
    }
}

void G3DPainter::drawPyramid(const double &topX, const double &topY, const double &topZ,
                             const double &base1X, const double &base1Y, const double &base1Z,
                             const double &base2X, const double &base2Y, const double &base2Z,
                             const double &base3X, const double &base3Y, const double &base3Z,
                             const double &base4X, const double &base4Y, const double &base4Z)
{
    if(canDraw(GL_OTHER))
    {
        stopDrawMultiple();

        glBegin( GL_TRIANGLE_FAN );

            glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(topX, topY, topZ, 1)).data());

            glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(base1X, base1Y, base1Z, 1)).data());
            glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(base2X, base2Y, base2Z, 1)).data());
            glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(base3X, base3Y, base3Z, 1)).data());
            glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(base4X, base4Y, base4Z, 1)).data());

            glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(base1X, base1Y, base1Z, 1)).data());
        glEnd();

        glBegin( GL_QUADS );
            glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(base1X, base1Y, base1Z, 1)).data());
            glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(base2X, base2Y, base2Z, 1)).data());
            glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(base3X, base3Y, base3Z, 1)).data());
            glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(base4X, base4Y, base4Z, 1)).data());
        glEnd();
    }
}

void G3DPainter::drawPartOfSphere(const double &centerX, const double &centerY, const double &centerZ,
                                  const double &radius,
                                  const double &initTheta, const double &endTheta,
                                  const double &initPhi, const double &endPhi,
                                  bool radians)
{
    if(canDraw(GL_OTHER))
    {
        stopDrawMultiple();

        double iTheta = initTheta;
        double eTheta = endTheta;
        double iPhi = initPhi;
        double ePhi = endPhi;

        if ( !radians )
        {
            iTheta *= M_PI/180.0;
            eTheta *= M_PI/180.0;
            iPhi *= M_PI/180.0;
            ePhi *= M_PI/180.0;
        }

        double nbSteps = 60;
        double stepTheta = fabs(eTheta-iTheta) / nbSteps;
        double stepPhi = fabs(ePhi-iPhi) / nbSteps;

        double cosPhi, sinPhi, cosTheta, sinTheta;

        for ( double currentTheta = iTheta ; currentTheta <= eTheta ;  currentTheta += stepTheta )
        {
            glBegin( GL_LINE_STRIP );
            for ( float currentPhi = iPhi ; currentPhi <= ePhi ; currentPhi += stepPhi )
            {
                    sinPhi = sin (currentPhi);
                    cosPhi = cos (currentPhi);
                    sinTheta = sin (currentTheta);
                    cosTheta = cos (currentTheta);

                    glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(radius*sinPhi*cosTheta + centerX,
                                                                       radius*sinPhi*sinTheta + centerY,
                                                                       radius*cosPhi + centerZ, 1)).data());
            }
            glEnd();
        }

        for ( double currentPhi = iPhi ; currentPhi <= ePhi ; currentPhi += stepPhi )
        {
            glBegin( GL_LINE_STRIP );
            for ( double currentTheta = iTheta ; currentTheta <= eTheta ;  currentTheta += stepTheta )
            {
                    sinPhi = sin (currentPhi);
                    cosPhi = cos (currentPhi);
                    sinTheta = sin (currentTheta);
                    cosTheta = cos (currentTheta);

                    glVertex3dv(Eigen::Vector4d(m_modelViewMatrix4d * Eigen::Vector4d(radius*sinPhi*cosTheta + centerX,
                                                                       radius*sinPhi*sinTheta + centerY,
                                                                       radius*cosPhi + centerZ, 1)).data());

            }
            glEnd();
        }
    }
}



//////////// PROTECTED ///////////

void G3DPainter::initPointShader()
{
    if(QT_GL_CONTEXT::currentContext() != NULL)
    {
        if(!m_shaderPointError && m_ShaderPoint == NULL)
        {
            m_ShaderPoint = new QT_GL_SHADER(QT_GL_SHADER::Vertex);

            GUI_LOG->addInfoMessage(LogInterface::gui, QObject::tr("OpenGL version detected : %1").arg(QString().setNum(m_openglVersion, 'f', 2)));

            QString shaderSourceCode;

            if(m_openglVersion < 3)
                m_shaderSourceFile = "./shaders/points_120.vert";
            else
                m_shaderSourceFile = "./shaders/points.vert";

            QFile f(m_shaderSourceFile);

            if(f.open(QFile::ReadOnly)) {
                QTextStream stream(&f);

                shaderSourceCode = stream.readAll();

                shaderSourceCode.replace("csMatrix[64]", QString("csMatrix[%1]").arg(m_maxMatrix));

                f.close();
            }


            if(!m_ShaderPoint->compileSourceCode(shaderSourceCode))
            {
                QString log;
                QString tmp;

                #ifdef __unix__
                if(!(tmp = m_ShaderPoint->log()).isEmpty())
                #else
                while(!(tmp = m_ShaderPoint->log()).isEmpty())
                #endif
                    log += tmp;

                if(!log.isEmpty())
                    GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" compilation error : %2").arg(m_shaderSourceFile).arg(log));
                else
                    GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" compilation error").arg(m_shaderSourceFile));

                delete m_ShaderPoint;
                m_ShaderPoint = NULL;

                m_shaderPointError = true;
            }
        }

        if(!m_shaderPointError
                && !m_shaderProgPointError)
        {
            if(m_shaderProgPoint == NULL)
                m_shaderProgPoint = new QT_GL_SHADERPROGRAM();

            if(m_shaderProgPoint->shaders().isEmpty())
            {
                if(!m_shaderProgPoint->addShader(m_ShaderPoint) || !m_shaderProgPoint->link())
                {
                    QString log;
                    QString tmp;

                    #ifdef __unix__
                    if(!(tmp = m_shaderProgPoint->log()).isEmpty())
                    #else
                    while(!(tmp = m_shaderProgPoint->log()).isEmpty())
                    #endif
                        log += tmp;

                    if(!log.isEmpty())
                        GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" link error : %2").arg(m_shaderSourceFile).arg(log));
                    else
                        GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" link error").arg(m_shaderSourceFile));

                    m_shaderProgPointError = true;
                }
            }
        }
    }
}

bool G3DPainter::bindPointShader()
{
    initPointShader();

    if(!m_shaderPointError
            && !m_shaderProgPointError
            && (QT_GL_CONTEXT::currentContext() != NULL))
    {
        if(!m_shaderProgPoint->bind())
        {
            QString log;
            QString tmp;

            #ifdef __unix__
            if(!(tmp = m_shaderProgPoint->log()).isEmpty())
            #else
            while(!(tmp = m_shaderProgPoint->log()).isEmpty())
            #endif
                log += tmp;

            if(!log.isEmpty())
                GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" bind error : %2").arg(m_shaderSourceFile).arg(log));
            else
                GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" bind error").arg(m_shaderSourceFile));


            m_shaderProgPointError = true;
        }
        else
        {
            if(!m_shaderProgPointSet) {

                if(!m_shaderLocInitialized) {
                    m_shaderLocCsIndex = m_shaderProgPoint->attributeLocation("csIndex");
                    m_shaderLocInfo = m_shaderProgPoint->attributeLocation("info");
                    m_shaderLocCsMatrix = m_shaderProgPoint->uniformLocation("csMatrix");
                    m_shaderLocSelectionColor = m_shaderProgPoint->uniformLocation("selectionColor");
                    m_shaderLocCheckSelected = m_shaderProgPoint->uniformLocation("checkSelected");

                    m_shaderLocInitialized = true;

                    QString err;

                    if(m_shaderLocCsIndex == -1)
                        err += "\r\n* attribute \"csIndex\" not found.";

                    if(m_shaderLocInfo == -1)
                        err += "\r\n* attribute \"info\" not found.";

                    if(m_shaderLocCsMatrix == -1)
                        err += "\r\n* uniform \"csMatrix\" not found.";

                    if(m_shaderLocSelectionColor == -1)
                        err += "\r\n* uniform \"selectionColor\" not found.";

                    if(m_shaderLocCheckSelected == -1)
                        err += "\r\n* uniform \"checkSelected\" not found.";

                    if(!err.isEmpty())
                        GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" error :%2").arg(m_shaderSourceFile).arg(err));
                }

                if(m_gv->pointsInformationManager()->informations()->size() > 0)
                {
                    int s = PS_COORDINATES_SYS_MANAGER->size();

                    if(s > m_maxMatrix) {
                        GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Too many coordinate system for the vertex shader !"));
                        s = m_maxMatrix;
                    }

                    for(int i=0; i<s; ++i)
                        m_csMatrix[i] = (m_modelViewMatrix4d * PS_COORDINATES_SYS_MANAGER->coordinateSystemAt(i)->toMatrix4x4()).cast<float>();

                    QColor sColor = m_gv->getOptions().getSelectedColor();
                    m_shaderProgPoint->setUniformValue(m_shaderLocSelectionColor, QVector4D(sColor.redF(), sColor.greenF(), sColor.blueF(), sColor.alphaF()));

                    m_shaderProgPoint->setUniformValue(m_shaderLocCheckSelected, (GLuint)m_gv->pointsInformationManager()->checkSelected());

                    m_shaderProgPoint->enableAttributeArray("info");
                    glVertexAttribPointer(m_shaderLocInfo, 1, GL_UNSIGNED_BYTE, GL_FALSE, 0, &m_gv->pointsInformationManager()->informations()->constTAt(0));

                    m_shaderProgPoint->enableAttributeArray("csIndex");
                    glVertexAttribPointer(m_shaderLocCsIndex, 1, GL_UNSIGNED_INT, GL_FALSE, 0, &PS_COORDINATES_SYS_MANAGER->indexCloudOfCoordinateSystemOfPoints()->valueAt(0));

                    glUniformMatrix4fv(m_shaderLocCsMatrix, m_csMatrix.size(), GL_FALSE, &m_csMatrix[0](0,0));

                    m_shaderProgPointSet = true;
                }

            } else {
                m_shaderProgPoint->enableAttributeArray("info");
                m_shaderProgPoint->enableAttributeArray("csIndex");
            }

            return true;
        }
    }

    return false;
}

void G3DPainter::releasePointShader(bool bindOk)
{
    if(bindOk)
        m_shaderProgPoint->release();
}

void G3DPainter::initDoubleElementShader()
{
    if(QT_GL_CONTEXT::currentContext() != NULL)
    {
        if(!m_shaderDeError && m_shaderDe == NULL)
        {
            m_shaderDe = new QT_GL_SHADER(QT_GL_SHADER::Vertex);

            m_shaderDeSourceFile = "./shaders/others.vert";

            if(!m_shaderDe->compileSourceFile(m_shaderDeSourceFile))
            {
                QString log;
                QString tmp;

                #ifdef __unix__
                if(!(tmp = m_shaderDe->log()).isEmpty())
                #else
                while(!(tmp = m_shaderDe->log()).isEmpty())
                #endif
                    log += tmp;

                if(!log.isEmpty())
                    GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" compilation error : %2").arg(m_shaderDeSourceFile).arg(log));
                else
                    GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" compilation error").arg(m_shaderDeSourceFile));

                delete m_shaderDe;
                m_shaderDe = NULL;

                m_shaderDeError = true;
            }
        }

        if(!m_shaderDeError
                && !m_shaderDeProgError)
        {
            if(m_shaderDeProg == NULL)
                m_shaderDeProg = new QT_GL_SHADERPROGRAM();

            if(m_shaderDeProg->shaders().isEmpty())
            {
                if(!m_shaderDeProg->addShader(m_shaderDe) || !m_shaderDeProg->link())
                {
                    QString log;
                    QString tmp;

                    #ifdef __unix__
                    if(!(tmp = m_shaderDeProg->log()).isEmpty())
                    #else
                    while(!(tmp = m_shaderDeProg->log()).isEmpty())
                    #endif
                        log += tmp;

                    if(!log.isEmpty())
                        GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" link error : %2").arg(m_shaderDeSourceFile).arg(log));
                    else
                        GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" link error").arg(m_shaderDeSourceFile));

                    m_shaderDeProgError = true;
                }
            }
        }
    }
}

bool G3DPainter::bindDoubleElementShader()
{
    initDoubleElementShader();

    if(!m_shaderDeError
            && !m_shaderDeProgError
            && (QT_GL_CONTEXT::currentContext() != NULL))
    {
        if(!m_shaderDeProg->bind())
        {
            QString log;
            QString tmp;

            #ifdef __unix__
            if(!(tmp = m_shaderDeProg->log()).isEmpty())
            #else
            while(!(tmp = m_shaderDeProg->log()).isEmpty())
            #endif
                log += tmp;

            if(!log.isEmpty())
                GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" bind error : %2").arg(m_shaderDeSourceFile).arg(log));
            else
                GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" bind error").arg(m_shaderDeSourceFile));


            m_shaderDeProgError = true;
        }
        else
        {
            if(!m_shaderDeLocInitialized) {
                m_shaderDeLocPMatrix1 = m_shaderDeProg->attributeLocation("pMatrix1");
                m_shaderDeLocPMatrix2 = m_shaderDeProg->attributeLocation("pMatrix2");
                m_shaderDeLocPMatrix3 = m_shaderDeProg->attributeLocation("pMatrix3");
                m_shaderDeLocPMatrix4 = m_shaderDeProg->attributeLocation("pMatrix4");

                m_shaderDeLocInitialized = true;

                QString err;

                if(m_shaderDeLocPMatrix1 == -1)
                    err += "\r\n* attribute \"pMatrix1\" not found.";

                if(m_shaderDeLocPMatrix2 == -1)
                    err += "\r\n* attribute \"pMatrix2\" not found.";

                if(m_shaderDeLocPMatrix3 == -1)
                    err += "\r\n* attribute \"pMatrix3\" not found.";

                if(m_shaderDeLocPMatrix4 == -1)
                    err += "\r\n* attribute \"pMatrix4\" not found.";

                if(!err.isEmpty())
                    GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("Vertex shader \"%1\" error :%2").arg(m_shaderDeSourceFile).arg(err));
            }

            setDoubleElementMatrix(Eigen::Matrix4d::Identity());

            return true;
        }
    }

    return false;
}

void G3DPainter::setDoubleElementMatrix(const Eigen::Matrix4d &mat)
{
    m_shaderDeProg->setAttributeValue(m_shaderDeLocPMatrix1, mat(0, 0), mat(1, 0), mat(2, 0), mat(3, 0));
    m_shaderDeProg->setAttributeValue(m_shaderDeLocPMatrix2, mat(0, 1), mat(1, 1), mat(2, 1), mat(3, 1));
    m_shaderDeProg->setAttributeValue(m_shaderDeLocPMatrix3, mat(0, 2), mat(1, 2), mat(2, 2), mat(3, 2));
    m_shaderDeProg->setAttributeValue(m_shaderDeLocPMatrix4, mat(0, 3), mat(1, 3), mat(2, 3), mat(3, 3));
}

void G3DPainter::releaseDoubleElementShader(bool bindOk)
{
    if(bindOk)
        m_shaderDeProg->release();
}

void G3DPainter::setPolygonMode(GLenum faces, GLenum mode)
{
    if(m_polygonMode != mode) {
        stopDrawMultiple();
        glPolygonMode(faces, mode);
        m_polygonMode = mode;
    }
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
    float a = 0;

    for(int i=0; i<size; ++i)
    {
        QPair<double, double> &pair = vector[i];
        pair.first = cos(a);
        pair.second = sin(a);

        a += inc;
    }

    return vector;
}

G3DPainter::G3DPainterCylinder G3DPainter::staticInitCylinder(const QVector<QPair<double, double> > &cosSinAlpha)
{
    return G3DPainter::G3DPainterCylinder(cosSinAlpha);
}

bool G3DPainter::canDraw(G3DPainter::GlBeginType type) const
{
    if(m_drawOnly == DRAW_ALL)
        return true;

    if((type == GL_BEGIN_POINT) && (m_drawOnly == POINT))
        return true;

    if((type == GL_BEGIN_POINT_FROM_PC) && (m_drawOnly == POINT_FROM_PC))
        return true;

    if((type == GL_BEGIN_LINE) && (m_drawOnly == LINE))
        return true;

    if((type == GL_BEGIN_LINE_FROM_PC) && (m_drawOnly == LINE_FROM_PC))
        return true;

    if((type == GL_BEGIN_TRIANGLE) && (m_drawOnly == TRIANGLE))
        return true;

    if((type == GL_BEGIN_TRIANGLE_FROM_PC) && (m_drawOnly == TRIANGLE_FROM_PC))
        return true;

    if((type == GL_BEGIN_QUAD) && (m_drawOnly == QUAD))
        return true;

    if((type == GL_BEGIN_QUAD_FROM_PC) && (m_drawOnly == QUAD_FROM_PC))
        return true;

    if((type == GL_OTHER) && (m_drawOnly == OTHER))
        return true;

    return false;
}

void G3DPainter::startDrawMultiple(GlBeginType type)
{
    m_firstPolygonPointValid = false;

    // call glEnd() if it was not already called and if we change
    // the type of glBegin(...)
    callGlEndIfGlBeginChanged(type);

    // if we have not already started a glBegin
    if(m_currentGlBeginType == GL_END_CALLED)
    {
        // if we want to draw points or points from cloud
        if((type == GL_BEGIN_POINT) || (type == GL_BEGIN_POINT_FROM_PC))
        {
            if((type == GL_BEGIN_POINT_FROM_PC) && !m_bindShaderPointOK)
                m_bindShaderPointOK = bindPointShader();
            /*else if((type == GL_BEGIN_POINT) && !m_bindShaderDeOK)
                m_bindShaderDeOK = bindDoubleElementShader();*/

            glBegin(GL_POINTS);
            m_currentGlBeginType = type;
        }
        // else if we want to draw triangles or triangles with points from cloud
        else if((type == GL_BEGIN_TRIANGLE) || (type == GL_BEGIN_TRIANGLE_FROM_PC))
        {
            if((type == GL_BEGIN_TRIANGLE_FROM_PC) && !m_bindShaderPointOK)
                m_bindShaderPointOK = bindPointShader();
            /*else if((type == GL_BEGIN_TRIANGLE) && !m_bindShaderDeOK)
                m_bindShaderDeOK = bindDoubleElementShader();*/

            glBegin(GL_TRIANGLES);
            m_currentGlBeginType = type;
        }
        // else if we want to draw lines or lines with points from cloud
        else if((type == GL_BEGIN_LINE) || (type == GL_BEGIN_LINE_FROM_PC))
        {
            if((type == GL_BEGIN_LINE_FROM_PC) && !m_bindShaderPointOK)
                m_bindShaderPointOK = bindPointShader();
            /*else if((type == GL_BEGIN_LINE) && !m_bindShaderDeOK)
                m_bindShaderDeOK = bindDoubleElementShader();*/

            glBegin(GL_LINES);
            m_currentGlBeginType = type;
        }
        // else if we want to draw quads or quads with points from cloud
        else if((type == GL_BEGIN_QUAD) || (type == GL_BEGIN_QUAD_FROM_PC))
        {
            if((type == GL_BEGIN_QUAD_FROM_PC) && !m_bindShaderPointOK)
                m_bindShaderPointOK = bindPointShader();
            /*else if((type == GL_BEGIN_QUAD) && !m_bindShaderDeOK)
                m_bindShaderDeOK = bindDoubleElementShader();*/

            glBegin(GL_QUADS);
            m_currentGlBeginType = type;
        }
    }

    // if we must draw other elements or all elements we don't call glBegin
}

void G3DPainter::stopDrawMultiple(bool rPointShader, bool rDeShader)
{
    m_firstPolygonPointValid = false;

    // if we have not call "glEnd()"
    if(m_currentGlBeginType != GL_END_CALLED)
    {
        glEnd();

        m_currentGlBeginType = GL_END_CALLED;
    }

    // if we want to release the point shader
    if(m_bindShaderPointOK && rPointShader) {
        releasePointShader(m_bindShaderPointOK);
        m_bindShaderPointOK = false;
    }

    // if we want to release the double element shader
    /*if(m_bindShaderDeOK && rDeShader) {
        releaseDoubleElementShader(m_bindShaderDeOK);
        m_bindShaderDeOK = false;
    }*/
}

void G3DPainter::callGlEndIfGlBeginChanged(G3DPainter::GlBeginType newGlBeginType)
{
    // if the current "glBegin(...)" was the same type
    if(m_currentGlBeginType == newGlBeginType) {
        return; // we must not call "glEnd"
    }
    // else if we currently draw with points from global cloud and now we want to draw
    // with double values
    else if(((newGlBeginType == GL_BEGIN_POINT) && (m_currentGlBeginType == GL_BEGIN_POINT_FROM_PC))
            || ((newGlBeginType == GL_BEGIN_LINE) && (m_currentGlBeginType == GL_BEGIN_LINE_FROM_PC))
            || ((newGlBeginType == GL_BEGIN_TRIANGLE) && (m_currentGlBeginType == GL_BEGIN_TRIANGLE_FROM_PC))
            || ((newGlBeginType == GL_BEGIN_QUAD) && (m_currentGlBeginType == GL_BEGIN_QUAD_FROM_PC)))
    {
        // we must just release the point shader
        releasePointShader(m_bindShaderPointOK);
        m_bindShaderPointOK = false;

        // and bind the double element shader
        /*if(!m_bindShaderDeOK)
            m_bindShaderDeOK = bindDoubleElementShader();*/
    }
    // else if we currently draw with double values and now we want to draw
    // with points from global cloud
    else if(((newGlBeginType == GL_BEGIN_POINT) && (m_currentGlBeginType == GL_BEGIN_POINT_FROM_PC))
            || ((newGlBeginType == GL_BEGIN_LINE) && (m_currentGlBeginType == GL_BEGIN_LINE_FROM_PC))
            || ((newGlBeginType == GL_BEGIN_TRIANGLE) && (m_currentGlBeginType == GL_BEGIN_TRIANGLE_FROM_PC))
            || ((newGlBeginType == GL_BEGIN_QUAD) && (m_currentGlBeginType == GL_BEGIN_QUAD_FROM_PC)))
    {
        // we must just release the double element shader
        /*releaseDoubleElementShader(m_bindShaderDeOK);
        m_bindShaderDeOK = false;*/

        // and bind the point shader
        if(!m_bindShaderPointOK)
            m_bindShaderPointOK = bindPointShader();
    }
    else if(m_currentGlBeginType != GL_END_CALLED) {
        stopDrawMultiple(); // otherwise we call glEnd() if it was not already called
    }
}

//////////////// G3DPainterCylinder ///////////////

G3DPainter::G3DPainterCylinder::G3DPainterCylinder(const QVector<QPair<double, double> > &cosSinAlpha)
{
    int sides = cosSinAlpha.size();
    double minH = 0;
    double maxH = 1;

    Eigen::Vector4d v;
    v(0) = 0;
    v(1) = 1;
    v(2) = maxH;
    v(3) = 1;

    for(size_t i=0; i<sides; ++i)
    {
        const QPair<double, double> &fPair = cosSinAlpha[i];

        Eigen::Vector4d v0 = v;

        Eigen::Vector4d v1 = v0;
        v1(2) = minH;

        Eigen::Vector4d v2;
        v2(3) = 1;

        Eigen::Vector4d v3;
        v3(3) = 1;

        if(i<sides-1)
        {
            v2(0) = fPair.second;
            v2(1) = fPair.first;
            v2(2) = maxH;

            v3(0) = v2(0);
            v3(1) = v2(1);
            v3(2) = minH;

            v = v2;
        }
        else
        {
            v2(0) = 0;
            v2(1) = 1;
            v2(2) = maxH;

            v3(0) = v2(0);
            v3(1) = v2(1);
            v3(2) = minH;
        }

        m_v.push_back(v0);
        m_v.push_back(v1);
        m_v.push_back(v2);
        m_v.push_back(v3);
    }
}

void G3DPainter::G3DPainterCylinder::draw(G3DPainter &painter) const
{
    int s = m_v.size();

    int i = 0;

    if(painter.canDraw(GL_BEGIN_TRIANGLE))
    {
        painter.startDrawMultiple(GL_BEGIN_TRIANGLE);

        while(i<s) {

            Eigen::Vector4d v0 = painter.m_modelViewMatrix4d * m_v[i++];
            Eigen::Vector4d v1 = painter.m_modelViewMatrix4d * m_v[i++];
            Eigen::Vector4d v2 = painter.m_modelViewMatrix4d * m_v[i++];
            Eigen::Vector4d v3 = painter.m_modelViewMatrix4d * m_v[i++];

            glVertex3dv(v0.data());
            glVertex3dv(v1.data());
            glVertex3dv(v2.data());

            glVertex3dv(v1.data());
            glVertex3dv(v3.data());
            glVertex3dv(v2.data());
        }
    }
}
