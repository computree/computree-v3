#include "ct_standardpolygon2ddrawmanager.h"
#include "ct_polygon2d_old.h"


const QString CT_StandardPolygon2DDrawManager::INDEX_CONFIG_DRAW_POINTS = CT_StandardPolygon2DDrawManager::staticInitConfigDrawPoints();
const QString CT_StandardPolygon2DDrawManager::INDEX_CONFIG_DRAW_LINES = CT_StandardPolygon2DDrawManager::staticInitConfigDrawLines();

CT_StandardPolygon2DDrawManager::CT_StandardPolygon2DDrawManager(QString drawConfigurationName) : CT_StandardAbstractShapeDrawManager(drawConfigurationName.isEmpty() ? "CT_Polygon2D_Old" : drawConfigurationName)
{
    
}

CT_StandardPolygon2DDrawManager::~CT_StandardPolygon2DDrawManager()
{
}

void CT_StandardPolygon2DDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShapeDrawManager::draw(view, painter, itemDrawable);

    const CT_Polygon2D_Old &item = dynamic_cast<const CT_Polygon2D_Old&>(itemDrawable);

    bool drawPoints = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_POINTS).toBool();
    bool drawLines = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_LINES).toBool();

    if(drawPoints || drawLines)
    {
        const QVector<QVector2D*>&vertices = item.getVertices();
        int size = vertices.size();

        QVector2D *pt1 = vertices.last();
        for (int i = 0 ; i < size ; i++)
        {
            QVector2D *pt2 = vertices.at(i);
            if(drawPoints)
            {
                painter.drawPoint(pt2->x(), pt2->y(), 0);
            }
            if(drawLines)
            {
                painter.drawLine(pt1->x(), pt1->y(), 0, pt2->x(), pt2->y(), 0);
            }
            pt1 = pt2;
        }
    }
}

CT_ItemDrawableConfiguration CT_StandardPolygon2DDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractShapeDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardPolygon2DDrawManager::staticInitConfigDrawPoints() ,"Dessiner les sommets", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardPolygon2DDrawManager::staticInitConfigDrawLines() ,"Dessiner les côtés", CT_ItemDrawableConfiguration::Bool, true);

    return item;
}

// PROTECTED //

QString CT_StandardPolygon2DDrawManager::staticInitConfigDrawPoints()
{
    return "P2D_PT";
}

QString CT_StandardPolygon2DDrawManager::staticInitConfigDrawLines()
{
    return "P2D_LI";
}
