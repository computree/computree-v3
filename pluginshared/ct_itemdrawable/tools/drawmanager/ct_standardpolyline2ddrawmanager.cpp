#include "ct_standardpolyline2ddrawmanager.h"
#include "ct_polyline2d.h"

#include "ct_global/ct_context.h"


const QString CT_StandardPolyline2DDrawManager::INDEX_CONFIG_DRAW_POINTS = CT_StandardPolyline2DDrawManager::staticInitConfigDrawPoints();
const QString CT_StandardPolyline2DDrawManager::INDEX_CONFIG_DRAW_LINES = CT_StandardPolyline2DDrawManager::staticInitConfigDrawLines();

CT_StandardPolyline2DDrawManager::CT_StandardPolyline2DDrawManager(QString drawConfigurationName) : CT_StandardAbstractShape2DDrawManager(drawConfigurationName.isEmpty() ? "CT_Polyline2D" : drawConfigurationName)
{
    
}

CT_StandardPolyline2DDrawManager::~CT_StandardPolyline2DDrawManager()
{
}

void CT_StandardPolyline2DDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShape2DDrawManager::draw(view, painter, itemDrawable);

    const CT_Polyline2D &item = dynamic_cast<const CT_Polyline2D&>(itemDrawable);

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

        for (int i = 0 ; i < size ; i++)
        {
            QVector2D *pt1 = vertices.at(i);

            if(drawPoints)
            {
                painter.drawPoint(pt1->x(), pt1->y(), zPlane);
            }
            if(drawLines && (i < size - 1))
            {
                QVector2D *pt2 = vertices.at(i+1);
                painter.drawLine(pt1->x(), pt1->y(), zPlane, pt2->x(), pt2->y(), zPlane);
            }
        }
    }
}

CT_ItemDrawableConfiguration CT_StandardPolyline2DDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractShape2DDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardPolyline2DDrawManager::staticInitConfigDrawPoints() ,"Dessiner les sommets", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardPolyline2DDrawManager::staticInitConfigDrawLines() ,"Dessiner les côtés", CT_ItemDrawableConfiguration::Bool, true);

    return item;
}

// PROTECTED //

QString CT_StandardPolyline2DDrawManager::staticInitConfigDrawPoints()
{
    return "PL2D_PT";
}

QString CT_StandardPolyline2DDrawManager::staticInitConfigDrawLines()
{
    return "PL2D_LI";
}
