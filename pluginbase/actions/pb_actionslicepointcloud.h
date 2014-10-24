#ifndef PB_ACTIONSLICEPOINTCLOUD_H
#define PB_ACTIONSLICEPOINTCLOUD_H


#include "views/actions/pb_actionslicepointcloudoptions.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "ct_itemdrawable/ct_scene.h"

#include <QRect>

class PB_ActionSlicePointCloud : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:

    PB_ActionSlicePointCloud(QList<CT_Scene*>* sceneList, float xmin, float ymin, float zmin, float xmax, float ymax, float zmax, double thickness, double spacing);

    ~PB_ActionSlicePointCloud();

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    void init();

    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool wheelEvent(QWheelEvent *e);

    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);

    void draw(GraphicsViewInterface &view, PainterInterface &painter);
    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    CT_AbstractAction* copy() const;

    inline float getZMin() {return _zmin;}
    inline float getThickness() {return _thickness;}
    inline float getSpacing() {return _spacing;}

public slots:
    void redraw();

private:
    QList<CT_Scene*>* _sceneList;
    double            _thickness;
    double            _spacing;

    float             _xmin;
    float             _ymin;
    float             _zmin;
    float             _xmax;
    float             _ymax;
    float             _zmax;

    float             _xwidth;
    float             _ywidth;

};


#endif // PB_ACTIONSLICEPOINTCLOUD_H
