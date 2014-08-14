#ifndef CT_STANDARDREFERENCEPOINTDRAWMANAGER_H
#define CT_STANDARDREFERENCEPOINTDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardReferencePointDrawManager : public CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager
{
public:
    CT_StandardReferencePointDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardReferencePointDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_BUFFER_VISIBLE;
    const static QString INDEX_CONFIG_FACTOR;
    const static QString INDEX_CONFIG_POINT_SIZE;

    static QString staticInitConfigBufferVisible();
    static QString staticInitConfigFactor();
    static QString staticInitConfigPointSize();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;
};

#endif // CT_STANDARDREFERENCEPOINTDRAWMANAGER_H
