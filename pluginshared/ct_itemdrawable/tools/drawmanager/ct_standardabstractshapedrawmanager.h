#ifndef CT_STANDARDABSTRACTSHAPEDRAWMANAGER_H
#define CT_STANDARDABSTRACTSHAPEDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardAbstractShapeDrawManager : public CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager
{
public:
    CT_StandardAbstractShapeDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardAbstractShapeDrawManager();
};

#endif // CT_STANDARDABSTRACTSHAPEDRAWMANAGER_H
