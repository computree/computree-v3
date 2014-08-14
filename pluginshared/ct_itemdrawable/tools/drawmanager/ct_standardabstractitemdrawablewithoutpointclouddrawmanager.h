#ifndef CT_STANDARDABSTRACTITEMDRAWABLEWITHOUTPOINTCLOUDDRAWMANAGER_H
#define CT_STANDARDABSTRACTITEMDRAWABLEWITHOUTPOINTCLOUDDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/abstract/ct_abstractitemdrawabledrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager : public CT_AbstractItemDrawableDrawManager
{
public:
    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_CENTER_VISIBLE;
    const static QString INDEX_CONFIG_CENTER_POINT_SIZE;

    static QString staticInitConfigCenterVisible();
    static QString staticInitConfigCenterPointSize();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;
};

#endif // CT_STANDARDABSTRACTITEMDRAWABLEWITHOUTPOINTCLOUDDRAWMANAGER_H
