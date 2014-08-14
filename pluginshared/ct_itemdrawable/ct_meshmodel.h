#ifndef CT_MESHMODEL_H
#define CT_MESHMODEL_H

#include "ct_itemdrawable/abstract/ct_abstractmeshmodel.h"

#include "ct_itemdrawable/tools/drawmanager/ct_standardmeshmodeldrawmanager.h"
#include "ct_mesh/ct_mesh.h"

/**
 * @brief A singular item that can manage a CT_Mesh
 */
class PLUGINSHAREDSHARED_EXPORT CT_MeshModel : public CT_AbstractMeshModel
{
    // IMPORTANT to have the name of the ItemDrawable in the view
    Q_OBJECT

public:
    CT_MeshModel();
    CT_MeshModel(const CT_OutAbstractItemModel *model,
                 const CT_AbstractResult *result);
    CT_MeshModel(const CT_OutAbstractItemModel *model,
                 const CT_AbstractResult *result,
                 CT_Mesh *mesh);

    CT_MeshModel(const QString &modelName,
                 const CT_AbstractResult *result);
    CT_MeshModel(const QString &modelName,
                 const CT_AbstractResult *result,
                 CT_Mesh *mesh);

    ~CT_MeshModel();

    /**
      * WARNING : don't forget to redefined this methods if you inherit this class
      */
    virtual QString getType() const;
    static QString staticGetType();

    /**
     * @brief Define if the mesh must be automatically deleted from memory. (true by default)
     */
    void setAutoDeleteMesh(bool enable);

    /**
     * @brief Return true if the mesh must be automatically deleted from memory
     */
    bool autoDeleteMesh() const;

    /**
     * @brief Set the mesh. If a mesh already exist it will be deleted from memory.
     */
    void setMesh(CT_Mesh *mesh);

    /**
     * @brief Getter for mesh
     */
    CT_Mesh* mesh() const;

    /**
     * @brief Getter for mesh. (Same as MeshLab => not a attribute but a method)
     */
    CT_Mesh& cm();

    /**
     * @brief Clear mesh (delete from memory if method "autoDeleteMesh" returns true)
     */
    void clearMesh();

    /**
     * @brief Returns a copy of this item
     */
    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    /**
     * @brief Draw only faces
     */
    void drawFaces(GraphicsViewInterface &view, PainterInterface &painter);

    /**
     * @brief Begin draw multiple faces
     */
    void beginDrawMultipleFace(GraphicsViewInterface &view, PainterInterface &painter);

    /**
     * @brief Draw only face at index 'index'
     */
    void drawFaceAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter);

    /**
     * @brief Finish draw multiple faces
     */
    void endDrawMultipleFace(GraphicsViewInterface &view, PainterInterface &painter);

    /**
     * @brief Draw only edges
     */
    void drawEdges(GraphicsViewInterface &view, PainterInterface &painter);

    /**
     * @brief Begin draw multiple edges
     */
    void beginDrawMultipleEdge(GraphicsViewInterface &view, PainterInterface &painter);

    /**
     * @brief Draw only edge at index 'index'
     */
    void drawEdgeAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter);

    /**
     * @brief Finish draw multiple edges
     */
    void endDrawMultipleEdge(GraphicsViewInterface &view, PainterInterface &painter);

    /**
     * @brief Returns the point cloud index
     */
    const CT_AbstractPointCloudIndex* getPointCloudIndex() const;

    /**
     * @brief Returns the face cloud index
     */
    const CT_AbstractFaceCloudIndex* getFaceCloudIndex() const;

    /**
     * @brief Returns the edge cloud index
     */
    const CT_AbstractEdgeCloudIndex* getEdgeCloudIndex() const;

protected:

    const static CT_StandardMeshModelDrawManager CT_MESHMODEL_DRAW_MANAGER;

private:
    CT_Mesh *m_mesh;
    bool    m_autoDeleteMesh;
};

#endif // CT_MESHMODEL_H
