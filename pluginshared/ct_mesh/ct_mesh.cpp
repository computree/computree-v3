#include "ct_mesh.h"

#include "ct_cloud/tools/abstract/ct_abstractglobalcloudmanagert.h"
#include "ct_mesh/tools/ct_meshallocatort.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <math.h>
#endif

CT_Mesh::CT_Mesh()
{
    m_pVert = NULL;
    m_pFace = NULL;
    m_pHedge = NULL;
}

CT_Mesh::~CT_Mesh()
{
    clear();
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
    if(m_vert == NULL)
        return NULL;

    return m_vert->abstractModifiableCloudIndexT();
}

CT_AbstractModifiableCloudIndexT<CT_Face>* CT_Mesh::pFace() const
{
    if(m_face == NULL)
        return NULL;

    return m_face->abstractModifiableCloudIndexT();
}

CT_AbstractModifiableCloudIndexT<CT_Edge>* CT_Mesh::pHedge() const
{
    if(m_hedge == NULL)
        return NULL;

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

void CT_Mesh::createCylinder(double radius, double height, int sides)
{
    clear();

    double inc = (2.0 * M_PI) / ((double)sides);
    double theta = 0;

    VertexIndexIterator vi = CT_MeshAllocatorT<CT_Mesh>::AddVertices(*this, 2*sides);
    FaceIndexIterator fi = CT_MeshAllocatorT<CT_Mesh>::AddFaces(*this, 2*sides);
    HEdgeIndexIterator ei = CT_MeshAllocatorT<CT_Mesh>::AddHEdges(*this, 6*sides);

    double minH = 0;
    double maxH = height;

    CT_Point &v = vi.cT();
    v(0) = maxH;
    v(1) = radius;
    v(2) = 0;

    size_t p = vi.cIndex();

    for(size_t i=0; i<sides; ++i)
    {
        CT_Point &v0 = vi.cT();
        size_t p0 = vi.cIndex();
        ++vi;

        CT_Point &v1 = vi.cT();
        size_t p1 = vi.cIndex();
        ++vi;

        v1(0) = minH;
        v1(1) = v0(1);
        v1(2) = v0(2);

        theta += inc;

        size_t p2;
        size_t p3;

        if(i<sides-1)
        {
            CT_Point &v2 = vi.cT();
            p2 = vi.cIndex();
            p3 = p2+1;

            v2(0) = maxH;
            v2(1) = radius * cos(theta);
            v2(2) = radius * sin(theta);
        }
        else
        {
            p2 = p;
            p3 = p+1;
        }

        CT_Face &face1 = fi.cT();
        size_t faceIndex = fi.cIndex();

        size_t e1Index = ei.cIndex();
        size_t e2Index;
        size_t e3Index;

        face1.setEdge(e1Index);

        CT_Edge &e1 = ei.cT();
        e1.setPoint0(p0);
        e1.setPoint1(p1);
        e1.setFace(faceIndex);
        ++ei;

        CT_Edge &e2 = ei.cT();
        e2.setPoint0(p1);
        e2.setPoint1(p2);
        e1.setFace(faceIndex);
        e2Index = ei.cIndex();
        ++ei;

        CT_Edge &e3 = ei.cT();
        e3.setPoint0(p2);
        e3.setPoint1(p0);
        e3.setFace(faceIndex);
        e3Index = ei.cIndex();
        ++ei;

        e1.setNext(e2Index);
        e1.setPrevious(e3Index);
        e2.setNext(e3Index);
        e2.setPrevious(e1Index);
        e3.setNext(e1Index);
        e3.setPrevious(e2Index);

        ++fi;

        CT_Face &face2 = fi.cT();
        faceIndex = fi.cIndex();

        e1Index = ei.cIndex();

        face2.setEdge(e1Index);

        CT_Edge &e11 = ei.cT();
        e11.setPoint0(p1);
        e11.setPoint1(p3);
        e11.setFace(faceIndex);
        ++ei;

        CT_Edge &e22 = ei.cT();
        e22.setPoint0(p3);
        e22.setPoint1(p2);
        e11.setFace(faceIndex);
        e2Index = ei.cIndex();
        ++ei;

        CT_Edge &e33 = ei.cT();
        e33.setPoint0(p2);
        e33.setPoint1(p1);
        e33.setFace(faceIndex);
        e3Index = ei.cIndex();
        ++ei;

        e11.setNext(e2Index);
        e11.setPrevious(e3Index);
        e22.setNext(e3Index);
        e22.setPrevious(e1Index);
        e33.setNext(e1Index);
        e33.setPrevious(e2Index);

        ++fi;
    }
}

void CT_Mesh::clear()
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
