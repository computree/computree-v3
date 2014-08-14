#include "ct_mesh.h"

#include "ct_cloud/tools/abstract/ct_abstractglobalcloudmanagert.h"

CT_Mesh::CT_Mesh()
{
    m_pVert = NULL;
    m_pFace = NULL;
    m_pHedge = NULL;
}

CT_Mesh::~CT_Mesh()
{
    // clear all index so it will not be sync when we
    // clear ege, face, point that have been new created
    m_vert.clear();
    m_face.clear();
    m_hedge.clear();

    // disable sync of all edge created
    size_t s = m_newEdge.size();

    for(size_t i=0; i<s; ++i)
        PS_REPOSITORY->globalCloudManager<CT_Edge>()->setEnableSyncForCIR(m_newEdge[i].data(), false);

    // disable sync of all face created
    s = m_newFace.size();

    for(size_t i=0; i<s; ++i)
        PS_REPOSITORY->globalCloudManager<CT_Face>()->setEnableSyncForCIR(m_newFace[i].data(), false);

    // clear edge
    s = m_newEdge.size();

    for(size_t i=0; i<s; ++i)
        m_newEdge[i].clear();

    // clear face
    s = m_newFace.size();

    for(size_t i=0; i<s; ++i)
        m_newFace[i].clear();

    // clear point
    s = m_newVert.size();

    for(size_t i=0; i<s; ++i)
        m_newVert[i].clear();
}

CT_AbstractModifiableCloudIndexT<CT_Point>& CT_Mesh::vert() const
{
    return *pVert();
}

CT_AbstractModifiableCloudIndexT<CT_Face>& CT_Mesh::face() const
{
    return *pFace();
}

CT_AbstractModifiableCloudIndexT<CT_Edge>& CT_Mesh::hedge() const
{
    return *pHedge();
}

CT_AbstractModifiableCloudIndexT<CT_Point>* CT_Mesh::pVert() const
{
    return m_vert->abstractModifiableCloudIndexT();
}

CT_AbstractModifiableCloudIndexT<CT_Face>* CT_Mesh::pFace() const
{
    return m_face->abstractModifiableCloudIndexT();
}

CT_AbstractModifiableCloudIndexT<CT_Edge>* CT_Mesh::pHedge() const
{
    return m_hedge->abstractModifiableCloudIndexT();
}

CT_AbstractPointCloudIndex* CT_Mesh::abstractVert() const
{
    return m_pVert;
}

CT_AbstractFaceCloudIndex* CT_Mesh::abstractFace() const
{
    return m_pFace;
}

CT_AbstractEdgeCloudIndex* CT_Mesh::abstractHedge() const
{
    return m_pHedge;
}

CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractModifiableCIR CT_Mesh::registeredVert() const
{
    return m_vert;
}

CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractModifiableCIR CT_Mesh::registeredFace() const
{
    return m_face;
}

CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractModifiableCIR CT_Mesh::registeredHedge() const
{
    return m_hedge;
}
