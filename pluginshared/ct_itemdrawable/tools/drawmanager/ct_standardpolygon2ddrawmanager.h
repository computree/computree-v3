#ifndef CT_STANDARDPOLYGON2DDRAWMANAGER_H
#define CT_STANDARDPOLYGON2DDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractshapedrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardPolygon2DDrawManager : public CT_StandardAbstractShapeDrawManager
{
public:
    CT_StandardPolygon2DDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardPolygon2DDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_DRAW_POINTS;
    const static QString INDEX_CONFIG_DRAW_LINES;

    static QString staticInitConfigDrawPoints();
    static QString staticInitConfigDrawLines();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;
};

#endif // CT_STANDARDPOLYGON2DDRAWMANAGER_H
