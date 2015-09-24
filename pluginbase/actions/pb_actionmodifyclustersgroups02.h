#ifndef PB_ACTIONMODIFYCLUSTERSGROUPS02_H
#define PB_ACTIONMODIFYCLUSTERSGROUPS02_H

#include <QRect>

#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "ct_itemdrawable/ct_point2d.h"
#include "ct_itemdrawable/ct_pointcluster.h"

/**
 * @brief Action to select items, points, faces, edges, etc...
 *
 * You can use this action in your action if you want. Just call methods :
 *
 * init()
 * mousePressEvent()
 * etc.....
 *
 * in methods of your action.
 *
 * Dont forget to set the selection mode => setSelectionMode method
 */
class PB_ActionModifyClustersGroups02 : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:

    PB_ActionModifyClustersGroups02(QMap<const CT_Point2D*, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > *map, QMultiMap<CT_PointCluster*, CT_PointCluster*> *clToCl, QList<CT_PointCluster*> *trash);

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    void init();

    void updateAllClustersColors();
    void updateColorForOneCluster(const CT_Point2D *position);

    void updateVisiblePositions();

    bool mouseDoubleClickEvent(QMouseEvent *e);
    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);

    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);

    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    CT_AbstractAction* copy() const;

    /**
     * @brief change the selection mode for this action. The mode will be set to view
     *        when the mousePressEvent() method is called
     * @return false if we are between mousePressEvent() and mouseReleaseEvent()
     */
    bool setSelectionMode(GraphicsViewInterface::SelectionMode mode);

    GraphicsViewInterface::SelectionMode selectionMode() const;

public slots:
    void setColorA(QColor color);
    void setColorB(QColor color);
    void selectPositionA();
    void selectPositionB();
    void swapAandB();
    void visibilityChanged();
    void affectClusterToA();
    void affectClusterToB();
    void affectClusterToTMP();
    void affectClusterToTrash();
    void extend();
    void validatePosition();

private:
    QRect                                   m_selectionRectangle;
    int                                     m_status;
    GraphicsViewInterface::SelectionMode    m_selectionMode;
    bool                                    m_mousePressed;

    QMap<const CT_Point2D*, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > *_positionToCluster;
    QMultiMap<CT_PointCluster*, CT_PointCluster*>                                               *_clusterToCluster;
    QList<CT_PointCluster*>                                                                     *_trashClusterList;

    QMap<const CT_PointCluster*, const CT_Point2D*> _clusterToPosition;

    QList<QColor>                   _automaticColorList;
    QMap<const CT_Point2D*, QColor> _positionsBaseColors;
    QColor                          _colorA;
    QColor                          _colorB;
    QColor                          _colorTmp;
    QColor                          _colorTrash;
    QColor                          _validatedColor;

    CT_Point2D*                     _positionA;
    CT_Point2D*                     _positionB;
    QList<CT_PointCluster*>         _temporaryClusterList;
    bool                            _positionsChanged;
    QList<CT_PointCluster*>         _clustersOrdered;
    int                             _currentLastA;
    QList<const CT_Point2D*>        _validatedPositions;



    GraphicsViewInterface::SelectionMode selectionModeToBasic(GraphicsViewInterface::SelectionMode mode) const;

    void addToA(CT_PointCluster* cluster);
    void addToB(CT_PointCluster* cluster);

};

#endif // PB_ACTIONMODIFYCLUSTERSGROUPS02_H
