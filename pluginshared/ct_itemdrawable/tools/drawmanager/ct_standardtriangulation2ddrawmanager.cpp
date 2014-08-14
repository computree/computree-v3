#include "ct_standardtriangulation2ddrawmanager.h"

#include "ct_triangulation2d.h"
#include "ct_nodet.h"
#include "ct_edget.h"
#include "ct_trianglet.h"

const QString CT_StandardTriangulation2DDrawManager::INDEX_CONFIG_NODES_VISIBLE = CT_StandardTriangulation2DDrawManager::staticInitConfigNodesVisible();
const QString CT_StandardTriangulation2DDrawManager::INDEX_CONFIG_EDGES_VISIBLE = CT_StandardTriangulation2DDrawManager::staticInitConfigEdgesVisible();
const QString CT_StandardTriangulation2DDrawManager::INDEX_CONFIG_HULL_VISIBLE = CT_StandardTriangulation2DDrawManager::staticInitConfigHullVisible();
const QString CT_StandardTriangulation2DDrawManager::INDEX_CONFIG_VORONOI_VISIBLE = CT_StandardTriangulation2DDrawManager::staticInitConfigVoronoiVisible();

CT_StandardTriangulation2DDrawManager::CT_StandardTriangulation2DDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? "CT_Triangulation2D" : drawConfigurationName)
{
    
}

CT_StandardTriangulation2DDrawManager::~CT_StandardTriangulation2DDrawManager()
{
}

void CT_StandardTriangulation2DDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    const CT_Triangulation2D &item = dynamic_cast<const CT_Triangulation2D&>(itemDrawable);

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_NODES_VISIBLE).toBool())
        drawNodes(view, painter, item);

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_EDGES_VISIBLE).toBool())
        drawHull(view, painter, item);

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HULL_VISIBLE).toBool())
        drawEdges(view, painter, item);

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_VORONOI_VISIBLE).toBool())
        drawVoronoi(view, painter, item);
}

CT_ItemDrawableConfiguration CT_StandardTriangulation2DDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardTriangulation2DDrawManager::staticInitConfigNodesVisible(), "Points", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(CT_StandardTriangulation2DDrawManager::staticInitConfigEdgesVisible(), "Aretes", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(CT_StandardTriangulation2DDrawManager::staticInitConfigHullVisible(), "Enveloppe convexe", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardTriangulation2DDrawManager::staticInitConfigVoronoiVisible(), "Voronoï", CT_ItemDrawableConfiguration::Bool, false);

    return item;
}

// PROTECTED //

QString CT_StandardTriangulation2DDrawManager::staticInitConfigNodesVisible()
{
    return "T2D_NV";
}

QString CT_StandardTriangulation2DDrawManager::staticInitConfigEdgesVisible()
{
    return "T2D_EV";
}

QString CT_StandardTriangulation2DDrawManager::staticInitConfigHullVisible()
{
    return "T2D_HV";
}

QString CT_StandardTriangulation2DDrawManager::staticInitConfigVoronoiVisible()
{
    return "T2D_VV";
}

void CT_StandardTriangulation2DDrawManager::drawNodes(GraphicsViewInterface &view, PainterInterface &painter, const CT_Triangulation2D &item) const
{
    Q_UNUSED(view)

    painter.setPointSize(10.0);

    painter.setForcedColor(Qt::white);

    QListIterator< QSharedPointer<CT_NodeT> > it(item.getDelaunayT()->getNodeList());

    while(it.hasNext())
    {
        float *p = it.next().data()->getPoint();

        painter.drawPoint(p[0], p[1], p[2]);
    }

    painter.restoreDefaultPointSize();
}

void CT_StandardTriangulation2DDrawManager::drawEdges(GraphicsViewInterface &view, PainterInterface &painter, const CT_Triangulation2D &item) const
{
    Q_UNUSED(view)

    painter.setForcedColor(Qt::white);

    QListIterator< QSharedPointer<CT_EdgeT> > it(item.getDelaunayT()->getEdgeList());

    while(it.hasNext())
    {
        QSharedPointer<CT_EdgeT> ed = it.next();

        float *p1 = ed.data()->getN1().data()->getPoint();
        float *p2 = ed.data()->getN2().data()->getPoint();

        painter.drawLine(p1[0], p1[1], p1[2], p2[0], p2[1], p2[2]);
    }
}

void CT_StandardTriangulation2DDrawManager::drawHull(GraphicsViewInterface &view, PainterInterface &painter, const CT_Triangulation2D &item) const
{
    Q_UNUSED(view)

    painter.setForcedColor(Qt::red);

    QSharedPointer<CT_EdgeT> hull_start = item.getDelaunayT()->getHullEdgeStart();

    if(!hull_start.isNull())
    {
        QSharedPointer<CT_EdgeT> next_edge = hull_start;

        do
        {
            float *p1 = next_edge.data()->getN1().data()->getPoint();
            float *p2 = next_edge.data()->getN2().data()->getPoint();

            painter.drawLine(p1[0], p1[1], p1[2], p2[0], p2[1], p2[2]);

            next_edge = next_edge.data()->getHullEdge();

        }while(hull_start != next_edge);
    }
}

void CT_StandardTriangulation2DDrawManager::drawVoronoi(GraphicsViewInterface &view, PainterInterface &painter, const CT_Triangulation2D &item) const
{
    Q_UNUSED(view)

    painter.setForcedColor(255, 0, 255);

    QListIterator< QSharedPointer<CT_EdgeT> > it(item.getDelaunayT()->getEdgeList());

    while(it.hasNext())
    {
        QSharedPointer<CT_EdgeT> ed = it.next();
        QSharedPointer<CT_EdgeT> ed_inv = ed.data()->getInversedEdge();

        if((!ed_inv.isNull())
            && (!ed_inv.data()->getTriangle().isNull()))
        {
            QSharedPointer<CT_TriangleT> ed_tri = ed.data()->getTriangle();
            QSharedPointer<CT_TriangleT> ed_inv_tri = ed_inv.data()->getTriangle();

            painter.drawLine(ed_tri.data()->getCx(), ed_tri.data()->getCy(), ed_tri.data()->getCz(),
                             ed_inv_tri.data()->getCx(), ed_inv_tri.data()->getCy(), ed_inv_tri.data()->getCz());
        }
    }
}
