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

    PB_ActionSlicePointCloud(QList<CT_Scene*>* sceneList);

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

public slots:
    void toDoIfButtonPushed();
    void redraw();

private:
    int               _value;
    QList<CT_Scene*>* _sceneList;

};


#endif // PB_ACTIONSLICEPOINTCLOUD_H
