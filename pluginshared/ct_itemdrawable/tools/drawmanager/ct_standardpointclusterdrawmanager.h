#ifndef CT_STANDARDPOINTCLUSTERDRAWMANAGER_H
#define CT_STANDARDPOINTCLUSTERDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithpointclouddrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardPointClusterDrawManager : public CT_StandardAbstractItemDrawableWithPointCloudDrawManager
{
public:
    CT_StandardPointClusterDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardPointClusterDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:
    const static QString INDEX_CONFIG_BARYCENTER_VISIBLE;
    const static QString INDEX_CONFIG_LINES_VISIBLE;

    static QString staticInitConfigBarycenterVisible();
    static QString staticInitConfigLinesVisible();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;
};

#endif // CT_STANDARDPOINTCLUSTERDRAWMANAGER_H
