#ifndef CT_STANDARDABSTRACTPOINTSATTRIBUTESDRAWMANAGER_H
#define CT_STANDARDABSTRACTPOINTSATTRIBUTESDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/abstract/ct_abstractitemdrawabledrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardAbstractPointsAttributesDrawManager : public CT_AbstractItemDrawableDrawManager
{
public:
    CT_StandardAbstractPointsAttributesDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;
};

#endif // CT_STANDARDABSTRACTPOINTSATTRIBUTESDRAWMANAGER_H
