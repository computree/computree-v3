#ifndef CT_STANDARDBEAMDRAWMANAGER_H
#define CT_STANDARDBEAMDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardBeamDrawManager : public CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager
{
public:
    CT_StandardBeamDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardBeamDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_LENGTH_OF_BEAM;

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;

    static QString staticInitConfigLengthOfRay();
};

#endif // CT_STANDARDBEAMDRAWMANAGER_H
