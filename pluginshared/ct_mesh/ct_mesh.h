#ifndef CT_MESH_H
#define CT_MESH_H

#include "pluginShared_global.h"

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"

#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"
#include "ct_mesh/cloud/abstract/ct_abstractedgecloudindex.h"
#include "ct_mesh/cloud/abstract/ct_abstractfacecloudindex.h"

/**
 * A Mesh. You must use a CT_MeshAllocatorT to add Point or Face or Edge to a CT_Mesh.
 */
class PLUGINSHAREDSHARED_EXPORT CT_Mesh
{
public:
    typedef CT_Repository::VertexIndexIterator      VertexIndexIterator;
    typedef CT_Repository::FaceIndexIterator        FaceIndexIterator;
    typedef CT_Repository::EdgeIndexIterator        HEdgeIndexIterator;

    typedef CT_Point*                               VertexPointer;
    typedef CT_Face*                                FacePointer;
    typedef CT_Edge*                                HEdgePointer;

    CT_Mesh();
    ~CT_Mesh();

    /**
     * @brief vert, face, hedge is the name used in the class TriMesh of vcg
     *        and the attributes is public. To help users to migrate we decided to do
     *        the same thing but use functions instead of public attributes.
     */
    CT_AbstractModifiableCloudIndexT<CT_Point>& vert() const;
    CT_AbstractModifiableCloudIndexT<CT_Face>& face() const;
    CT_AbstractModifiableCloudIndexT<CT_Edge>& hedge() const;

    CT_AbstractModifiableCloudIndexT<CT_Point>* pVert() const;
    CT_AbstractModifiableCloudIndexT<CT_Face>* pFace() const;
    CT_AbstractModifiableCloudIndexT<CT_Edge>* pHedge() const;

    CT_AbstractPointCloudIndex* abstractVert() const;
    CT_AbstractFaceCloudIndex* abstractFace() const;
    CT_AbstractEdgeCloudIndex* abstractHedge() const;

    CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractModifiableCIR registeredVert() const;
    CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractModifiableCIR registeredFace() const;
    CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractModifiableCIR registeredHedge() const;

protected:

    template<typename MeshType> friend class CT_MeshAllocatorT;

    /**
     * @brief set of vertices (index)
     */
    CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractModifiableCIR m_vert;
    CT_AbstractPointCloudIndex *m_pVert;    // used by CT_MeshAllocatorT to avoid dynamic_cast

    /**
     * @brief set of face (index)
     */
    CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractModifiableCIR  m_face;
    CT_AbstractFaceCloudIndex *m_pFace;    // used by CT_MeshAllocatorT to avoid dynamic_cast

    /**
     * @brief set of hedge (index)
     */
    CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractModifiableCIR  m_hedge;
    CT_AbstractEdgeCloudIndex *m_pHedge;    // used by CT_MeshAllocatorT to avoid dynamic_cast

    /**
     * @brief a collection of vertices created in the global points cloud
     */
    std::vector< CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractNotModifiableCIR > m_newVert;

    /**
     * @brief a collection of faces created in the global faces cloud
     */
    std::vector< CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractNotModifiableCIR > m_newFace;

    /**
     * @brief a collection of edges created in the global edges cloud
     */
    std::vector< CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractNotModifiableCIR > m_newEdge;
};

#endif // CT_MESH_H
