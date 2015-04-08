#ifndef CT_STANDARDABSTRACTSHAPE2DDRAWMANAGER_H
#define CT_STANDARDABSTRACTSHAPE2DDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardAbstractShape2DDrawManager : public CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager
{
public:
    CT_StandardAbstractShape2DDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardAbstractShape2DDrawManager();

    void setZValue(double val);

protected:

    const static QString INDEX_CONFIG_USE_ALTERNATIVE_ZVALUE;
    const static QString INDEX_CONFIG_Z_VALUE;

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;


    static QString staticInitConfigUseAlternativeZValue();
    static QString staticInitConfigZValue();
};

#endif // CT_STANDARDABSTRACTSHAPE2DDRAWMANAGER_H
