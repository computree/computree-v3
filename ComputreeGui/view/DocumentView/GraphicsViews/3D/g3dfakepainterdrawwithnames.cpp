#include "g3dfakepainterdrawwithnames.h"

#include "dm_guimanager.h"

#include "ct_global/ct_context.h"
#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"
#include "ct_pointcloud/abstract/ct_abstractpointcloud.h"
#include "ct_itemdrawable/abstract/ct_abstractmeshmodel.h"
#include "ct_mesh/cloud/abstract/ct_abstractedgecloudindex.h"
#include "ct_mesh/cloud/abstract/ct_abstractfacecloudindex.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

G3DFakePainterDrawWithNames::G3DFakePainterDrawWithNames()
{
    m_gv = NULL;

    m_defaultPointSize = 1.0;

    beginNewDraw();
}

void G3DFakePainterDrawWithNames::setGraphicsView(const GraphicsViewInterface *gv)
{
    m_gv = (GraphicsViewInterface*)gv;
}

void G3DFakePainterDrawWithNames::beginNewDraw()
{
    m_drawMode = DrawNone;

    m_drawFastest = false;

    m_nCallEnableSetPointSize = 0;
    m_nCallEnableSetForcedPointSize = 0;

    m_drawMultipleLine = false;
    m_drawMultipleTriangle = false;

    m_nPoints = 0;
    m_nEdges = 0;
    m_nFaces = 0;

    m_pCloudIndexBackup.clear();
    m_fCloudIndexBackup.clear();
    m_eCloudIndexBackup.clear();
}

void G3DFakePainterDrawWithNames::setDrawMode(DrawModes mode)
{
    m_drawMode = mode;
}

bool G3DFakePainterDrawWithNames::drawFastest() const
{
    return m_drawFastest;
}

void G3DFakePainterDrawWithNames::setDrawFastest(bool enable)
{
    m_drawFastest = enable;
}

size_t G3DFakePainterDrawWithNames::nPoints() const
{
    return m_nPoints;
}

size_t G3DFakePainterDrawWithNames::nEdges() const
{
    return m_nEdges;
}

size_t G3DFakePainterDrawWithNames::nFaces() const
{
    return m_nFaces;
}

const QList<CT_AbstractCloudIndex *>& G3DFakePainterDrawWithNames::pointCloudIndexBackup() const
{
    return m_pCloudIndexBackup;
}

const QList<CT_AbstractCloudIndex *>& G3DFakePainterDrawWithNames::edgeCloudIndexBackup() const
{
    return m_eCloudIndexBackup;
}

const QList<CT_AbstractCloudIndex *>& G3DFakePainterDrawWithNames::faceCloudIndexBackup() const
{
    return m_fCloudIndexBackup;
}

void G3DFakePainterDrawWithNames::save()
{
    glPushMatrix();
}

void G3DFakePainterDrawWithNames::restore()
{
    glPopMatrix();
}

void G3DFakePainterDrawWithNames::startRestoreIdentityMatrix(GLdouble *matrix)
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

void G3DFakePainterDrawWithNames::stopRestoreIdentityMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void G3DFakePainterDrawWithNames::setPointSize(double size)
{
    if(m_nCallEnableSetPointSize == 0)
        glPointSize(size);
}

void G3DFakePainterDrawWithNames::setDefaultPointSize(double size)
{
    if(m_nCallEnableSetPointSize == 0)
        m_defaultPointSize = size;
}

void G3DFakePainterDrawWithNames::restoreDefaultPointSize()
{
    if(m_nCallEnableSetPointSize == 0)
        glPointSize(m_defaultPointSize);
}

void G3DFakePainterDrawWithNames::enableSetPointSize(bool enable)
{
    m_nCallEnableSetPointSize += (enable ? 1 : -1);
}

void G3DFakePainterDrawWithNames::enableSetForcedPointSize(bool enable)
{
    m_nCallEnableSetForcedPointSize += (enable ? 1 : -1);
}

void G3DFakePainterDrawWithNames::translate(double x, double y, double z)
{
    glTranslated(x, y, z);
}

void G3DFakePainterDrawWithNames::rotate(double alpha, double x, double y, double z)
{
    glRotated(alpha, x, y, z);
}

void G3DFakePainterDrawWithNames::scale(double x, double y, double z)
{
    glScaled(x, y, z);
}

void G3DFakePainterDrawWithNames::drawPointCloud(const CT_AbstractPointCloud *pc,
                                                 const CT_AbstractCloudIndex *pci,
                                                 int fastestIncrement)
{
    if(m_drawMode.testFlag(DrawPoints))
    {
        glColor3ub(255, 255, 255);

        size_t n = 0;
        size_t pIndex;
        size_t indexCount = pci->size();
        size_t increment = 1;

        if(drawFastest() && (fastestIncrement > 0))
            increment = fastestIncrement;

        // FAST
        if(increment != 1)
        {
            while(n < indexCount)
            {
                pci->indexAt(n, pIndex);

                glPushName(pIndex);

                glBegin(GL_POINTS);
                glVertex3fv(pc->constTAt(pIndex).vertex());
                glEnd();

                glPopName();

                n += increment;
            }
        }
        // NORMAL
        else
        {
            while(n < indexCount)
            {
                pci->indexAt(n, pIndex);

                glPushName(pIndex);

                glBegin(GL_POINTS);
                glVertex3fv(pc->constTAt(pIndex).vertex());
                glEnd();

                glPopName();

                ++n;
            }
        }
    }
    else if(m_drawMode.testFlag(CountPoints))
    {
        m_nPoints += pci->size();
    }
    else if(m_drawMode.testFlag(BackupPointCloudIndex))
    {
        if(!m_pCloudIndexBackup.contains((CT_AbstractCloudIndex*)pci))
            m_pCloudIndexBackup.append((CT_AbstractCloudIndex*)pci);
    }
}

void G3DFakePainterDrawWithNames::drawMesh(const CT_AbstractMeshModel *mesh)
{
    if(m_drawMode.testFlag(BackupPointCloudIndex)
            || m_drawMode.testFlag(BackupFaceCloudIndex)
            || m_drawMode.testFlag(BackupEdgeCloudIndex))
    {
        if(m_drawMode.testFlag(BackupPointCloudIndex))
        {
            if(mesh == NULL)
                return;

            const CT_AbstractPointCloudIndex *pci = mesh->getPointCloudIndex();

            if(!m_pCloudIndexBackup.contains((CT_AbstractPointCloudIndex*)pci))
                m_pCloudIndexBackup.append((CT_AbstractPointCloudIndex*)pci);
        }

        if(m_drawMode.testFlag(BackupFaceCloudIndex))
        {
            if(mesh == NULL)
                return;

            const CT_AbstractCloudIndex *fci = mesh->getFaceCloudIndex();

            if(!m_fCloudIndexBackup.contains((CT_AbstractCloudIndex*)fci))
                m_fCloudIndexBackup.append((CT_AbstractCloudIndex*)fci);
        }

        if(m_drawMode.testFlag(BackupEdgeCloudIndex))
        {
            if(mesh == NULL)
                return;

            const CT_AbstractCloudIndex *eci = mesh->getEdgeCloudIndex();

            if(!m_eCloudIndexBackup.contains((CT_AbstractCloudIndex*)eci))
                m_eCloudIndexBackup.append((CT_AbstractCloudIndex*)eci);
        }
    }
    else
    {
        drawFaces(mesh);
    }
}

void G3DFakePainterDrawWithNames::drawFaces(const CT_AbstractMeshModel *mesh)
{
    if(m_drawMode.testFlag(DrawFaces))
    {
        glColor3ub(255, 255, 255);

        if(mesh == NULL)
            return;

        const CT_AbstractCloudIndex *fci = mesh->getFaceCloudIndex();

        size_t fIndex;
        size_t size = fci->size();

        for(size_t n=0; n<size; ++n)
        {
            fci->indexAt(n, fIndex);

            glPushName(fIndex);
            ((CT_AbstractMeshModel*)mesh)->drawFaceAt(n, *m_gv, *this);
            glPopName();
        }
    }
    else if(m_drawMode.testFlag(CountFaces))
    {
        if(mesh == NULL)
            return;

        const CT_AbstractCloudIndex *fci = mesh->getFaceCloudIndex();

        m_nFaces += fci->size();
    }
    else if(m_drawMode.testFlag(BackupFaceCloudIndex))
    {
        drawMesh(mesh);
    }
}

void G3DFakePainterDrawWithNames::drawEdges(const CT_AbstractMeshModel *mesh)
{
    if(m_drawMode.testFlag(DrawEdges))
    {
        glColor3ub(255, 255, 255);

        if(mesh == NULL)
            return;

        const CT_AbstractCloudIndex *eci = mesh->getEdgeCloudIndex();

        size_t eIndex;
        size_t size = eci->size();

        for(size_t n=0; n<size; ++n)
        {
            eci->indexAt(n, eIndex);

            glPushName(eIndex);
            ((CT_AbstractMeshModel*)mesh)->drawEdgeAt(n, *m_gv, *this);
            glPopName();
        }
    }
    else if(m_drawMode.testFlag(CountEdges))
    {
        if(mesh == NULL)
            return;

        const CT_AbstractCloudIndex *eci = mesh->getEdgeCloudIndex();

        m_nEdges += eci->size();
    }
    else if(m_drawMode.testFlag(BackupEdgeCloudIndex))
    {
        drawMesh(mesh);
    }
}

void G3DFakePainterDrawWithNames::drawPoints(const CT_AbstractMeshModel *mesh, int fastestIncrement)
{
    if(m_drawMode.testFlag(BackupPointCloudIndex))
        drawMesh(mesh);
    else
        drawPointCloud(PS_REPOSITORY->globalPointCloud(), mesh->getPointCloudIndex(), fastestIncrement);
}

void G3DFakePainterDrawWithNames::beginDrawMultipleLine()
{
    glBegin(GL_LINES);
    m_drawMultipleLine = true;
}

void G3DFakePainterDrawWithNames::drawLine(double x1, double y1, double z1, double x2, double y2, double z2)
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

void G3DFakePainterDrawWithNames::drawLine(const float *p1, const float *p2)
{
    if(m_drawMultipleLine)
    {
        glVertex3fv(p1);
        glVertex3fv(p2);
    }
    else
    {
        glBegin(GL_LINES);

        glVertex3fv(p1);
        glVertex3fv(p2);

        glEnd();
    }
}

void G3DFakePainterDrawWithNames::endDrawMultipleLine()
{
    glEnd();
    m_drawMultipleLine = false;
}

void G3DFakePainterDrawWithNames::beginDrawMultipleTriangle()
{
    glBegin( GL_TRIANGLES );
    m_drawMultipleTriangle = true;
}

void G3DFakePainterDrawWithNames::drawTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3)
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

void G3DFakePainterDrawWithNames::drawTriangle(const float *p1, const float *p2, const float *p3)
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

void G3DFakePainterDrawWithNames::endDrawMultipleTriangle()
{
    glEnd();
    m_drawMultipleTriangle = false;
}
