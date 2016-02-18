#ifndef DM_POINTSRECOVERER_H
#define DM_POINTSRECOVERER_H

#include <QList>
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_fakepainter.h"
#include "ct_cloudindex/ct_cloudindexstdvectort.h"

class CT_AbstractCloudIndex;
class CT_AbstractItemDrawable;
class GraphicsViewInterface;

/**
 * @brief This tool class can be used to recover points in itemdrawable. It can be two types : only points drawn or real points inside the itemdrawable.
 */
class DM_PointsRecoverer
{
public:
    DM_PointsRecoverer();

    /**
     * @brief Recover drawn points of multiple itemdrawable
     * @param items : the list of items that you will get index cloud
     * @param graphicsview : the view that will draw this item
     * @param optionnalCloudIndexForPointDrawOneByOne : if you call a multiple time this method you must pass at each time a cloud index that will be used
     *        to add point draw one by one in the graphics view. If you don't pass this element, this class will pass a temporary cloud index that will be cleared
     *        at each call of this method.
     */
    QList<CT_AbstractCloudIndex*> recoverDrawnGlobalPointsInItemDrawable(const QList<CT_AbstractItemDrawable *> &items,
                                                                         GraphicsViewInterface &graphicsView,
                                                                         CT_PointCloudIndexVector *optionnalCloudIndexForPointDrawOneByOne = NULL);

    /**
     * @brief Recover contained points in multiple itemdrawable.
     * @warning Itemdrawable must inherit from CT_IAccessPointCloud otherwise the method will return a empty list
     */
    QList<CT_AbstractCloudIndex*> recoverGlobalPointsInItemDrawable(const QList<CT_AbstractItemDrawable *> &items);

private:
    DM_FakePainter              m_fakePainter;
    CT_PointCloudIndexVector    m_pIndexBackup;
};

#endif // DM_POINTSRECOVERER_H
