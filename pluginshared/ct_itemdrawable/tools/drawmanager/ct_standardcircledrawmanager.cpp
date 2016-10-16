#include "ct_standardcircledrawmanager.h"

#include "ct_circle.h"

CT_StandardCircleDrawManager::CT_StandardCircleDrawManager(QString drawConfigurationName) : CT_StandardAbstractShapeDrawManager(drawConfigurationName.isEmpty() ? CT_Circle::staticName() : drawConfigurationName)
{
}

CT_StandardCircleDrawManager::~CT_StandardCircleDrawManager()
{
}

void CT_StandardCircleDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShapeDrawManager::draw(view, painter, itemDrawable);

    const CT_Circle &item = dynamic_cast<const CT_Circle&>(itemDrawable);

    painter.drawCircle3D(item.getCenter(), item.getDirection(), item.getRadius());
}
