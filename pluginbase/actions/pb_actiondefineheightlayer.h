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
    CT_Grid2DXY<double>  *_mnsGrid;
    double              _zmin;
    double              _zmax;
};

class PB_ActionDefineHeightLayer : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:

    PB_ActionDefineHeightLayer();
    PB_ActionDefineHeightLayer(const CT_AbstractResult *result, const QString &densityGridModel, const QString &mnsGridModel, PB_ActionDefineHeightLayer_gridContainer *gridContainer, const QList<CT_Scene*> &list, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);

    void createGrids(double res);

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    void init();

    double getZmin() const;
    double getZmax() const;

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
    void updateZValues(double zmin, double zmax);
    void updateResolution(double resolution);
    void updateThreshold(int threshold);

private:
    int         m_status;

    CT_AbstractResult *_result;
    QString _densityGridModel;
    QString _mnsGridModel;

    PB_ActionDefineHeightLayer_gridContainer *_gridContainer;

    QList<CT_Scene*>     _sceneList;
    double               _previousResolution;
    double               _resolution;
    int                 _densityThreshold;
    double               _xmin;
    double               _ymin;
    double               _xmax;
    double               _ymax;
    double               _xrange;
    double               _yrange;

};


#endif // PB_ACTIONDEFINEHEIGHTLAYER_H
