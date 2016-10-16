#include "ct_standardcircle2ddrawmanager.h"
#include "ct_circle2d.h"

#include "ct_global/ct_context.h"

#include <QObject>

const QString CT_StandardCircle2DDrawManager::INDEX_CONFIG_DRAW_CIRCLE = CT_StandardCircle2DDrawManager::staticInitConfigDrawCircle();

CT_StandardCircle2DDrawManager::CT_StandardCircle2DDrawManager(QString drawConfigurationName) : CT_StandardAbstractShape2DDrawManager(drawConfigurationName.isEmpty() ? CT_Circle2D::staticName() : drawConfigurationName)
{
    
}

CT_StandardCircle2DDrawManager::~CT_StandardCircle2DDrawManager()
{
}

void CT_StandardCircle2DDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShape2DDrawManager::draw(view, painter, itemDrawable);

    const CT_Circle2D &item = dynamic_cast<const CT_Circle2D&>(itemDrawable);

    bool drawCircle = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_CIRCLE).toBool();
    bool useAltZVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_USE_ALTERNATIVE_ZVALUE).toBool();
    double zVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_Z_VALUE).toDouble();

    double zPlane = CT_Context::staticInstance()->getZPlaneFor2DShapes();
    if (useAltZVal)
    {
        zPlane = zVal;
    } else if (item.isZValueDefined()) {
        zPlane = item.getZValue();
    }

    if (drawCircle)
    {
        painter.drawCircle(item.getCenterX(), item.getCenterY(), zPlane, item.getRadius());
    }
}

CT_ItemDrawableConfiguration CT_StandardCircle2DDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractShape2DDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardCircle2DDrawManager::staticInitConfigDrawCircle() ,QObject::tr("Dessiner le cercle"), CT_ItemDrawableConfiguration::Bool, true);

    return item;
}

// PROTECTED //

QString CT_StandardCircle2DDrawManager::staticInitConfigDrawCircle()
{
    return "C2D_CR";
}
