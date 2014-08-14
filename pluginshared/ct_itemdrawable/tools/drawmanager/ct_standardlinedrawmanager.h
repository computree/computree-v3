#ifndef CT_STANDARDLINEDRAWMANAGER_H
#define CT_STANDARDLINEDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractshapedrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardLineDrawManager : public CT_StandardAbstractShapeDrawManager
{
public:
    CT_StandardLineDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardLineDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

    void configure(const QPen &pen);

protected:

    const static QString INDEX_CONFIG_LINE_SIZE;
    const static QString INDEX_CONFIG_LINE_STYLE;

    static QString staticInitConfigLineSize();
    static QString staticInitConfigLineStyle();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;

private:
    QPen            _pen;
    bool            _forcedStyle;

};

#endif // CT_STANDARDLINEDRAWMANAGER_H
