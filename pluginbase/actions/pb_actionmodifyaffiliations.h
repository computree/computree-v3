#ifndef PB_ACTIONMODIFYAFFILIATIONS_H
#define PB_ACTIONMODIFYAFFILIATIONS_H

#include "views/actions/pb_actionmodifyaffiliationsoptions.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include <QRect>

class PB_ActionModifyAffiliations : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:
    PB_ActionModifyAffiliations(const QList<CT_AbstractSingularItemDrawable*> *sourceList,
                                const QList<CT_AbstractSingularItemDrawable*> *targetList,
                                QMap<CT_AbstractSingularItemDrawable*, CT_AbstractSingularItemDrawable*> *correspondances);

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    void init();

    bool mouseDoubleClickEvent(QMouseEvent *e);
    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool wheelEvent(QWheelEvent *e);

    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);

    void draw(GraphicsViewInterface &view, PainterInterface &painter);
    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    CT_AbstractAction* copy() const;

    CT_AbstractSingularItemDrawable * closetItemFromClickDirection(QMouseEvent *e,
                                                                   const QList<CT_AbstractSingularItemDrawable *> *itemList);
public slots:
    void updateGraphics();
    void affiliate();
    void breakAffiliation();

private:
    int         m_status;

    float       _symbolSize;

    const QList<CT_AbstractSingularItemDrawable*>     *_sourceList;
    const QList<CT_AbstractSingularItemDrawable*>     *_targetList;
    QMap<CT_AbstractSingularItemDrawable*, CT_AbstractSingularItemDrawable*>    *_correspondances; // (key=target, value=source)
    QMap<CT_AbstractSingularItemDrawable*, QVector3D>   _itemCenters;
    CT_AbstractSingularItemDrawable*                    _activeSource;
    CT_AbstractSingularItemDrawable*                    _activeTarget;


    void drawSourceItem(PainterInterface &painter, const QVector3D &position);
    void drawTargetItem(PainterInterface &painter, const QVector3D &position);
    void drawAffiliation(PainterInterface &painter, const QVector3D &source, const QVector3D &target);

};


#endif // PB_ACTIONMODIFYAFFILIATIONS_H
