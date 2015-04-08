#ifndef PB_ACTIONMODIFYPOSITIONS2D_H
#define PB_ACTIONMODIFYPOSITIONS2D_H


#include "views/actions/pb_actionmodifypositions2doptions.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

#include "ct_itemdrawable/ct_point2d.h"
#include "ct_result/ct_resultgroup.h"

#include <QRect>

class PB_ActionModifyPositions2D : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:

    PB_ActionModifyPositions2D(QList<CT_Point2D*> &positions, CT_OutAbstractSingularItemModel *model, CT_ResultGroup *outRes);

    ~PB_ActionModifyPositions2D();

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
    void zValChanged();
private:
    Qt::MouseButtons                    _buttonsPressed;
    QList<CT_Point2D*>*                 _positions;
    CT_Point2D*                         _selectedPoint;
    CT_OutAbstractSingularItemModel*    _model;
    CT_ResultGroup*                     _outRes;

    QColor                              _selectedColor;
    QColor                              _normalColor;
    Eigen::Vector2d                     _min;
    Eigen::Vector2d                     _max;

    bool getCoordsForMousePosition(const QMouseEvent *e, double &x, double &y);
    CT_Point2D *getNearestPosition(double x, double y);
};


#endif // PB_ACTIONMODIFYPOSITIONS2D_H
