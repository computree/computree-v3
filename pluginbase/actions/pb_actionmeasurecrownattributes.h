#ifndef PB_ACTIONMEASURECROWNATTRIBUTES_H
#define PB_ACTIONMEASURECROWNATTRIBUTES_H


#include "views/actions/pb_actionmeasurecrownattributesoptions.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

#include "ct_itemdrawable/abstract/ct_abstractgrid3d.h"
#include "ct_itemdrawable/ct_grid2dxy.h"
#include "ct_itemdrawable/ct_scene.h"


#include <QRect>

class PB_ActionMeasureCrownAttributes : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:
    PB_ActionMeasureCrownAttributes(CT_Grid2DXY<int> *grid, const QList<CT_Scene*> &list, float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);

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

public slots:
    void updateGraphics();
    void setSideView();
    void computeCrownProjection(float zmin, float zmax);


private:
    int         m_status;

    CT_Grid2DXY<int>    *_grid;
    QList<CT_Scene*>     _sceneList;
    float               _projectionLevel;
    float               _xmin;
    float               _ymin;
    float               _zmin;
    float               _xmax;
    float               _ymax;
    float               _zmax;
    float               _xrange;
    float               _yrange;

};


#endif // PB_ACTIONMEASURECROWNATTRIBUTES_H
