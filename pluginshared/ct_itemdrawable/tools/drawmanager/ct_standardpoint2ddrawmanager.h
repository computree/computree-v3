#ifndef CT_STANDARDPOINT2DDRAWMANAGER_H
#define CT_STANDARDPOINT2DDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractshape2ddrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardPoint2DDrawManager : public CT_StandardAbstractShape2DDrawManager
{
public:
    CT_StandardPoint2DDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardPoint2DDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_DRAW_POINT;
    const static QString INDEX_CONFIG_POINT_SIZE;

    static QString staticInitConfigDrawPoint();
    static QString staticInitConfigPointSize();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;
};

#endif // CT_STANDARDPOINT2DDRAWMANAGER_H
