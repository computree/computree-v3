#include "ct_standardellipsedrawmanager.h"

#include "ct_ellipse.h"

#include <QObject>


CT_StandardEllipseDrawManager::CT_StandardEllipseDrawManager(QString drawConfigurationName) : CT_StandardAbstractShapeDrawManager(drawConfigurationName.isEmpty() ? QObject::tr("3D ellipse") : drawConfigurationName)
{
}

CT_StandardEllipseDrawManager::~CT_StandardEllipseDrawManager()
{
}

void CT_StandardEllipseDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShapeDrawManager::draw(view, painter, itemDrawable);

    const CT_Ellipse &item = dynamic_cast<const CT_Ellipse&>(itemDrawable);

    painter.drawEllipse(item.getCenterX(), item.getCenterY(), item.getCenterZ(), item.getAxisA().length()/2.0, item.getAxisB().length()/2.0);
}
