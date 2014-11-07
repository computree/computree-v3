#ifndef CT_STANDARDCIRCLE2DDRAWMANAGER_H
#define CT_STANDARDCIRCLE2DDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractshape2ddrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardCircle2DDrawManager : public CT_StandardAbstractShape2DDrawManager
{
public:
    CT_StandardCircle2DDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardCircle2DDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_DRAW_CIRCLE;

    static QString staticInitConfigDrawCircle();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;
};

#endif // CT_STANDARDCIRCLE2DDRAWMANAGER_H
