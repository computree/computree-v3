#ifndef CT_STANDARDABSTRACTITEMDRAWABLEWITHPOINTCLOUDDRAWMANAGER_H
#define CT_STANDARDABSTRACTITEMDRAWABLEWITHPOINTCLOUDDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/abstract/ct_abstractitemdrawabledrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardAbstractItemDrawableWithPointCloudDrawManager : public CT_AbstractItemDrawableDrawManager
{
public:
    CT_StandardAbstractItemDrawableWithPointCloudDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardAbstractItemDrawableWithPointCloudDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_POINTS_VISIBLE;
    const static QString INDEX_CONFIG_BOUNDING_SHAPE_VISIBLE;
    const static QString INDEX_CONFIG_BOUNDING_SHAPE_CENTER_POINT_VISIBLE;

    static QString staticInitConfigPointsVisible();
    static QString staticInitConfigBoundingShapeVisible();
    static QString staticInitConfigBoundingShapeCenterPointVisible();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;
};

#endif // CT_STANDARDABSTRACTITEMDRAWABLEWITHPOINTCLOUDDRAWMANAGER_H
