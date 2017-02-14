#ifndef CT_STANDARDCOLORCOMPOSITEDRAWMANAGER_H
#define CT_STANDARDCOLORCOMPOSITEDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

#ifdef USE_OPENCV
class CT_StandardColorCompositeDrawManager : public CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager
{
public:
    CT_StandardColorCompositeDrawManager(QString drawConfigurationName = "", bool mapMode = true, bool scale = false);
    virtual ~CT_StandardColorCompositeDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_3D_MODE_ENABLED;
    const static QString INDEX_CONFIG_MAP_MODE_ENABLED;
    const static QString INDEX_CONFIG_MAP_MODE_ZLEVEL_ENABLED;
    const static QString INDEX_CONFIG_MAP_MODE_ZLEVEL_VALUE;

    static QString staticInitConfig3DModeEnabled();
    static QString staticInitConfigMapModeEnabled();
    static QString staticInitConfigMapModeZLevelEnabled();
    static QString staticInitConfigMapModeZLevelValue();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;

private:
    bool    _defaultMapMode;
    bool    _defaultScaleState;
};

#endif

#endif // CT_STANDARDCOLORCOMPOSITEDRAWMANAGER_H
