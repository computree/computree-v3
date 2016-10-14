#include "ct_standardpointclusterdrawmanager.h"
#include "ct_itemdrawable/ct_pointcluster.h"

#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"
#include "ct_accessor/ct_pointaccessor.h"

#include <QObject>


const QString CT_StandardPointClusterDrawManager::INDEX_CONFIG_BARYCENTER_VISIBLE = CT_StandardPointClusterDrawManager::staticInitConfigBarycenterVisible();
const QString CT_StandardPointClusterDrawManager::INDEX_CONFIG_LINES_VISIBLE = CT_StandardPointClusterDrawManager::staticInitConfigLinesVisible();

CT_StandardPointClusterDrawManager::CT_StandardPointClusterDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithPointCloudDrawManager(drawConfigurationName.isEmpty() ? QObject::tr("Point cluster") : drawConfigurationName)
{
}

CT_StandardPointClusterDrawManager::~CT_StandardPointClusterDrawManager()
{
}

void CT_StandardPointClusterDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    const CT_PointCluster &item = dynamic_cast<const CT_PointCluster&>(itemDrawable);

    CT_StandardAbstractItemDrawableWithPointCloudDrawManager::draw(view, painter, itemDrawable);

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BARYCENTER_VISIBLE).toBool())
    {
        const CT_PointClusterBarycenter &barycenter = item.getBarycenter();
        painter.drawPoint(barycenter.x(), barycenter.y(), barycenter.z());
    }

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_LINES_VISIBLE).toBool())
    {
        CT_PointAccessor pointAccessor;
        const CT_AbstractPointCloudIndex* pointCloudIndex = item.getPointCloudIndex();

        for (int i = 0 ; i < pointCloudIndex->size() - 1; i++)
        {
            const size_t index1 = pointCloudIndex->constIndexAt(i);
            const size_t index2 = pointCloudIndex->constIndexAt(i+1);

            CT_Point point1 = pointAccessor.constPointAt(index1);
            CT_Point point2 = pointAccessor.constPointAt(index2);
            painter.drawLine(point1(0), point1(1), point1(2), point2(0), point2(1), point2(2));
        }
    }
}

CT_ItemDrawableConfiguration CT_StandardPointClusterDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractItemDrawableWithPointCloudDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardPointClusterDrawManager::staticInitConfigBarycenterVisible(),"Barycentre", CT_ItemDrawableConfiguration::Bool, false );
    item.addNewConfiguration(CT_StandardPointClusterDrawManager::staticInitConfigLinesVisible(),"Polyligne", CT_ItemDrawableConfiguration::Bool, false );

    return item;
}

// PROTECTED //

QString CT_StandardPointClusterDrawManager::staticInitConfigBarycenterVisible()
{
    return "PTCL_BA";
}

QString CT_StandardPointClusterDrawManager::staticInitConfigLinesVisible()
{
    return "PTCL_LI";
}
