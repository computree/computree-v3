#include "ct_standardpolygon2ddrawmanager.h"
#include "ct_polygon2d.h"

#include "ct_global/ct_context.h"


const QString CT_StandardPolygon2DDrawManager::INDEX_CONFIG_DRAW_POINTS = CT_StandardPolygon2DDrawManager::staticInitConfigDrawPoints();
const QString CT_StandardPolygon2DDrawManager::INDEX_CONFIG_DRAW_LINES = CT_StandardPolygon2DDrawManager::staticInitConfigDrawLines();

CT_StandardPolygon2DDrawManager::CT_StandardPolygon2DDrawManager(QString drawConfigurationName) : CT_StandardAbstractShape2DDrawManager(drawConfigurationName.isEmpty() ? "CT_Polygon2D" : drawConfigurationName)
{
    
}

CT_StandardPolygon2DDrawManager::~CT_StandardPolygon2DDrawManager()
{
}

void CT_StandardPolygon2DDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShape2DDrawManager::draw(view, painter, itemDrawable);

    const CT_Polygon2D &item = dynamic_cast<const CT_Polygon2D&>(itemDrawable);

    bool drawPoints = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_POINTS).toBool();
    bool drawLines = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_LINES).toBool();
    bool useAltZVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_USE_ALTERNATIVE_ZVALUE).toBool();
    float zVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_Z_VALUE).toFloat();

    float zPlane = CT_Context::staticInstance()->getZPlaneFor2DShapes();
    if (useAltZVal) {zPlane = zVal;}

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
                painter.drawPoint(pt2->x(), pt2->y(), zPlane);
            }
            if(drawLines)
            {
                painter.drawLine(pt1->x(), pt1->y(), zPlane, pt2->x(), pt2->y(), zPlane);
            }
            pt1 = pt2;
        }
    }
}

CT_ItemDrawableConfiguration CT_StandardPolygon2DDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractShape2DDrawManager::createDrawConfiguration(drawConfigurationName));
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
