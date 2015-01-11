#include "ct_standardpoint2ddrawmanager.h"
#include "ct_point2d.h"

#include "ct_global/ct_context.h"


const QString CT_StandardPoint2DDrawManager::INDEX_CONFIG_DRAW_POINT = CT_StandardPoint2DDrawManager::staticInitConfigDrawPoint();

CT_StandardPoint2DDrawManager::CT_StandardPoint2DDrawManager(QString drawConfigurationName) : CT_StandardAbstractShape2DDrawManager(drawConfigurationName.isEmpty() ? "CT_Point2D" : drawConfigurationName)
{
    
}

CT_StandardPoint2DDrawManager::~CT_StandardPoint2DDrawManager()
{
}

void CT_StandardPoint2DDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShape2DDrawManager::draw(view, painter, itemDrawable);

    const CT_Point2D &item = dynamic_cast<const CT_Point2D&>(itemDrawable);

    bool drawPoints = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_POINT).toBool();
    bool useAltZVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_USE_ALTERNATIVE_ZVALUE).toBool();
    double zVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_Z_VALUE).toDouble();

    double zPlane = CT_Context::staticInstance()->getZPlaneFor2DShapes();
    if (useAltZVal) {zPlane = zVal;}

    if (drawPoints)
    {
        painter.drawPoint(item.x(), item.y(), zPlane);
    }

}

CT_ItemDrawableConfiguration CT_StandardPoint2DDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractShape2DDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardPoint2DDrawManager::staticInitConfigDrawPoint() ,"Dessiner le point", CT_ItemDrawableConfiguration::Bool, true);

    return item;
}

// PROTECTED //

QString CT_StandardPoint2DDrawManager::staticInitConfigDrawPoint()
{
    return "PT2D_PT";
}
