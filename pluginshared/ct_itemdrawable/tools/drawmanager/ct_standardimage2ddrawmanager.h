#ifndef CT_STANDARDIMAGE2DDRAWMANAGER_H
#define CT_STANDARDIMAGE2DDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

template< typename DataT>
class CT_StandardImage2DDrawManager : public CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager
{
public:
    CT_StandardImage2DDrawManager(QString drawConfigurationName = "", bool mapMode = true, bool scale = false);
    virtual ~CT_StandardImage2DDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_3D_MODE_ENABLED;
    const static QString INDEX_CONFIG_3D_MODE_LINK_POINTS_ENABLED;
    const static QString INDEX_CONFIG_3D_MODE_HEIGHT_MAP_ENABLED;
    const static QString INDEX_CONFIG_3D_MODE_SCALING_ENABLED;
    const static QString INDEX_CONFIG_3D_MODE_ZMIN_SCALE_VALUE;
    const static QString INDEX_CONFIG_3D_MODE_ZMAX_SCALE_VALUE;
    const static QString INDEX_CONFIG_MAP_MODE_ENABLED;
    const static QString INDEX_CONFIG_MAP_MODE_ZLEVEL_ENABLED;
    const static QString INDEX_CONFIG_MAP_MODE_ZLEVEL_VALUE;
    const static QString INDEX_CONFIG_MAP_MODE_SHOW_GRID;

    static QString staticInitConfig3DModeEnabled();
    static QString staticInitConfig3DModeLinkPointsEnabled();
    static QString staticInitConfig3DModeHeightMapEnabled();
    static QString staticInitConfig3DModeScalingEnabled();
    static QString staticInitConfig3DModeZMinScaleValue();
    static QString staticInitConfig3DModeZMaxScaleValue();
    static QString staticInitConfigMapModeEnabled();
    static QString staticInitConfigMapModeZLevelEnabled();
    static QString staticInitConfigMapModeZLevelValue();
    static QString staticInitConfigMapModeShowGrid();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;

private:
    bool    _defaultMapMode;
    bool    _defaultScaleState;
};

#include "ct_itemdrawable/tools/drawmanager/ct_standardimage2ddrawmanager.hpp"

#endif // CT_STANDARDIMAGE2DDRAWMANAGER_H
