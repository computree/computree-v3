#include "ct_standardmeshmodeldrawmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractmeshmodel.h"
#include "ct_mesh/ct_mesh.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_SHOW_FACES = CT_StandardMeshModelDrawManager::staticInitConfigShowFaces();
const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_SHOW_EDGES = CT_StandardMeshModelDrawManager::staticInitConfigShowEdges();
const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_SHOW_POINTS = CT_StandardMeshModelDrawManager::staticInitConfigShowPoints();

CT_StandardMeshModelDrawManager::CT_StandardMeshModelDrawManager(QString drawConfigurationName) : CT_AbstractItemDrawableDrawManager(drawConfigurationName.isEmpty() ? "CT_MeshModel" : drawConfigurationName)
{
}

void CT_StandardMeshModelDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    Q_UNUSED(view)

    const CT_AbstractMeshModel &item = dynamic_cast<const CT_AbstractMeshModel&>(itemDrawable);

    painter.pushMatrix();

    QMatrix4x4 m = item.transformMatrix();
    Eigen::Matrix4d em;
    em << m(0, 0), m(0, 1), m(0, 2), m(0, 3),
          m(1, 0), m(1, 1), m(1, 2), m(1, 3),
          m(2, 0), m(2, 1), m(2, 2), m(2, 3),
          m(3, 0), m(3, 1), m(3, 2), m(3, 3);

    painter.multMatrix(em);

    bool showFaces = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_FACES).toBool();
    bool showEdges = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_EDGES).toBool();
    bool showPoints = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_POINTS).toBool();

    if(showFaces)
        painter.drawFaces(&item);

    if(showEdges)
        painter.drawEdges(&item);

    if(showPoints)
        painter.drawPoints(&item);

    painter.popMatrix();
}

void CT_StandardMeshModelDrawManager::drawFaces(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
    if((mesh == NULL) || (mesh->pFace() == NULL))
        return;

    size_t s = mesh->face().size();

    beginDrawMultipleFace(view, painter, mesh);

    for(size_t i=0; i<s; ++i)
        drawFaceAt(i, view, painter, mesh);

    endDrawMultipleFace(view, painter, mesh);
}

void CT_StandardMeshModelDrawManager::beginDrawMultipleFace(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
}

void CT_StandardMeshModelDrawManager::drawFaceAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
    if((mesh == NULL) || (mesh->pFace() == NULL))
        return;

    const CT_Face &face = mesh->face().constTAt(index);

    painter.drawTriangle(face.iPointAt(0), face.iPointAt(1), face.iPointAt(2));
}

void CT_StandardMeshModelDrawManager::endDrawMultipleFace(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
}

void CT_StandardMeshModelDrawManager::drawEdges(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
    if((mesh == NULL) || (mesh->pHedge() == NULL))
        return;

    size_t s = mesh->hedge().size();

    beginDrawMultipleEdge(view, painter, mesh);

    for(size_t i=0; i<s; ++i)
        drawEdgeAt(i, view, painter, mesh);

    endDrawMultipleEdge(view, painter, mesh);
}

void CT_StandardMeshModelDrawManager::beginDrawMultipleEdge(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
}

void CT_StandardMeshModelDrawManager::drawEdgeAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
    if((mesh == NULL) || (mesh->pHedge() == NULL))
        return;

    const CT_Edge &edge = mesh->hedge().constTAt(index);

    painter.drawLine(edge.iPointAt(0), edge.iPointAt(1));
}

void CT_StandardMeshModelDrawManager::endDrawMultipleEdge(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
}

CT_ItemDrawableConfiguration CT_StandardMeshModelDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_AbstractItemDrawableDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardMeshModelDrawManager::staticInitConfigShowFaces(), "Faces", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(CT_StandardMeshModelDrawManager::staticInitConfigShowEdges(), "Edges", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardMeshModelDrawManager::staticInitConfigShowPoints(), "Points", CT_ItemDrawableConfiguration::Bool, false);

    return item;
}

// PROTECTED //

QString CT_StandardMeshModelDrawManager::staticInitConfigShowFaces()
{
    return "SHOW_FACES";
}

QString CT_StandardMeshModelDrawManager::staticInitConfigShowEdges()
{
    return "SHOW_EDGES";
}

QString CT_StandardMeshModelDrawManager::staticInitConfigShowPoints()
{
    return "SHOW_POINTS";
}
