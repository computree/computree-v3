#ifndef CT_STANDARDPLANARBSPLINEDRAWMANAGER_H
#define CT_STANDARDPLANARBSPLINEDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractshapedrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardPlanarBSplineDrawManager : public CT_StandardAbstractShapeDrawManager
{
public:
    CT_StandardPlanarBSplineDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardPlanarBSplineDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:
    const static QString INDEX_CONFIG_DRAW_CONTROL_POINTS;
    const static QString INDEX_CONFIG_CONTROL_POINTS_SIZE;
    const static QString INDEX_CONFIG_DRAW_POLYLINE;

    static QString staticInitConfigDrawControlPoints();
    static QString staticInitConfigControlPointsSize();
    static QString staticInitConfigDrawPolyline();


    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;

};

#endif // CT_STANDARDPLANARBSPLINEDRAWMANAGER_H
