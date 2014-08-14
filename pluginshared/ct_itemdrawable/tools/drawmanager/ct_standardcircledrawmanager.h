#ifndef CT_STANDARDCIRCLEDRAWMANAGER_H
#define CT_STANDARDCIRCLEDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractshapedrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardCircleDrawManager : public CT_StandardAbstractShapeDrawManager
{
public:
    CT_StandardCircleDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardCircleDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;
};

#endif // CT_STANDARDCIRCLEDRAWMANAGER_H
