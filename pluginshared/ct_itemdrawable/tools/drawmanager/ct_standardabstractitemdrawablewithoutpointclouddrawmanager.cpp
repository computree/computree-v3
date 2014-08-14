#include "ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

#include "ct_abstractitemdrawablewithoutpointcloud.h"

const QString CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::INDEX_CONFIG_CENTER_VISIBLE = CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigCenterVisible();
const QString CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::INDEX_CONFIG_CENTER_POINT_SIZE = CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigCenterPointSize();

CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(QString drawConfigurationName) : CT_AbstractItemDrawableDrawManager(drawConfigurationName.isEmpty() ? "CT_AbstractItemDrawableWithoutPointCloud" : drawConfigurationName)
{
}

CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::~CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager()
{
}

void CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    Q_UNUSED(view)

    const CT_AbstractItemDrawableWithoutPointCloud &item = dynamic_cast<const CT_AbstractItemDrawableWithoutPointCloud&>(itemDrawable);

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_CENTER_VISIBLE).toBool())
    {
        painter.setPointSize(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_CENTER_POINT_SIZE).toDouble());
        painter.drawPoint(item.getCenterX(), item.getCenterY(), item.getCenterZ());
        painter.restoreDefaultPointSize();
    }
}

CT_ItemDrawableConfiguration CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_AbstractItemDrawableDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigCenterVisible(), "Centre", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigCenterPointSize(), "Taille des points (Centre)", CT_ItemDrawableConfiguration::Double, 10.0);

    return item;
}

// PROTECTED //

QString CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigCenterVisible()
{
    return "WOPC_CV";
}

QString CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigCenterPointSize()
{
    return "WOPC_CPS";
}
