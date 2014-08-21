#ifndef PB_ACTIONDEFINEHEIGHTLAYER_H
#define PB_ACTIONDEFINEHEIGHTLAYER_H


#include "views/actions/pb_actiondefineheightlayeroptions.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

#include "ct_itemdrawable/ct_grid2dxy.h"
#include "ct_itemdrawable/ct_scene.h"


#include <QRect>

class PB_ActionDefineHeightLayer_gridContainer
{
public:
    PB_ActionDefineHeightLayer_gridContainer();

    CT_Grid2DXY<int>    *_densityGrid;
    CT_Grid2DXY<float>  *_mnsGrid;
    float               _zmin;
    float               _zmax;
};

class PB_ActionDefineHeightLayer : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:

    PB_ActionDefineHeightLayer();
    PB_ActionDefineHeightLayer(const CT_AbstractResult *result, const QString &densityGridModel, const QString &mnsGridModel, PB_ActionDefineHeightLayer_gridContainer *gridContainer, const QList<CT_Scene*> &list, float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);

    void createGrids(float res);

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    void init();

    float getZmin() const;
    float getZmax() const;

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
    void computeCrownProjection();
    void updateZValues(float zmin, float zmax);
    void updateResolution(double resolution);
    void updateThreshold(int threshold);

private:
    int         m_status;

    CT_AbstractResult *_result;
    QString _densityGridModel;
    QString _mnsGridModel;

    PB_ActionDefineHeightLayer_gridContainer *_gridContainer;

    QList<CT_Scene*>     _sceneList;
    float               _previousResolution;
    float               _resolution;
    int                 _densityThreshold;
    float               _xmin;
    float               _ymin;
    float               _xmax;
    float               _ymax;
    float               _xrange;
    float               _yrange;

};


#endif // PB_ACTIONDEFINEHEIGHTLAYER_H
