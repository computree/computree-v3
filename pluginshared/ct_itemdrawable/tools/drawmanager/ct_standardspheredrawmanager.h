#ifndef CT_STANDARDSPHEREDRAWMANAGER_H
#define CT_STANDARDSPHEREDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

#include <QObject>

class CT_Sphere;

class PLUGINSHAREDSHARED_EXPORT CT_StandardSphereDrawManager : public CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager
{
public:
    CT_StandardSphereDrawManager(QString drawConfigurationName = QObject::tr("Sphere"));
    virtual ~CT_StandardSphereDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;
};

#endif // CT_STANDARDSPHEREDRAWMANAGER_H
