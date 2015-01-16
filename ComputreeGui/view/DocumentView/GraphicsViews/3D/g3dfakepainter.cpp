#include "g3dfakepainter.h"

#include "dm_guimanager.h"

#include "view/DocumentView/GraphicsViews/3D/g3dgraphicsview.h"

#include "tools/graphicsview/dm_elementinfomanager.h"

#include "ct_global/ct_context.h"
#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"
#include "ct_pointcloud/abstract/ct_abstractpointcloud.h"
#include "ct_itemdrawable/abstract/ct_abstractmeshmodel.h"
#include "ct_mesh/cloud/abstract/ct_abstractedgecloudindex.h"
#include "ct_mesh/cloud/abstract/ct_abstractfacecloudindex.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

G3DFakePainter::G3DFakePainter() : G3DPainter()
{
    beginNewDraw();
}

void G3DFakePainter::setApplyColor(const QColor &color)
{
    m_applyColor = color;
}

void G3DFakePainter::setPointsColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> pColors)
{
    m_pColors = pColors;
}

void G3DFakePainter::setEdgesColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> eColors)
{
    m_eColors = eColors;
}

void G3DFakePainter::setFacesColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> fColors)
{
    m_fColors = fColors;
}

void G3DFakePainter::beginNewDraw()
{
    G3DPainter::beginNewDraw();

    m_drawEnabled = true;
    m_drawMode = DrawNone;

    m_drawMultipleLine = false;
    m_drawMultipleTriangle = false;

    m_nPoints = 0;
    m_nEdges = 0;
    m_nFaces = 0;

    m_pCloudIndexBackup.clear();
    m_fCloudIndexBackup.clear();
    m_eCloudIndexBackup.clear();
}

void G3DFakePainter::endNewDraw()
{
    G3DPainter::endNewDraw();

    stopDrawMultiple();
}

void G3DFakePainter::setDrawMode(DrawModes mode)
{
    m_drawMode = mode;

    m_drawEnabled = (m_drawMode.testFlag(DrawPointsWithName)
                        || m_drawMode.testFlag(DrawEdgesWithName)
                        || m_drawMode.testFlag(DrawFacesWithName));
}

size_t G3DFakePainter::nPoints() const
{
    return m_nPoints;
}

size_t G3DFakePainter::nEdges() const
{
    return m_nEdges;
}

size_t G3DFakePainter::nFaces() const
{
    return m_nFaces;
}

const QList<CT_AbstractCloudIndex *>& G3DFakePainter::pointCloudIndexBackup() const
{
    return m_pCloudIndexBackup;
}

const QList<CT_AbstractCloudIndex *>& G3DFakePainter::edgeCloudIndexBackup() const
{
    return m_eCloudIndexBackup;
}

const QList<CT_AbstractCloudIndex *>& G3DFakePainter::faceCloudIndexBackup() const
{
    return m_fCloudIndexBackup;
}

void G3DFakePainter::save()
{
    if(m_drawEnabled)
        G3DPainter::save();
}

void G3DFakePainter::restore()
{
    if(m_drawEnabled)
        G3DPainter::restore();
}

void G3DFakePainter::startRestoreIdentityMatrix(GLdouble *matrix)
{
    if(m_drawEnabled)
        G3DPainter::startRestoreIdentityMatrix(matrix);
}

void G3DFakePainter::stopRestoreIdentityMatrix()
{
    if(m_drawEnabled)
        G3DPainter::stopRestoreIdentityMatrix();
}

void G3DFakePainter::pushMatrix()
{
    if(m_drawEnabled)
        G3DPainter::pushMatrix();
}

void G3DFakePainter::multMatrix(const Eigen::Matrix4d &matrix)
{
    if(m_drawEnabled)
        G3DPainter::multMatrix(matrix);
}

void G3DFakePainter::popMatrix()
{
    if(m_drawEnabled)
        G3DPainter::popMatrix();
}

void G3DFakePainter::setPointSize(float size)
{
    if(m_drawEnabled)
        G3DPainter::setPointSize(size);
}

void G3DFakePainter::setDefaultPointSize(float size)
{
    if(m_drawEnabled)
        G3DPainter::setDefaultPointSize(size);
}

void G3DFakePainter::restoreDefaultPointSize()
{
    if(m_drawEnabled)
        G3DPainter::restoreDefaultPointSize();
}

void G3DFakePainter::translate(const double &x, const double &y, const double &z)
{
    if(m_drawEnabled)
        G3DPainter::translate(x, y, z);
}

void G3DFakePainter::rotate(const double &alpha, const double &x, const double &y, const double &z)
{
    if(m_drawEnabled)
        G3DPainter::rotate(alpha, x, y, z);
}

void G3DFakePainter::translateThenRotateToDirection(const Eigen::Vector3d &translation, const Eigen::Vector3d &direction)
{
    if(m_drawEnabled)
        G3DPainter::translateThenRotateToDirection(translation, direction);
}

void G3DFakePainter::scale(const double & x, const double & y, const double & z)
{
    if(m_drawEnabled)
        G3DPainter::scale(x, y, z);
}

void G3DFakePainter::drawPoint(const size_t &globalIndex)
{
    if(m_drawMode.testFlag(DrawPointsWithName))
    {
        DM_ElementInfoManager *infos = graphicsView()->pointsInformationManager();

        if(!infos->inlineIsInvisible(globalIndex)) {
            glColor3ub(255, 255, 255);

            glPushName(globalIndex);

            startDrawMultiple(GL_BEGIN_POINT_FROM_PC);
            glArrayElement(globalIndex);
            stopDrawMultiple(false);

            glPopName();
        }
    }
    else if(m_drawMode.testFlag(CountPoints))
    {
        ++m_nPoints;
    }
    else if(m_drawMode.testFlag(ApplyColorPoints))
    {
        CT_StandardColorCloudRegistered *scc = m_pColors.data();

        if(scc != NULL)
        {
            CT_AbstractColorCloud *cc = scc->abstractColorCloud();

            int r = m_applyColor.red();
            int g = m_applyColor.green();
            int b = m_applyColor.blue();
            int a = m_applyColor.alpha();

            CT_Color &col = cc->colorAt(globalIndex);

            col.r = r;
            col.g = g;
            col.b = b;
            col.a = a;
        }
    }
}

void G3DFakePainter::drawOctreeOfPoints(const OctreeInterface *octree, PainterInterface::DrawOctreeModes modes)
{
    if(!modes.testFlag(DrawElements) || (graphicsView() == NULL))
        return;

    GLdouble planeCoefficients[6][4];
    graphicsView()->getCameraFrustumPlanesCoefficients(planeCoefficients);

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
                    if(octree->isCellVisibleInFrustrum(x, y, z, planeCoefficients))
                        drawPointCloud(indexes);
                }
            }
        }
    }
}

void G3DFakePainter::drawPointCloud(const CT_AbstractCloudIndex *pci)
{
    if(pci == NULL)
        return;

    if(m_drawMode.testFlag(DrawPointsWithName))
    {
        glColor3ub(255, 255, 255);

        size_t n = 0;
        size_t pIndex;
        size_t indexCount = pci->size();
        size_t increment = 1;

        if((pointFastestIncrement() != 0) && drawFastest())
            increment = pointFastestIncrement();

        DM_ElementInfoManager *infos = graphicsView()->pointsInformationManager();

        // FAST
        if(increment != 1)
        {
            while(n < indexCount)
            {
                pci->indexAt(n, pIndex);

                if(!infos->inlineIsInvisible(pIndex))
                {
                    glPushName(pIndex);

                    startDrawMultiple(GL_BEGIN_POINT_FROM_PC);
                    glArrayElement(pIndex);
                    stopDrawMultiple(false);

                    glPopName();
                }

                n += increment;
            }
        }
        // NORMAL
        else
        {
            while(n < indexCount)
            {
                pci->indexAt(n, pIndex);

                if(!infos->inlineIsInvisible(pIndex))
                {
                    glPushName(pIndex);

                    startDrawMultiple(GL_BEGIN_POINT_FROM_PC);
                    glArrayElement(pIndex);
                    stopDrawMultiple(false);

                    glPopName();
                }

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
    else if(m_drawMode.testFlag(ApplyColorPoints))
    {
        CT_StandardColorCloudRegistered *scc = m_pColors.data();

        if(scc != NULL)
        {
            CT_AbstractColorCloud *cc = scc->abstractColorCloud();

            int r = m_applyColor.red();
            int g = m_applyColor.green();
            int b = m_applyColor.blue();
            int a = m_applyColor.alpha();

            size_t n = 0;
            size_t pIndex;
            size_t indexCount = pci->size();

            while(n < indexCount)
            {
                pci->indexAt(n, pIndex);
                CT_Color &col = cc->colorAt(pIndex);

                col.r = r;
                col.g = g;
                col.b = b;
                col.a = a;

                ++n;
            }
        }
    }
}

void G3DFakePainter::drawMesh(const CT_AbstractMeshModel *mesh)
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

void G3DFakePainter::drawFaces(const CT_AbstractMeshModel *mesh)
{
    if(m_drawMode.testFlag(DrawFacesWithName))
    {
        glColor3ub(255, 255, 255);

        if(mesh == NULL)
            return;

        const CT_AbstractCloudIndex *fci = mesh->getFaceCloudIndex();

        if(fci == NULL)
            return;

        DM_ElementInfoManager *infos = graphicsView()->facesInformationManager();

        size_t fIndex;
        size_t size = fci->size();

        for(size_t n=0; n<size; ++n)
        {
            fci->indexAt(n, fIndex);

            if(!infos->inlineIsInvisible(fIndex))
            {
                glPushName(fIndex);

                startDrawMultiple(GL_BEGIN_TRIANGLE_FROM_PC);

                const CT_Face &face = faceCloud()->constTAt(fIndex);
                glArrayElement(face.iPointAt(0));
                glArrayElement(face.iPointAt(1));
                glArrayElement(face.iPointAt(2));

                stopDrawMultiple(false);

                glPopName();
            }
        }
    }
    else if(m_drawMode.testFlag(CountFaces))
    {
        if(mesh == NULL)
            return;

        const CT_AbstractCloudIndex *fci = mesh->getFaceCloudIndex();

        if(fci == NULL)
            return;

        m_nFaces += fci->size();
    }
    else if(m_drawMode.testFlag(BackupFaceCloudIndex))
    {
        drawMesh(mesh);
    }
    else if(m_drawMode.testFlag(ApplyColorFaces))
    {
        CT_StandardColorCloudRegistered *scc = m_fColors.data();

        if(scc != NULL)
        {
            const CT_AbstractCloudIndex *fci = mesh->getFaceCloudIndex();

            if(fci == NULL)
                return;

            CT_AbstractColorCloud *cc = scc->abstractColorCloud();

            int r = m_applyColor.red();
            int g = m_applyColor.green();
            int b = m_applyColor.blue();
            int a = m_applyColor.alpha();

            size_t n = 0;
            size_t fIndex;
            size_t indexCount = fci->size();

            while(n < indexCount)
            {
                fci->indexAt(n, fIndex);
                CT_Color &col = cc->colorAt(fIndex);

                col.r = r;
                col.g = g;
                col.b = b;
                col.a = a;

                ++n;
            }
        }
    }
}

void G3DFakePainter::drawEdges(const CT_AbstractMeshModel *mesh)
{
    if(m_drawMode.testFlag(DrawEdgesWithName))
    {
        glColor3ub(255, 255, 255);

        if(mesh == NULL)
            return;

        const CT_AbstractCloudIndex *eci = mesh->getEdgeCloudIndex();

        if(eci == NULL)
            return;

        DM_ElementInfoManager *infos = graphicsView()->edgesInformationManager();

        size_t eIndex;
        size_t size = eci->size();

        for(size_t n=0; n<size; ++n)
        {
            eci->indexAt(n, eIndex);

            if(!infos->inlineIsInvisible(eIndex))
            {
                glPushName(eIndex);

                startDrawMultiple(GL_BEGIN_LINE_FROM_PC);

                const CT_Edge &edge = edgeCloud()->constTAt(eIndex);
                glArrayElement(edge.iPointAt(0));
                glArrayElement(edge.iPointAt(1));

                stopDrawMultiple(false);

                glPopName();
            }
        }
    }
    else if(m_drawMode.testFlag(CountEdges))
    {
        if(mesh == NULL)
            return;

        const CT_AbstractCloudIndex *eci = mesh->getEdgeCloudIndex();

        if(eci == NULL)
            return;

        m_nEdges += eci->size();
    }
    else if(m_drawMode.testFlag(BackupEdgeCloudIndex))
    {
        drawMesh(mesh);
    }
    else if(m_drawMode.testFlag(ApplyColorEdges))
    {
        CT_StandardColorCloudRegistered *scc = m_eColors.data();

        if(scc != NULL)
        {
            const CT_AbstractCloudIndex *eci = mesh->getEdgeCloudIndex();

            if(eci == NULL)
                return;

            CT_AbstractColorCloud *cc = scc->abstractColorCloud();

            int r = m_applyColor.red();
            int g = m_applyColor.green();
            int b = m_applyColor.blue();
            int a = m_applyColor.alpha();

            size_t n = 0;
            size_t eIndex;
            size_t indexCount = eci->size();

            while(n < indexCount)
            {
                eci->indexAt(n, eIndex);
                CT_Color &col = cc->colorAt(eIndex);

                col.r = r;
                col.g = g;
                col.b = b;
                col.a = a;

                ++n;
            }
        }
    }
}

void G3DFakePainter::drawPoints(const CT_AbstractMeshModel *mesh)
{
    if(m_drawMode.testFlag(BackupPointCloudIndex))
        drawMesh(mesh);
    else
        drawPointCloud(mesh->getPointCloudIndex());
}

void G3DFakePainter::drawLine(const size_t &p1GlobalIndex, const size_t &p2GlobalIndex)
{
    if(m_drawEnabled)
    {
        startDrawMultiple(GL_BEGIN_LINE_FROM_PC);

        glArrayElement(p1GlobalIndex);
        glArrayElement(p2GlobalIndex);

        stopDrawMultiple(false);
    }
}

void G3DFakePainter::drawTriangle(const size_t &p1GlobalIndex, const size_t &p2GlobalIndex, const size_t &p3GlobalIndex)
{
    if(m_drawEnabled)
    {
        startDrawMultiple(GL_BEGIN_TRIANGLE_FROM_PC);

        glArrayElement(p1GlobalIndex);
        glArrayElement(p2GlobalIndex);
        glArrayElement(p3GlobalIndex);

        stopDrawMultiple(false);
    }
}
