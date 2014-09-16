#ifndef DM_ABSTRACTITEMTREEVIEWCONTROLLER_H
#define DM_ABSTRACTITEMTREEVIEWCONTROLLER_H

#include <QObject>

class DM_AbstractItemTreeViewController : public QObject
{
    Q_OBJECT
public:
    explicit DM_AbstractItemTreeViewController(QObject *parent = 0);

private slots:

    /**
     * @brief Called when the model builder is finished (called in the gui thread)
     */
    virtual void slotModelBuilderFinished(bool canceled) = 0;

    /**
     * @brief Called when the model builder has finish to update (called in the gui thread)
     */
    virtual void slotModelBuilderUpdateFinished(bool canceled) = 0;

    /**
     * @brief Called when the model builder is finished (called in the gui thread)
     */
    virtual void slotModelBuilderAddFinished(bool canceled) = 0;

    /**
     * @brief Called when the model builder is finished (called in the gui thread)
     */
    virtual void slotModelBuilderRemoveFinished(bool canceled) = 0;

    /**
     * @brief Called when the timer time out and when we must add CT_AbstractItemDrawable of the
     *        m_tmpItemsToAdd list to the treeview
     */
    virtual void slotAddTemporaryItemsInTable() = 0;

    /**
     * @brief Called when the timer time out and when we must remove CT_AbstractItemDrawable of the
     *        m_tmpItemsToRemove list from the treeview
     */
    virtual void slotRemoveTemporaryItemsInTable() = 0;

signals:
    void mustStartAddTimer();
    void mustStartRemoveTimer();
    void updated();

};

#endif // DM_ABSTRACTITEMTREEVIEWCONTROLLER_H
