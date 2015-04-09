#include "ct_standardmeshmodeldrawmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractmeshmodel.h"
#include "ct_mesh/ct_mesh.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_SHOW_FACES = CT_StandardMeshModelDrawManager::staticInitConfigShowFaces();
const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_SHOW_EDGES = CT_StandardMeshModelDrawManager::staticInitConfigShowEdges();
const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_SHOW_POINTS = CT_StandardMeshModelDrawManager::staticInitConfigShowPoints();
const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_BOUNDING_SHAPE_VISIBLE = CT_StandardMeshModelDrawManager::staticInitConfigBoundingShapeVisible();
const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_BOUNDING_SHAPE_POINT_SIZE = CT_StandardMeshModelDrawManager::staticInitConfigBoundingShapePointSize();
const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_BOUNDING_SHAPE_CENTER_POINT_VISIBLE = CT_StandardMeshModelDrawManager::staticInitConfigBoundingShapeCenterPointVisible();

CT_StandardMeshModelDrawManager::CT_StandardMeshModelDrawManager(QString drawConfigurationName) : CT_AbstractItemDrawableDrawManager(drawConfigurationName.isEmpty() ? "CT_MeshModel" : drawConfigurationName)
{
}

void CT_StandardMeshModelDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    Q_UNUSED(view)

    const CT_AbstractMeshModel &item = dynamic_cast<const CT_AbstractMeshModel&>(itemDrawable);

    bool showFaces = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_FACES).toBool();
    bool showEdges = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_EDGES).toBool();
    bool showPoints = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_POINTS).toBool();

    if(showFaces)
        painter.drawFaces(&item);

    if(showEdges)
        painter.drawEdges(&item);

    if(showPoints)
        painter.drawPoints(&item);

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_VISIBLE).toBool())
    {
        painter.setPointSize(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_POINT_SIZE).toDouble());
        painter.drawPoint(item.minX(), item.minY(), item.minZ());
        painter.drawPoint(item.maxX(), item.minY(), item.minZ());
        painter.drawPoint(item.minX(), item.maxY(), item.minZ());
        painter.drawPoint(item.minX(), item.minY(), item.maxZ());
        painter.drawPoint(item.maxX(), item.maxY(), item.minZ());
        painter.drawPoint(item.minX(), item.maxY(), item.maxZ());
        painter.drawPoint(item.maxX(), item.minY(), item.maxZ());
        painter.drawPoint(item.maxX(), item.maxY(), item.maxZ());
        painter.restoreDefaultPointSize();

        // B
        painter.drawLine(item.minX(), item.minY(), item.minZ(), item.maxX(), item.minY(), item.minZ());
        painter.drawLine(item.maxX(), item.minY(), item.minZ(), item.maxX(), item.maxY(), item.minZ());
        painter.drawLine(item.maxX(), item.maxY(), item.minZ(), item.minX(), item.maxY(), item.minZ());
        painter.drawLine(item.minX(), item.maxY(), item.minZ(), item.minX(), item.minY(), item.minZ());

        // H
        painter.drawLine(item.minX(), item.minY(), item.maxZ(), item.maxX(), item.minY(), item.maxZ());
        painter.drawLine(item.maxX(), item.minY(), item.maxZ(), item.maxX(), item.maxY(), item.maxZ());
        painter.drawLine(item.maxX(), item.maxY(), item.maxZ(), item.minX(), item.maxY(), item.maxZ());
        painter.drawLine(item.minX(), item.maxY(), item.maxZ(), item.minX(), item.minY(), item.maxZ());

        // G
        painter.drawLine(item.minX(), item.minY(), item.minZ(), item.minX(), item.minY(), item.maxZ());
        painter.drawLine(item.minX(), item.minY(), item.maxZ(), item.minX(), item.maxY(), item.maxZ());
        painter.drawLine(item.minX(), item.maxY(), item.maxZ(), item.minX(), item.maxY(), item.minZ());
        painter.drawLine(item.minX(), item.maxY(), item.minZ(), item.minX(), item.minY(), item.minZ());

        // D
        painter.drawLine(item.maxX(), item.minY(), item.minZ(), item.maxX(), item.minY(), item.maxZ());
        painter.drawLine(item.maxX(), item.minY(), item.maxZ(), item.maxX(), item.maxY(), item.maxZ());
        painter.drawLine(item.maxX(), item.maxY(), item.maxZ(), item.maxX(), item.maxY(), item.minZ());
        painter.drawLine(item.maxX(), item.maxY(), item.minZ(), item.maxX(), item.minY(), item.minZ());
    }

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_CENTER_POINT_VISIBLE).toBool())
    {
        painter.setPointSize(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_POINT_SIZE).toDouble());

        painter.drawPoint(item.getCenterX(),
                          item.getCenterY(),
                          item.getCenterZ());

        painter.restoreDefaultPointSize();
    }
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

    const CT_Face &face = m_fAccess.constFaceAt(mesh->face().constIndexAt(index));

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

    const CT_Edge &edge = m_eAccess.constEdgeAt(mesh->hedge().constIndexAt(index));

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
    item.addNewConfiguration(CT_StandardMeshModelDrawManager::staticInitConfigBoundingShapeVisible(),"Bounding Shape", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardMeshModelDrawManager::staticInitConfigBoundingShapeCenterPointVisible(), "Centre de la Bounding Shape", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardMeshModelDrawManager::staticInitConfigBoundingShapePointSize(), "Taille des points (Bounding Shape)", CT_ItemDrawableConfiguration::Double, 10.0);

    return item;
}

// PROTECTED //

QString CT_StandardMeshModelDrawManager::staticInitConfigShowFaces()
{
    return "MM_SHOW_FACES";
}

QString CT_StandardMeshModelDrawManager::staticInitConfigShowEdges()
{
    return "MM_SHOW_EDGES";
}

QString CT_StandardMeshModelDrawManager::staticInitConfigShowPoints()
{
    return "MM_SHOW_POINTS";
}

QString CT_StandardMeshModelDrawManager::staticInitConfigBoundingShapeVisible()
{
    return "MM_BSV";
}

QString CT_StandardMeshModelDrawManager::staticInitConfigBoundingShapePointSize()
{
    return "MM_BSPS";
}

QString CT_StandardMeshModelDrawManager::staticInitConfigBoundingShapeCenterPointVisible()
{
    return "MM_BSCPS";
}
