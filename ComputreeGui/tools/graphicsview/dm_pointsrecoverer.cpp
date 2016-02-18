#include "dm_pointsrecoverer.h"

#include "ct_itemdrawable/accessibility/ct_iaccesspointcloud.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

DM_PointsRecoverer::DM_PointsRecoverer()
{
    m_pIndexBackup.setSortType(CT_AbstractCloudIndex::NotSorted);
    m_fakePainter.setComputingMode(DM_FakePainter::BackupPointCloudIndex
                                   | DM_FakePainter::BackupPointCloudIndexIfEdge
                                   | DM_FakePainter::BackupPointCloudIndexIfFace);
}

QList<CT_AbstractCloudIndex *> DM_PointsRecoverer::recoverDrawnGlobalPointsInItemDrawable(const QList<CT_AbstractItemDrawable *> &items,
                                                                                          GraphicsViewInterface &graphicsView,
                                                                                          CT_PointCloudIndexVector *optionnalCloudIndexForPointDrawOneByOne)
{
    m_fakePainter.clear();

    if(optionnalCloudIndexForPointDrawOneByOne != NULL) {
        m_fakePainter.setForGlobalPointDrawOneByOneTheBackupCloudIndex(optionnalCloudIndexForPointDrawOneByOne);
    } else {
        m_pIndexBackup.clear();
        m_fakePainter.setForGlobalPointDrawOneByOneTheBackupCloudIndex(&m_pIndexBackup);
    }

    QListIterator<CT_AbstractItemDrawable *> it(items);

    while(it.hasNext())
        it.next()->draw(graphicsView, m_fakePainter);

    return m_fakePainter.pointCloudIndexBackup();
}

QList<CT_AbstractCloudIndex *> DM_PointsRecoverer::recoverGlobalPointsInItemDrawable(const QList<CT_AbstractItemDrawable *> &items)
{
    QList<CT_AbstractCloudIndex *> l;

    QListIterator<CT_AbstractItemDrawable *> it(items);

    while(it.hasNext()) {
        CT_IAccessPointCloud *ap = dynamic_cast<CT_IAccessPointCloud*>(it.next());

        if((ap != NULL) && (ap->getPointCloudIndex() != NULL))
            l.append((CT_AbstractCloudIndex*)ap->getPointCloudIndex());
    }

    return l;
}
