#include "ct_standardspheredrawmanager.h"

#include "ct_itemdrawable/ct_sphere.h"

CT_StandardSphereDrawManager::CT_StandardSphereDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName)
{
}

CT_StandardSphereDrawManager::~CT_StandardSphereDrawManager()
{

}

void CT_StandardSphereDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    const CT_Sphere &item = dynamic_cast<const CT_Sphere&>(itemDrawable);

    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    painter.drawPartOfSphere(item.getCenterX(), item.getCenterY(), item.getCenterZ(), item.getRadius(), 0, 360, 0, 360, false);
}
