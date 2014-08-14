#ifndef CT_STANDARDELLIPSEDRAWMANAGER_H
#define CT_STANDARDELLIPSEDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractshapedrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardEllipseDrawManager : public CT_StandardAbstractShapeDrawManager
{
public:
    CT_StandardEllipseDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardEllipseDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;
};

#endif // CT_STANDARDELLIPSEDRAWMANAGER_H
