#ifndef CT_STANDARDBOX2DDRAWMANAGER_H
#define CT_STANDARDBOX2DDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractshape2ddrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardBox2DDrawManager : public CT_StandardAbstractShape2DDrawManager
{
public:
    CT_StandardBox2DDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardBox2DDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_DRAW_BOX;

    static QString staticInitConfigDrawBox();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;
};

#endif // CT_STANDARDBOX2DDRAWMANAGER_H
