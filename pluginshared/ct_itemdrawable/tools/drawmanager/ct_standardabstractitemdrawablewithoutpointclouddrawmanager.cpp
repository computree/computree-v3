#include "ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

#include "ct_abstractitemdrawablewithoutpointcloud.h"

const QString CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::INDEX_CONFIG_BOUNDING_SHAPE_VISIBLE = CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigBoundingShapeVisible();
const QString CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::INDEX_CONFIG_BOUNDING_SHAPE_POINT_SIZE = CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigBoundingShapePointSize();
const QString CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::INDEX_CONFIG_BOUNDING_SHAPE_CENTER_POINT_VISIBLE = CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigBoundingShapeCenterPointVisible();

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

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_VISIBLE).toBool())
    {
        painter.setPointSize(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_POINT_SIZE).toDouble());
        painter.drawPoint(item.minX(), item.minY(), item.minZ());
        painter.drawPoint(item.maxX(), item.minY(), item.minZ());
        painter.drawPoint(item.minX(), item.maxY(), item.minZ());
        painter.drawPoint(item.minX(), item.minY(), item.maxZ());
        painter.drawPoint(item.maxX(), item.maxY(), item.minZ());
        painter.drawPoint(item.minX(), item.maxY(), item.maxZ());
        painter.drawPoint(item.maxX(), item.minY(), item.maxZ());
        painter.drawPoint(item.maxX(), item.maxY(), item.maxZ());
        painter.restoreDefaultPointSize();

        // B
        painter.drawLine(item.minX(), item.minY(), item.minZ(), item.maxX(), item.minY(), item.minZ());
        painter.drawLine(item.maxX(), item.minY(), item.minZ(), item.maxX(), item.maxY(), item.minZ());
        painter.drawLine(item.maxX(), item.maxY(), item.minZ(), item.minX(), item.maxY(), item.minZ());
        painter.drawLine(item.minX(), item.maxY(), item.minZ(), item.minX(), item.minY(), item.minZ());

        // H
        painter.drawLine(item.minX(), item.minY(), item.maxZ(), item.maxX(), item.minY(), item.maxZ());
        painter.drawLine(item.maxX(), item.minY(), item.maxZ(), item.maxX(), item.maxY(), item.maxZ());
        painter.drawLine(item.maxX(), item.maxY(), item.maxZ(), item.minX(), item.maxY(), item.maxZ());
        painter.drawLine(item.minX(), item.maxY(), item.maxZ(), item.minX(), item.minY(), item.maxZ());

        // G
        painter.drawLine(item.minX(), item.minY(), item.minZ(), item.minX(), item.minY(), item.maxZ());
        painter.drawLine(item.minX(), item.minY(), item.maxZ(), item.minX(), item.maxY(), item.maxZ());
        painter.drawLine(item.minX(), item.maxY(), item.maxZ(), item.minX(), item.maxY(), item.minZ());
        painter.drawLine(item.minX(), item.maxY(), item.minZ(), item.minX(), item.minY(), item.minZ());

        // D
        painter.drawLine(item.maxX(), item.minY(), item.minZ(), item.maxX(), item.minY(), item.maxZ());
        painter.drawLine(item.maxX(), item.minY(), item.maxZ(), item.maxX(), item.maxY(), item.maxZ());
        painter.drawLine(item.maxX(), item.maxY(), item.maxZ(), item.maxX(), item.maxY(), item.minZ());
        painter.drawLine(item.maxX(), item.maxY(), item.minZ(), item.maxX(), item.minY(), item.minZ());
    }

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_CENTER_POINT_VISIBLE).toBool())
    {
        painter.setPointSize(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_POINT_SIZE).toDouble());
        painter.drawPoint(item.getCenterX(), item.getCenterY(), item.getCenterZ());
        painter.restoreDefaultPointSize();
    }
}

CT_ItemDrawableConfiguration CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_AbstractItemDrawableDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigBoundingShapeVisible(),"Bounding Shape", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigBoundingShapeCenterPointVisible(), "Centre de la Bounding Shape", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigBoundingShapePointSize(), "Taille des points (Bounding Shape)", CT_ItemDrawableConfiguration::Double, 10.0);

    return item;
}

// PROTECTED //

QString CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigBoundingShapeVisible()
{
    return "WOPC_BSV";
}

QString CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigBoundingShapePointSize()
{
    return "WOPC_BSPS";
}

QString CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::staticInitConfigBoundingShapeCenterPointVisible()
{
    return "WOPC_BSCPS";
}
