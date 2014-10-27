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

#include <QQuaternion>

#define M_PI_MULT_2 6.28318530717958647692

QVector< QPair<double, double> > G3DPainter::VECTOR_CIRCLE_FASTEST = G3DPainter::staticInitCircleVector(G3DPainter::VECTOR_CIRCLE_FASTEST_SIZE);
QVector< QPair<double, double> > G3DPainter::VECTOR_CIRCLE_NORMAL = G3DPainter::staticInitCircleVector(G3DPainter::VECTOR_CIRCLE_NORMAL_SIZE);

G3DPainter::G3DPainter()
{
    m_gv = NULL;

    _quadric = gluNewQuadric();
    _color = Qt::white;
    _forcedColor = Qt::white;
    _defaultPointSize = 1.0;
    _drawFastest = false;

    m_shaderProgPoint = new QT_GL_SHADERPROGRAM();
    /*m_shaderProgFace = new QT_GL_SHADERPROGRAM();
    m_shaderProgEdge = new QT_GL_SHADERPROGRAM();*/

    m_shaderProgPointError = false;
    /*m_shaderProgFaceError = false;
    m_shaderProgEdgeError = false;*/

    m_ShaderPoint = NULL;
    /*m_ShaderFace = NULL;
    m_ShaderEdge = NULL;*/

    m_shaderPointError = false;
    /*m_shaderFaceError = false;
    m_shaderEdgeError = false;*/

    m_pointCloud = PS_REPOSITORY->globalCloud<CT_Point>();
    m_faceCloud = PS_REPOSITORY->globalCloud<CT_Face>();
    m_edgeCloud = PS_REPOSITORY->globalCloud<CT_Edge>();

    beginNewDraw();
}

G3DPainter::~G3DPainter()
{
    gluDeleteQuadric(_quadric);

    delete m_shaderProgPoint;
    m_shaderProgPoint = NULL;
    delete m_ShaderPoint;

    /*delete m_shaderProgFace;
    m_shaderProgFace = NULL;
    delete m_ShaderFace;

    delete m_shaderProgEdge;
    m_shaderProgEdge = NULL;
    delete m_ShaderEdge;*/

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

    m_drawMultipleLine = false;
    m_drawMultipleTriangle = false;
    m_drawMultiplePoint = false;

    m_beginMultipleEnable = false;

    m_fastestIncrementPoint = 0;

    m_octreeCellsDraw = 0;

    m_shaderProgPointSet = false;
    /*m_shaderProgFaceSet = false;
    m_shaderProgEdgeSet = false;*/

    m_bindShaderPointOK = false;

    // DRAW ALL elements
    m_drawOnly = DRAW_ALL;
}

void G3DPainter::endNewDraw()
{
    resetDrawMultiple();
}

void G3DPainter::setDrawOnly(G3DPainter::MultiDrawableType type)
{
    resetDrawMultiple();

    // the new type to draw
    m_drawOnly = type;

    startDrawMultiple();
}

void G3DPainter::save()
{
    stopDrawMultiple();

    glPushMatrix();
    glGetIntegerv(GL_POLYGON_MODE, &m_polygonMode);
}

void G3DPainter::restore()
{
    stopDrawMultiple();

    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
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
    {
        stopDrawMultiple();
        glPushMatrix();
    }
}

// add overloaded functions which call the underlying OpenGL function
inline void glMultMatrix(const GLfloat  *m) { glMultMatrixf(m); }
inline void glMultMatrix(const GLdouble *m) { glMultMatrixd(m); }

// add an overload for QMatrix4x4 for convenience
inline void glMultMatrix(const QMatrix4x4 &m) { glMultMatrix(m.constData()); }

void G3DPainter::multMatrix(const QMatrix4x4 &matrix)
{
    if(_nCallEnablePushMatrix == 0)
    {
        stopDrawMultiple();
        glMultMatrix(matrix);
    }
}

void G3DPainter::popMatrix()
{
    if(_nCallEnablePushMatrix == 0)
    {
        stopDrawMultiple();
        glPopMatrix();
    }
}

void G3DPainter::setPointSize(double size)
{
    if(_nCallEnableSetPointSize == 0)
    {
        stopDrawMultiple();
        glPointSize(size);
    }
}

void G3DPainter::setDefaultPointSize(double size)
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

void G3DPainter::translate(double x, double y, double z)
{
    stopDrawMultiple();
    glTranslated(x, y, z);
}

void G3DPainter::rotate(double alpha, double x, double y, double z)
{
    stopDrawMultiple();
    glRotated(alpha, x, y, z);
}

void G3DPainter::translateThenRotateToDirection(const QVector3D &translation, const QVector3D &direction)
{
    stopDrawMultiple();

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
    stopDrawMultiple();

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
                const CT_AbstractCloudIndex *indexes = octree->at(x, y, z);

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
                            drawPointCloud(NULL, indexes, 10);
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
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == POINT))
    {
        startDrawMultiple();

        beginMultiplePoints();

        glVertex3d(x, y, z);

        endMultiplePoints();
    }
}

void G3DPainter::drawPoint(double *p)
{
    drawPoint(p[0], p[1], p[2]);
}

void G3DPainter::drawPoint(float *p)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == POINT))
    {
        startDrawMultiple();

        beginMultiplePoints();

        glVertex3f(p[0], p[1], p[2]);

        endMultiplePoints();
    }
}

void G3DPainter::drawPointCloud(const CT_AbstractPointCloud *pc,
                                const CT_AbstractCloudIndex *pci,
                                int fastestIncrement)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == POINT_CLOUD) || (m_drawOnly == LINE))
    {
        if(pci == NULL)
            return;

        size_t indexCount = pci->size();

        if(indexCount == 0)
            return;

        startDrawMultiple();

        bool bindOk = bindPointShader(false);

        if(!m_gv->getOptions().useColor())
            setCurrentColor();

        size_t n = 0;
        size_t increment = 1;
        size_t globalIndex;

        if((m_fastestIncrementPoint == 0) && (fastestIncrement > 0) && drawFastest())
            increment = fastestIncrement;
        else if((m_fastestIncrementPoint != 0) && drawFastest())
            increment = m_fastestIncrementPoint;

        QSharedPointer<CT_StandardNormalCloudRegistered> normalCloud = m_gv->normalCloudOf(GraphicsViewInterface::NPointCloud);

        DM_ElementInfoManager *infos = m_gv->pointsInformationManager();

        // FAST
        if(increment > 1)
        {
            if(m_usePNormalCloud
                    && (normalCloud.data() != NULL)
                    && ((m_drawOnly == DRAW_ALL) || (m_drawOnly == LINE)))
            {
                CT_AbstractNormalCloud *nn = normalCloud->abstractNormalCloud();

                beginDrawMultipleLine();

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

                endDrawMultipleLine();
            }

            if((m_drawOnly == DRAW_ALL) || (m_drawOnly == POINT_CLOUD))
            {
                beginMultiplePoints();

                n = 0;
                while(n < indexCount)
                {
                    pci->indexAt(n, globalIndex);

                    if(!infos->inlineIsInvisible(globalIndex))
                        glArrayElement(globalIndex);

                    n += increment;
                }

                endMultiplePoints();
            }
        }
        // NORMAL
        else
        {
            if(m_usePNormalCloud
                    && (normalCloud.data() != NULL)
                    && ((m_drawOnly == DRAW_ALL) || (m_drawOnly == LINE)))
            {

                CT_AbstractNormalCloud *nn = normalCloud->abstractNormalCloud();

                beginDrawMultipleLine();

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

                endDrawMultipleLine();
            }

            /*if(dynamic_cast<const CT_CloudIndexLessMemoryT<CT_Point>*>(pci) != NULL) {
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

                if((m_drawOnly == DRAW_ALL) || (m_drawOnly == POINT_CLOUD))
                {
                    beginMultiplePoints();

                    n = 0;
                    while(n < indexCount)
                    {
                        pci->indexAt(n, globalIndex);

                        if(!infos->inlineIsInvisible(globalIndex))
                            glArrayElement(globalIndex);

                        ++n;
                    }

                    endMultiplePoints();
                }
            //}
        }

        releasePointShader(bindOk);
    }
}

void G3DPainter::drawMesh(const CT_AbstractMeshModel *mesh)
{
    drawFaces(mesh);
}

void G3DPainter::drawFaces(const CT_AbstractMeshModel *mesh)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == FACE))
    {
        if(mesh == NULL)
            return;

        const CT_AbstractCloudIndex *fIndex = mesh->getFaceCloudIndex();

        if(fIndex == NULL)
            return;

        startDrawMultiple();

        //bool bindOk = bindFaceShader();

        if(!m_gv->getOptions().useColor())
            setCurrentColor();

        QSharedPointer<CT_StandardColorCloudRegistered> colorCloud = m_gv->colorCloudOf(GraphicsViewInterface::CFaceCloud);
        QSharedPointer<CT_StandardNormalCloudRegistered> normalCloud = m_gv->normalCloudOf(GraphicsViewInterface::NFaceCloud);
        QColor selColor = m_gv->getOptions().getSelectedColor();
        DM_ElementInfoManager *infos = m_gv->facesInformationManager();
        size_t globalIndex;
        size_t size = fIndex->size();

        beginDrawMultipleTriangle();

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

                        const CT_Face &face = m_faceCloud->constTAt(globalIndex);
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

                        const CT_Face &face = m_faceCloud->constTAt(globalIndex);
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

                        const CT_Face &face = m_faceCloud->constTAt(globalIndex);
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

                        const CT_Face &face = m_faceCloud->constTAt(globalIndex);
                        glArrayElement(face.iPointAt(0));
                        glArrayElement(face.iPointAt(1));
                        glArrayElement(face.iPointAt(2));
                    }
                }
            }
        }

        endDrawMultipleTriangle();

        //releaseFaceShader(bindOk);
    }
}

void G3DPainter::drawEdges(const CT_AbstractMeshModel *mesh)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == EDGE))
    {
        if(mesh == NULL)
            return;

        const CT_AbstractCloudIndex *eIndex = mesh->getEdgeCloudIndex();

        if(eIndex == NULL)
            return;

        startDrawMultiple();

        //bool bindOk = bindEdgeShader();

        if(!m_gv->getOptions().useColor())
            setCurrentColor();

        QSharedPointer<CT_StandardColorCloudRegistered> colorCloud = m_gv->colorCloudOf(GraphicsViewInterface::CEdgeCloud);
        //QSharedPointer<CT_StandardNormalCloudRegistered> normalCloud = m_gv->normalCloudOf(GraphicsViewInterface::NEdgeCloud);
        QColor selColor = m_gv->getOptions().getSelectedColor();
        DM_ElementInfoManager *infos = m_gv->edgesInformationManager();
        size_t size = eIndex->size();
        size_t globalIndex;

        beginDrawMultipleLine();

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

                    const CT_Edge &edge = m_edgeCloud->constTAt(globalIndex);
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

                    const CT_Edge &edge = m_edgeCloud->constTAt(globalIndex);
                    glArrayElement(edge.iPointAt(0));
                    glArrayElement(edge.iPointAt(1));
                }
            }
        }

        endDrawMultipleLine();
        //releaseEdgeShader(bindOk);
    }
}

void G3DPainter::drawPoints(const CT_AbstractMeshModel *mesh, int fastestIncrement)
{
    drawPointCloud(NULL, mesh->getPointCloudIndex(), fastestIncrement);
}

void G3DPainter::beginMultiplePoints()
{
    if(m_drawOnly == DRAW_ALL)
        glBegin(GL_POINTS);

    m_drawMultiplePoint = true;
}

void G3DPainter::addPoint(float *p)
{
    startDrawMultiple();

    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == POINT))
        glVertex3fv(p);
}

void G3DPainter::endMultiplePoints()
{
    if(m_drawOnly == DRAW_ALL)
        glEnd();

    m_drawMultiplePoint = false;
}

void G3DPainter::drawRectXY(const QRectF &rectangle, double z)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
    {
        glBegin(GL_LINE_LOOP);

        glVertex3d(rectangle.left(), rectangle.top(), z);
        glVertex3d(rectangle.right(), rectangle.top(), z);
        glVertex3d(rectangle.right(), rectangle.bottom(), z);
        glVertex3d(rectangle.left(), rectangle.bottom(), z);

        glEnd();
    }
}

void G3DPainter::fillRectXY(const QRectF &rectangle, double z)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == QUADS))
    {
        startDrawMultiple();

        if(m_drawOnly == DRAW_ALL)
            glBegin(GL_QUADS);

        glVertex3d(rectangle.left(), rectangle.top(), z);
        glVertex3d(rectangle.right(), rectangle.top(), z);
        glVertex3d(rectangle.right(), rectangle.bottom(), z);
        glVertex3d(rectangle.left(), rectangle.bottom(), z);

        if(m_drawOnly == DRAW_ALL)
            glEnd();
    }
}

void G3DPainter::drawRectXZ(const QRectF &rectangle, double y)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
    {
        glBegin(GL_LINE_LOOP);

        glVertex3d(rectangle.left(), y, rectangle.top());
        glVertex3d(rectangle.right(), y, rectangle.top());
        glVertex3d(rectangle.right(), y, rectangle.bottom());
        glVertex3d(rectangle.left(), y, rectangle.bottom());

        glEnd();
    }
}

void G3DPainter::fillRectXZ(const QRectF &rectangle, double y)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == QUADS))
    {
        startDrawMultiple();

        if(m_drawOnly == DRAW_ALL)
            glBegin(GL_QUADS);

        glVertex3d(rectangle.left(), y, rectangle.top());
        glVertex3d(rectangle.right(), y, rectangle.top());
        glVertex3d(rectangle.right(), y, rectangle.bottom());
        glVertex3d(rectangle.left(), y, rectangle.bottom());

        if(m_drawOnly == DRAW_ALL)
            glEnd();
    }
}

void G3DPainter::drawRectYZ(const QRectF &rectangle, double x)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
    {
        glBegin(GL_LINE_LOOP);

        glVertex3d(x, rectangle.left(), rectangle.top());
        glVertex3d(x, rectangle.right(), rectangle.top());
        glVertex3d(x, rectangle.right(), rectangle.bottom());
        glVertex3d(x, rectangle.left(), rectangle.bottom());

        glEnd();
    }
}

void G3DPainter::fillRectYZ(const QRectF &rectangle, double x)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == QUADS))
    {
        startDrawMultiple();

        if(m_drawOnly == DRAW_ALL)
            glBegin(GL_QUADS);

        glVertex3d(x, rectangle.left(), rectangle.top());
        glVertex3d(x, rectangle.right(), rectangle.top());
        glVertex3d(x, rectangle.right(), rectangle.bottom());
        glVertex3d(x, rectangle.left(), rectangle.bottom());

        if(m_drawOnly == DRAW_ALL)
            glEnd();
    }
}

void G3DPainter::beginPolygon()
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
        glBegin(GL_LINE_STRIP);
}

void G3DPainter::addPointToPolygon(float *p)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
        glVertex3fv(p);
}

void G3DPainter::endPolygon()
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
        glEnd();
}

void G3DPainter::beginDrawMultipleLine()
{
    if(m_drawOnly == DRAW_ALL)
        glBegin(GL_LINES);

    m_drawMultipleLine = true;
}

void G3DPainter::drawLine(double x1, double y1, double z1, double x2, double y2, double z2)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == LINE))
    {
        startDrawMultiple();

        beginDrawMultipleLine();

        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y2, z2);

        endDrawMultipleLine();
    }
}

void G3DPainter::drawLine(const float *p1, const float *p2)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == LINE))
    {
        startDrawMultiple();

        beginDrawMultipleLine();

        glVertex3fv(p1);
        glVertex3fv(p2);

        endDrawMultipleLine();
    }
}

void G3DPainter::endDrawMultipleLine()
{
    if(m_drawOnly == DRAW_ALL)
        glEnd();

    m_drawMultipleLine = false;
}

void G3DPainter::drawCircle(double x, double y, double z, double radius)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
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
}

void G3DPainter::drawCircle3D(const QVector3D &center, const QVector3D &direction, double radius)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
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
}

void G3DPainter::drawCylinder(double x, double y, double z, double radius, double height)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
    {
        save();

        translate(x, y, z);

        gluCylinder(_quadric, radius, radius, height, 20, 1);

        restore();
    }
}

void G3DPainter::drawCylinder3D(const QVector3D &center, const QVector3D &direction, double radius, double height)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
    {
        save();

        float delta = - height/2.0;
        translateThenRotateToDirection(QVector3D(center.x() + delta*direction.x(), center.y() + delta*direction.y(), center.z() + delta*direction.z()), direction);

        gluCylinder(_quadric, radius, radius, height, 20, 1);

        restore();
    }
}

void G3DPainter::drawEllipse(double x, double y, double z, double radiusA, double radiusB)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
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
}

void G3DPainter::beginDrawMultipleTriangle()
{
    if(m_drawOnly == DRAW_ALL)
        glBegin( GL_TRIANGLES );

    m_drawMultipleTriangle = true;
}

void G3DPainter::drawTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == TRIANGLE))
    {
        startDrawMultiple();

        beginDrawMultipleTriangle();

        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y2, z2);
        glVertex3d(x3, y3, z3);

        endDrawMultipleTriangle();
    }
}

void G3DPainter::drawTriangle(const float *p1, const float *p2, const float *p3)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == TRIANGLE))
    {
        startDrawMultiple();

        beginDrawMultipleTriangle();

        glVertex3fv(p1);
        glVertex3fv(p2);
        glVertex3fv(p3);

        endDrawMultipleTriangle();
    }
}

void G3DPainter::endDrawMultipleTriangle()
{
    if(m_drawOnly == DRAW_ALL)
        glEnd();

    m_drawMultipleTriangle = false;
}

void G3DPainter::drawCube(double x1, double y1, double z1, double x2, double y2, double z2)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == QUADS))
    {
        startDrawMultiple();

        if(m_drawOnly == DRAW_ALL)
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

        if(m_drawOnly == DRAW_ALL)
            glEnd();
    }
}

void G3DPainter::drawPyramid(double topX, double topY, double topZ, double base1X, double base1Y, double base1Z, double base2X, double base2Y, double base2Z, double base3X, double base3Y, double base3Z, double base4X, double base4Y, double base4Z)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
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
}

void G3DPainter::drawPartOfSphere(double centerX, double centerY, double centerZ, double radius, double initTheta, double endTheta, double initPhi, double endPhi, bool radians)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
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
}

void G3DPainter::fillQuadFace(float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float x4, float y4, float z4)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == QUADS))
    {
        startDrawMultiple();

        if(m_drawOnly == DRAW_ALL)
            glBegin(GL_QUADS);

        glVertex3d(x1, y1, z1 );
        glVertex3d(x2, y2, z2 );
        glVertex3d(x3, y3, z3 );
        glVertex3d(x4, y4, z4 );

        if(m_drawOnly == DRAW_ALL)
            glEnd();
    }
}

void G3DPainter::fillQuadFace(float x1, float y1, float z1, int r1, int g1, int b1,
                               float x2, float y2, float z2, int r2, int g2, int b2,
                               float x3, float y3, float z3, int r3, int g3, int b3,
                               float x4, float y4, float z4, int r4, int g4, int b4)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == QUADS))
    {
        startDrawMultiple();

        if(m_drawOnly == DRAW_ALL)
            glBegin(GL_QUADS);

        glColor3ub(r1, g1, b1);
        glVertex3d(x1, y1, z1 );

        glColor3ub(r2, g2, b2);
        glVertex3d(x2, y2, z2 );

        glColor3ub(r3, g3, b3);
        glVertex3d(x3, y3, z3 );

        glColor3ub(r4, g4, b4);
        glVertex3d(x4, y4, z4 );

        if(m_drawOnly == DRAW_ALL)
            glEnd();
     }
}

void G3DPainter::drawQuadFace(float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float x4, float y4, float z4)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
    {
        glBegin(GL_LINE_STRIP);

        glVertex3d(x1, y1, z1 );
        glVertex3d(x2, y2, z2 );
        glVertex3d(x3, y3, z3 );
        glVertex3d(x4, y4, z4 );

        glEnd();
    }
}

void G3DPainter::drawQuadFace(float x1, float y1, float z1, int r1, int g1, int b1,
                               float x2, float y2, float z2, int r2, int g2, int b2,
                               float x3, float y3, float z3, int r3, int g3, int b3,
                               float x4, float y4, float z4, int r4, int g4, int b4)
{
    if((m_drawOnly == DRAW_ALL) || (m_drawOnly == NO_MULTI_AVAILABLE))
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
}


//////////// PROTECTED ///////////

void G3DPainter::initPointShader()
{
    if(QT_GL_CONTEXT::currentContext() != NULL)
    {
        if(!m_shaderPointError && m_ShaderPoint == NULL)
        {
            m_ShaderPoint = new QT_GL_SHADER(QT_GL_SHADER::Vertex);

            if(!m_ShaderPoint->compileSourceFile("./shaders/points.vert"))
            {
                GUI_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("G3DPainter (points) => Vertex shader compilation error : %1").arg(m_ShaderPoint->log()));

                delete m_ShaderPoint;
                m_ShaderPoint = NULL;

                m_shaderPointError = true;
            }
        }

        if(!m_shaderPointError
                && !m_shaderProgPointError
                && m_shaderProgPoint->shaders().isEmpty())
        {
            m_shaderProgPointError = !m_shaderProgPoint->addShader(m_ShaderPoint);

            if(!m_shaderProgPointError && !m_shaderProgPoint->link())
            {
                GUI_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("G3DPainter (points) => Link error : %1").arg(m_shaderProgPoint->log()));
                m_shaderProgPointError = true;
            }
        }
    }
}

/*void G3DPainter::initFaceShader()
{
    if(QT_GL_CONTEXT::currentContext() != NULL)
    {
        if(!m_shaderFaceError && (m_ShaderFace == NULL))
        {
            m_ShaderFace = new QT_GL_SHADER(QT_GL_SHADER::Vertex);

            if(!m_ShaderFace->compileSourceFile("./shaders/faces.vert"))
            {
                GUI_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("G3DPainter (faces) => Vertex shader compilation error : %1").arg(m_ShaderFace->log()));

                delete m_ShaderFace;
                m_ShaderFace = NULL;

                m_shaderFaceError = true;
            }
        }

        if(!m_shaderFaceError
                && !m_shaderProgFaceError
                && m_shaderProgFace->shaders().isEmpty())
        {
            m_shaderProgFaceError = !m_shaderProgFace->addShader(m_ShaderFace);

            if(!m_shaderProgFaceError && !m_shaderProgFace->link())
            {
                GUI_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("G3DPainter (faces) => Link error : %1").arg(m_shaderProgFace->log()));
                m_shaderProgFaceError = true;
            }
        }
    }
}

void G3DPainter::initEdgeShader()
{
    if(QT_GL_CONTEXT::currentContext() != NULL)
    {
        if(!m_shaderEdgeError && (m_ShaderEdge == NULL))
        {
            m_ShaderEdge = new QT_GL_SHADER(QT_GL_SHADER::Vertex);

            if(!m_ShaderEdge->compileSourceFile("./shaders/edges.vert"))
            {
                GUI_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("G3DPainter (edges) => Vertex shader compilation error : %1").arg(m_ShaderEdge->log()));

                delete m_ShaderEdge;
                m_ShaderEdge = NULL;

                m_shaderEdgeError = true;
            }
        }

        if(!m_shaderEdgeError
                && !m_shaderProgEdgeError
                && m_shaderProgEdge->shaders().isEmpty())
        {
            m_shaderProgEdgeError = !m_shaderProgEdge->addShader(m_ShaderEdge);

            if(!m_shaderProgEdgeError && !m_shaderProgEdge->link())
            {
                GUI_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("G3DPainter (edges) => Link error : %1").arg(m_shaderProgEdge->log()));
                m_shaderProgEdgeError = true;
            }
        }
    }
}*/

bool G3DPainter::bindPointShader(bool force)
{
    if((m_drawOnly != POINT_CLOUD) || force)
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

                while(!(tmp = m_shaderProgPoint->log()).isEmpty())
                    log += tmp;

                if(!log.isEmpty())
                    GUI_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("G3DPainter (points) => Bind error : %1").arg(m_shaderProgPoint->log()));

                m_shaderProgPointError = true;
            }
            else
            {
                if(!m_shaderProgPointSet) {
                    if(m_gv->pointsInformationManager()->informations()->size() > 0)
                    {
                        QColor sColor = m_gv->getOptions().getSelectedColor();
                        m_shaderProgPoint->setUniformValue("selectionColor", QVector4D(sColor.redF(), sColor.greenF(), sColor.blueF(), sColor.alphaF()));

                        m_shaderProgPoint->setUniformValue("checkSelected", (GLuint)m_gv->pointsInformationManager()->checkSelected());

                        m_shaderProgPoint->enableAttributeArray("info");
                        int loc = m_shaderProgPoint->attributeLocation("info");
                        glVertexAttribPointer(loc, 1, GL_UNSIGNED_BYTE, GL_FALSE, 0, &m_gv->pointsInformationManager()->informations()->constTAt(0));

                        m_shaderProgPointSet = true;
                    }

                } else {
                    m_shaderProgPoint->enableAttributeArray("info");
                }

                return true;
            }
        }
    }

    return false;
}

void G3DPainter::releasePointShader(bool bindOk)
{
    if(bindOk)
    {
        m_shaderProgPoint->disableAttributeArray("info");
        m_shaderProgPoint->release();
    }
}

/*bool G3DPainter::bindFaceShader()
{
    initFaceShader();

    if(!m_shaderFaceError
            && !m_shaderProgFaceError
            && (QT_GL_CONTEXT::currentContext() != NULL))
    {
        if(!m_shaderProgFace->bind())
        {
            QString log;
            QString tmp;

            while(!(tmp = m_shaderProgFace->log()).isEmpty())
                log += tmp;

            if(!log.isEmpty())
                GUI_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("G3DPainter (faces) => Bind error : %1").arg(m_shaderProgFace->log()));

            m_shaderProgFaceError = true;
        }
        else
        {
            return true;
        }
    }

    return false;
}

void G3DPainter::releaseFaceShader(bool bindOk)
{
    if(bindOk)
        m_shaderProgFace->release();
}

bool G3DPainter::bindEdgeShader()
{
    initEdgeShader();

    if(!m_shaderEdgeError
            && !m_shaderProgEdgeError
            && (QT_GL_CONTEXT::currentContext() != NULL))
    {
        if(!m_shaderProgEdge->bind())
        {
            QString log;
            QString tmp;

            while(!(tmp = m_shaderProgEdge->log()).isEmpty())
                log += tmp;

            if(!log.isEmpty())
                GUI_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("G3DPainter (edges) => Bind error : %1").arg(m_shaderProgEdge->log()));

            m_shaderProgEdgeError = true;
        }
        else
        {
            return true;
        }
    }

    return false;
}

void G3DPainter::releaseEdgeShader(bool bindOk)
{
    if(bindOk)
        m_shaderProgEdge->release();
}*/

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

void G3DPainter::startDrawMultiple()
{
    if(!m_beginMultipleEnable)
    {
        // if we must only draw point or point cloud
        if((m_drawOnly == POINT) || (m_drawOnly == POINT_CLOUD))
        {
            if((m_drawOnly == POINT_CLOUD) && !m_bindShaderPointOK)
                m_bindShaderPointOK = bindPointShader(true);

            glBegin(GL_POINTS);

            m_beginMultipleEnable = true;
        }
        else if((m_drawOnly == TRIANGLE) || (m_drawOnly == FACE))
        {
            glBegin(GL_TRIANGLES);
            m_beginMultipleEnable = true;
        }
        else if((m_drawOnly == LINE) || (m_drawOnly == EDGE))
                {
            glBegin(GL_LINES);
            m_beginMultipleEnable = true;
        }
        else if(m_drawOnly == QUADS)
        {
            glBegin(GL_QUADS);
            m_beginMultipleEnable = true;
        }
    }

    // if we must draw other elements or all elements we don't call glBegin
}

void G3DPainter::stopDrawMultiple()
{
    if(m_beginMultipleEnable)
    {
        glEnd();
        m_beginMultipleEnable = false;
    }
}

void G3DPainter::resetDrawMultiple()
{
    // if we have draw a specific type we must call glEnd() before call a glBegin(...)
    if((m_drawOnly != NO_MULTI_AVAILABLE) && (m_drawOnly != DRAW_ALL))
    {
        if(m_beginMultipleEnable)
            glEnd();

        if(m_drawOnly == POINT_CLOUD)
            releasePointShader(m_bindShaderPointOK);

        m_bindShaderPointOK = false;
    }

    m_drawOnly = DRAW_ALL;
}
