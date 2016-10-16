#include "ct_standardline2ddrawmanager.h"
#include "ct_line2d.h"

#include "ct_global/ct_context.h"

#include <QObject>


const QString CT_StandardLine2DDrawManager::INDEX_CONFIG_DRAW_POINTS = CT_StandardLine2DDrawManager::staticInitConfigDrawPoints();
const QString CT_StandardLine2DDrawManager::INDEX_CONFIG_DRAW_LINE = CT_StandardLine2DDrawManager::staticInitConfigDrawLine();

CT_StandardLine2DDrawManager::CT_StandardLine2DDrawManager(QString drawConfigurationName) : CT_StandardAbstractShape2DDrawManager(drawConfigurationName.isEmpty() ? CT_Line2D::staticName() : drawConfigurationName)
{
    
}

CT_StandardLine2DDrawManager::~CT_StandardLine2DDrawManager()
{
}

void CT_StandardLine2DDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShape2DDrawManager::draw(view, painter, itemDrawable);

    const CT_Line2D &item = dynamic_cast<const CT_Line2D&>(itemDrawable);

    bool drawPoints = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_POINTS).toBool();
    bool drawLine = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_LINE).toBool();
    bool useAltZVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_USE_ALTERNATIVE_ZVALUE).toBool();
    double zVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_Z_VALUE).toDouble();

    double zPlane = CT_Context::staticInstance()->getZPlaneFor2DShapes();
    if (useAltZVal) {zPlane = zVal;}

    if (drawPoints)
    {
        painter.drawPoint(item.x1(), item.y1(), zPlane);
        painter.drawPoint(item.x2(), item.y2(), zPlane);
    }

    if (drawLine)
    {
        painter.drawLine(item.x1(), item.y1(), zPlane, item.x2(), item.y2(), zPlane);
    }
}

CT_ItemDrawableConfiguration CT_StandardLine2DDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractShape2DDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardLine2DDrawManager::staticInitConfigDrawPoints() ,QObject::tr("Dessiner les points"), CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardLine2DDrawManager::staticInitConfigDrawLine() ,QObject::tr("Dessiner la ligne"), CT_ItemDrawableConfiguration::Bool, true);

    return item;
}

// PROTECTED //

QString CT_StandardLine2DDrawManager::staticInitConfigDrawPoints()
{
    return "L2D_PT";
}

QString CT_StandardLine2DDrawManager::staticInitConfigDrawLine()
{
    return "L2D_LI";
}
