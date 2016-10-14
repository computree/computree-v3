#include "ct_standardpolyline2ddrawmanager.h"
#include "ct_polyline2d.h"

#include "ct_global/ct_context.h"

#include <QObject>



const QString CT_StandardPolyline2DDrawManager::INDEX_CONFIG_DRAW_POINTS = CT_StandardPolyline2DDrawManager::staticInitConfigDrawPoints();
const QString CT_StandardPolyline2DDrawManager::INDEX_CONFIG_DRAW_LINES = CT_StandardPolyline2DDrawManager::staticInitConfigDrawLines();

CT_StandardPolyline2DDrawManager::CT_StandardPolyline2DDrawManager(QString drawConfigurationName) : CT_StandardAbstractShape2DDrawManager(drawConfigurationName.isEmpty() ? QObject::tr("2D polyline") : drawConfigurationName)
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
    double zVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_Z_VALUE).toDouble();    

    double zPlane = CT_Context::staticInstance()->getZPlaneFor2DShapes();

    if (useAltZVal)
    {
        zPlane = zVal;
    } else if (item.isZValueDefined()) {
        zPlane = item.getZValue();
    }

    if(drawPoints || drawLines)
    {
        const QVector<Eigen::Vector2d*>&vertices = item.getVertices();
        int size = vertices.size();

        for (int i = 0 ; i < size ; i++)
        {
            Eigen::Vector2d *pt1 = vertices.at(i);

            if(drawPoints)
            {
                painter.drawPoint((*pt1)(0), (*pt1)(1), zPlane);
            }
            if(drawLines && (i < size - 1))
            {
                Eigen::Vector2d *pt2 = vertices.at(i+1);
                painter.drawLine((*pt1)(0), (*pt1)(1), zPlane, (*pt2)(0), (*pt2)(1), zPlane);
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
