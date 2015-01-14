#include "ct_standardcircledrawmanager.h"

#include "ct_circle.h"

CT_StandardCircleDrawManager::CT_StandardCircleDrawManager(QString drawConfigurationName) : CT_StandardAbstractShapeDrawManager(drawConfigurationName.isEmpty() ? "CT_Circle" : drawConfigurationName)
{
}

CT_StandardCircleDrawManager::~CT_StandardCircleDrawManager()
{
}

void CT_StandardCircleDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShapeDrawManager::draw(view, painter, itemDrawable);

    const CT_Circle &item = dynamic_cast<const CT_Circle&>(itemDrawable);

    const QVector3D &cc = item.getCenter();
    const QVector3D &dd = item.getDirection();

    Eigen::Vector3d center(cc.x(), cc.y(), cc.z());
    Eigen::Vector3d direction(dd.x(), dd.y(), dd.z());

    painter.drawCircle3D(center, direction, item.getRadius());
}
