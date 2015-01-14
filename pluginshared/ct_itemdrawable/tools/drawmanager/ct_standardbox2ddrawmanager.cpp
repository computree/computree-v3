#include "ct_standardbox2ddrawmanager.h"
#include "ct_box2d.h"

#include "ct_global/ct_context.h"


const QString CT_StandardBox2DDrawManager::INDEX_CONFIG_DRAW_BOX = CT_StandardBox2DDrawManager::staticInitConfigDrawBox();

CT_StandardBox2DDrawManager::CT_StandardBox2DDrawManager(QString drawConfigurationName) : CT_StandardAbstractShape2DDrawManager(drawConfigurationName.isEmpty() ? "CT_Box2D" : drawConfigurationName)
{
    
}

CT_StandardBox2DDrawManager::~CT_StandardBox2DDrawManager()
{
}

void CT_StandardBox2DDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShape2DDrawManager::draw(view, painter, itemDrawable);

    const CT_Box2D &item = dynamic_cast<const CT_Box2D&>(itemDrawable);

    bool drawBox = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_BOX).toBool();
    bool useAltZVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_USE_ALTERNATIVE_ZVALUE).toBool();
    double zVal = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_Z_VALUE).toDouble();

    double zPlane = CT_Context::staticInstance()->getZPlaneFor2DShapes();
    if (useAltZVal) {zPlane = zVal;}

    if (drawBox)
    {
        Eigen::Vector3d min, max;
        item.getData().getBoundingBox(min, max);

        painter.drawRectXY(min.head(2), max.head(2), zPlane);
    }
}

CT_ItemDrawableConfiguration CT_StandardBox2DDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractShape2DDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardBox2DDrawManager::staticInitConfigDrawBox() ,"Dessiner le rectangle", CT_ItemDrawableConfiguration::Bool, true);

    return item;
}

// PROTECTED //

QString CT_StandardBox2DDrawManager::staticInitConfigDrawBox()
{
    return "B2D_BX";
}
