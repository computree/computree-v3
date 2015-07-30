#include "dm_fakepainter.h"

#include "ct_itemdrawable/abstract/ct_abstractmeshmodel.h"

DM_FakePainter::DM_FakePainter()
{
    m_mode = Nothing;
    m_nPoints = 0;
    m_nEdges = 0;
    m_nFaces = 0;

    m_pCloudIndexBackup.append(&m_pIndexBackup);
}

void DM_FakePainter::setComputingMode(ComputingModes modes)
{
    m_mode = modes;
}

size_t DM_FakePainter::nPoints() const
{
    return m_nPoints;
}

size_t DM_FakePainter::nEdges() const
{
    return m_nEdges;
}

size_t DM_FakePainter::nFaces() const
{
    return m_nFaces;
}

const QList<CT_AbstractCloudIndex *> &DM_FakePainter::pointCloudIndexBackup() const
{
    return m_pCloudIndexBackup;
}

const QList<CT_AbstractCloudIndex *> &DM_FakePainter::edgeCloudIndexBackup() const
{
    return m_eCloudIndexBackup;
}

const QList<CT_AbstractCloudIndex *> &DM_FakePainter::faceCloudIndexBackup() const
{
    return m_fCloudIndexBackup;
}

void DM_FakePainter::drawPoint(const size_t &globalIndex)
{
    if(m_mode.testFlag(CountPoints))
        ++m_nPoints;

    if(m_mode.testFlag(BackupPointCloudIndex))
        m_pIndexBackup.addIndex(globalIndex);
}

void DM_FakePainter::drawPoints(const CT_AbstractMeshModel *mesh)
{
    drawPointCloud(mesh->getPointCloudIndex());
}

void DM_FakePainter::drawPointCloud(const CT_AbstractCloudIndex *pci)
{
    if(pci == NULL)
        return;

    if(m_mode.testFlag(CountPoints))
        m_nPoints += pci->size();

    if(m_mode.testFlag(BackupPointCloudIndex))
        addPointIndexes(pci);
}

void DM_FakePainter::drawOctreeOfPoints(const OctreeInterface *octree, PainterInterface::DrawOctreeModes modes)
{
    // TODO : to remove if octree was not used
}

void DM_FakePainter::drawEdges(const CT_AbstractMeshModel *mesh)
{
    if(mesh == NULL)
        return;

    const CT_AbstractCloudIndex *eci = mesh->getEdgeCloudIndex();

    if(eci == NULL)
        return;

    if(m_mode.testFlag(CountEdges))
        m_nEdges += eci->size();
    else if(m_mode.testFlag(BackupEdgeCloudIndex))
        addEdgeIndexes(eci);
    else if(m_mode.testFlag(BackupPointCloudIndexIfEdge) && (mesh->getPointCloudIndex() != NULL))
        addPointIndexes(mesh->getPointCloudIndex());
}

void DM_FakePainter::drawMesh(const CT_AbstractMeshModel *mesh)
{
    drawPoints(mesh);
    drawEdges(mesh);
    drawFaces(mesh);
}

void DM_FakePainter::drawFaces(const CT_AbstractMeshModel *mesh)
{
    if(mesh == NULL)
        return;

    const CT_AbstractCloudIndex *fci = mesh->getFaceCloudIndex();

    if(fci == NULL)
        return;

    if(m_mode.testFlag(CountEdges))
        m_nFaces += fci->size();
    else if(m_mode.testFlag(BackupEdgeCloudIndex))
        addFaceIndexes(fci);
    else if(m_mode.testFlag(BackupPointCloudIndexIfFace) && (mesh->getPointCloudIndex() != NULL))
        addPointIndexes(mesh->getPointCloudIndex());
}

void DM_FakePainter::addPointIndexes(const CT_AbstractCloudIndex *pci)
{
    if(!m_pCloudIndexBackup.contains((CT_AbstractCloudIndex*)pci))
        m_pCloudIndexBackup.append((CT_AbstractCloudIndex*)pci);
}

void DM_FakePainter::addEdgeIndexes(const CT_AbstractCloudIndex *eci)
{
    if(!m_eCloudIndexBackup.contains((CT_AbstractCloudIndex*)eci))
        m_eCloudIndexBackup.append((CT_AbstractCloudIndex*)eci);
}

void DM_FakePainter::addFaceIndexes(const CT_AbstractCloudIndex *fci)
{
    if(!m_fCloudIndexBackup.contains((CT_AbstractCloudIndex*)fci))
        m_fCloudIndexBackup.append((CT_AbstractCloudIndex*)fci);
}
