#include "ct_meshmodel.h"

const CT_StandardMeshModelDrawManager CT_MeshModel::CT_MESHMODEL_DRAW_MANAGER;

CT_MeshModel::CT_MeshModel() : CT_AbstractMeshModel()
{
    m_mesh = NULL;
    m_autoDeleteMesh = true;

    setBaseDrawManager(&CT_MESHMODEL_DRAW_MANAGER);
}

CT_MeshModel::CT_MeshModel(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result) : CT_AbstractMeshModel(model, result)
{
    m_mesh = NULL;
    m_autoDeleteMesh = true;

    setBaseDrawManager(&CT_MESHMODEL_DRAW_MANAGER);
}

CT_MeshModel::CT_MeshModel(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_Mesh *mesh) : CT_AbstractMeshModel(model, result)
{
    m_mesh = mesh;
    m_autoDeleteMesh = true;

    setBaseDrawManager(&CT_MESHMODEL_DRAW_MANAGER);
}

CT_MeshModel::CT_MeshModel(const QString &modelName, const CT_AbstractResult *result) : CT_AbstractMeshModel(modelName, result)
{
    m_mesh = NULL;
    m_autoDeleteMesh = true;

    setBaseDrawManager(&CT_MESHMODEL_DRAW_MANAGER);
}

CT_MeshModel::CT_MeshModel(const QString &modelName, const CT_AbstractResult *result, CT_Mesh *mesh) : CT_AbstractMeshModel(modelName, result)
{
    m_mesh = mesh;
    m_autoDeleteMesh = true;

    setBaseDrawManager(&CT_MESHMODEL_DRAW_MANAGER);
}

CT_MeshModel::~CT_MeshModel()
{
    clearMesh();
}

QString CT_MeshModel::getType() const
{
    return staticGetType();
}

QString CT_MeshModel::staticGetType()
{
    return CT_AbstractMeshModel::staticGetType() + "/CT_MeshModel";
}

void CT_MeshModel::setAutoDeleteMesh(bool enable)
{
    m_autoDeleteMesh = enable;
}

bool CT_MeshModel::autoDeleteMesh() const
{
    return m_autoDeleteMesh;
}

void CT_MeshModel::setMesh(CT_Mesh *mesh)
{
    clearMesh();
    m_mesh = mesh;
}

CT_Mesh* CT_MeshModel::mesh() const
{
    return m_mesh;
}

CT_Mesh& CT_MeshModel::cm()
{
    return *mesh();
}

void CT_MeshModel::clearMesh()
{
    if(autoDeleteMesh())
        delete m_mesh;

    m_mesh = NULL;
}

CT_AbstractItemDrawable* CT_MeshModel::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_MeshModel* meshm = new CT_MeshModel(model, result, m_mesh);
    meshm->setAutoDeleteMesh(false);

    return meshm;
}

void CT_MeshModel::drawFaces(GraphicsViewInterface &view, PainterInterface &painter)
{
    if(getAlternativeDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getAlternativeDrawManager())->drawFaces(view, painter, this->mesh());

    if(getBaseDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getBaseDrawManager())->drawFaces(view, painter, this->mesh());
}

void CT_MeshModel::beginDrawMultipleFace(GraphicsViewInterface &view, PainterInterface &painter)
{
    if(getAlternativeDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getAlternativeDrawManager())->beginDrawMultipleFace(view, painter, this->mesh());

    if(getBaseDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getBaseDrawManager())->beginDrawMultipleFace(view, painter, this->mesh());
}

void CT_MeshModel::drawFaceAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter)
{
    if(getAlternativeDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getAlternativeDrawManager())->drawFaceAt(index, view, painter, this->mesh());

    if(getBaseDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getBaseDrawManager())->drawFaceAt(index, view, painter, this->mesh());
}

void CT_MeshModel::endDrawMultipleFace(GraphicsViewInterface &view, PainterInterface &painter)
{
    if(getAlternativeDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getAlternativeDrawManager())->endDrawMultipleFace(view, painter, this->mesh());

    if(getBaseDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getBaseDrawManager())->endDrawMultipleFace(view, painter, this->mesh());
}

void CT_MeshModel::drawEdges(GraphicsViewInterface &view, PainterInterface &painter)
{
    if(getAlternativeDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getAlternativeDrawManager())->drawEdges(view, painter, this->mesh());

    if(getBaseDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getBaseDrawManager())->drawEdges(view, painter, this->mesh());
}

void CT_MeshModel::beginDrawMultipleEdge(GraphicsViewInterface &view, PainterInterface &painter)
{
    if(getAlternativeDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getAlternativeDrawManager())->beginDrawMultipleEdge(view, painter, this->mesh());

    if(getBaseDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getBaseDrawManager())->beginDrawMultipleEdge(view, painter, this->mesh());
}

void CT_MeshModel::drawEdgeAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter)
{
    if(getAlternativeDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getAlternativeDrawManager())->drawEdgeAt(index, view, painter, this->mesh());

    if(getBaseDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getBaseDrawManager())->drawEdgeAt(index, view, painter, this->mesh());
}

void CT_MeshModel::endDrawMultipleEdge(GraphicsViewInterface &view, PainterInterface &painter)
{
    if(getAlternativeDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getAlternativeDrawManager())->endDrawMultipleEdge(view, painter, this->mesh());

    if(getBaseDrawManager() != NULL)
        ((CT_StandardMeshModelDrawManager*)getBaseDrawManager())->endDrawMultipleEdge(view, painter, this->mesh());
}

const CT_AbstractPointCloudIndex* CT_MeshModel::getPointCloudIndex() const
{
    if(m_mesh == NULL)
        return NULL;

    return m_mesh->abstractVert();
}

const CT_AbstractFaceCloudIndex* CT_MeshModel::getFaceCloudIndex() const
{
    if(m_mesh == NULL)
        return NULL;

    return m_mesh->abstractFace();
}

const CT_AbstractEdgeCloudIndex* CT_MeshModel::getEdgeCloudIndex() const
{
    if(m_mesh == NULL)
        return NULL;

    return m_mesh->abstractHedge();
}
