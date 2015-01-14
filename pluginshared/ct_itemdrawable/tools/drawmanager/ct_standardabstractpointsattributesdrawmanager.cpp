#include "ct_standardabstractpointsattributesdrawmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"

CT_StandardAbstractPointsAttributesDrawManager::CT_StandardAbstractPointsAttributesDrawManager() : CT_AbstractItemDrawableDrawManager()
{
}

void CT_StandardAbstractPointsAttributesDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    const CT_AbstractPointsAttributes &item = dynamic_cast<const CT_AbstractPointsAttributes&>(itemDrawable);

    painter.drawPointCloud(item.getPointCloudIndex());
}
