#include "ct_standardmeshmodeldrawmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractmeshmodel.h"
#include "ct_mesh/ct_mesh.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

#include <QtOpenGL>

// GLU was removed from Qt in version 4.8
#ifdef Q_OS_MAC
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif


const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_SHOW_FACES = CT_StandardMeshModelDrawManager::staticInitConfigShowFaces();
const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_SHOW_EDGES = CT_StandardMeshModelDrawManager::staticInitConfigShowEdges();
const QString CT_StandardMeshModelDrawManager::INDEX_CONFIG_SHOW_POINTS = CT_StandardMeshModelDrawManager::staticInitConfigShowPoints();

CT_StandardMeshModelDrawManager::CT_StandardMeshModelDrawManager(QString drawConfigurationName) : CT_AbstractItemDrawableDrawManager(drawConfigurationName.isEmpty() ? "CT_MeshModel" : drawConfigurationName)
{
}

// add overloaded functions which call the underlying OpenGL function
inline void glMultMatrix(const GLfloat  *m) { glMultMatrixf(m); }
inline void glMultMatrix(const GLdouble *m) { glMultMatrixd(m); }

// add an overload for QMatrix4x4 for convenience
inline void glMultMatrix(const QMatrix4x4 &m) { glMultMatrix(m.constData()); }

void CT_StandardMeshModelDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    Q_UNUSED(view)

    const CT_AbstractMeshModel &item = dynamic_cast<const CT_AbstractMeshModel&>(itemDrawable);

    glPushMatrix();
    glMultMatrix(item.transformMatrix());

    bool showFaces = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_FACES).toBool();
    bool showEdges = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_EDGES).toBool();
    bool showPoints = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_POINTS).toBool();

    if(showFaces)
        painter.drawFaces(&item);

    if(showEdges)
        painter.drawEdges(&item);

    if(showPoints)
        painter.drawPoints(&item, 4);

    glPopMatrix();
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
    if((mesh == NULL) || (mesh->pFace() == NULL))
        return;

    painter.beginDrawMultipleTriangle();
}

void CT_StandardMeshModelDrawManager::drawFaceAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
    if((mesh == NULL) || (mesh->pFace() == NULL))
        return;

    const CT_Face &face = mesh->face().constTAt(index);

    painter.drawTriangle(&((*face.pointAt(0))(0)), &((*face.pointAt(1))(0)), &((*face.pointAt(2))(0)));
}

void CT_StandardMeshModelDrawManager::endDrawMultipleFace(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
    if((mesh == NULL) || (mesh->pFace() == NULL))
        return;

    painter.endDrawMultipleTriangle();
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
    if((mesh == NULL) || (mesh->pHedge() == NULL))
        return;

    painter.beginDrawMultipleLine();
}

void CT_StandardMeshModelDrawManager::drawEdgeAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
    if((mesh == NULL) || (mesh->pHedge() == NULL))
        return;

    const CT_Edge &edge = mesh->hedge().constTAt(index);

    painter.drawLine(&((*edge.pointAt(0))(0)), &((*edge.pointAt(1))(0)));
}

void CT_StandardMeshModelDrawManager::endDrawMultipleEdge(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh)
{
    if((mesh == NULL) || (mesh->pHedge() == NULL))
        return;

    painter.endDrawMultipleLine();
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
