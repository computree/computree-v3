#include "dm_itemdrawabletreeviewcontroller.h"

#include "dm_guimanager.h"
#include "tools/treeview/dm_itemdrawabletreeviewmodelbuilder.h"
#include "tools/treeview/dm_itemdrawabletreeviewmodelresearcher.h"

DM_ItemDrawableTreeViewController::DM_ItemDrawableTreeViewController(QObject *parent) : QObject(parent)
{
    m_standardItemBuilder = NULL;
    m_treeViewManager = NULL;
    m_model = NULL;

    m_addMultiple = false;
    m_removeMultiple = false;
    m_timerAddToView.setSingleShot(true);
    m_timerAddToView.setInterval(50);
    m_timerRemoveFromView.setSingleShot(true);
    m_timerRemoveFromView.setInterval(50);

    m_maxRemoveInGuiThread = 100;

    connect(this, SIGNAL(mustStartAddTimer()), &m_timerAddToView, SLOT(start()), Qt::QueuedConnection);
    connect(&m_timerAddToView, SIGNAL(timeout()), this, SLOT(slotAddTemporaryItemsInTable()), Qt::QueuedConnection);

    connect(this, SIGNAL(mustStartRemoveTimer()), &m_timerRemoveFromView, SLOT(start()), Qt::QueuedConnection);
    connect(&m_timerRemoveFromView, SIGNAL(timeout()), this, SLOT(slotRemoveTemporaryItemsInTable()), Qt::QueuedConnection);
}

void DM_ItemDrawableTreeViewController::setModel(const QStandardItemModel *model)
{
    m_model = (QStandardItemModel*)model;
}

void DM_ItemDrawableTreeViewController::setStandardItemBuilder(const DM_IItemDrawableStandardItemBuilder *builder)
{
    m_standardItemBuilder = (DM_IItemDrawableStandardItemBuilder*)builder;
}

void DM_ItemDrawableTreeViewController::setTreeViewManager(const DM_ITreeViewManager *manager)
{
    m_treeViewManager = (DM_ITreeViewManager*)manager;
}

void DM_ItemDrawableTreeViewController::setMaxRemoveToDoInGuiThread(const int &n)
{
    m_maxRemoveInGuiThread = n;
}

int DM_ItemDrawableTreeViewController::maxRemoveToDoInGuiThread() const
{
    return m_maxRemoveInGuiThread;
}

void DM_ItemDrawableTreeViewController::beginAddMultipleItemDrawable()
{
    m_addMultiple = true;
}

void DM_ItemDrawableTreeViewController::addItemDrawable(CT_AbstractItemDrawable &item)
{
    m_tmpItemsToAdd.append(&item);

    // and if we don't add multiple items we start the timer
    if(!m_addMultiple)
        emit mustStartAddTimer();
}

void DM_ItemDrawableTreeViewController::endAddMultipleItemDrawable()
{
    m_addMultiple = false;

    if(!m_tmpItemsToAdd.isEmpty())
        emit mustStartAddTimer();
}

void DM_ItemDrawableTreeViewController::beginRemoveMultipleItemDrawable()
{
    m_removeMultiple = true;
}

void DM_ItemDrawableTreeViewController::removeItemDrawable(CT_AbstractItemDrawable &item)
{
    m_tmpItemsToRemove.append(&item);

    // and if we don't remove multiple items we start the timer
    if(!m_removeMultiple)
        emit mustStartRemoveTimer();
}

void DM_ItemDrawableTreeViewController::endRemoveMultipleItemDrawable()
{
    m_removeMultiple = false;

    if(!m_tmpItemsToRemove.isEmpty())
        emit mustStartRemoveTimer();
}

void DM_ItemDrawableTreeViewController::addElementsOfCollection()
{
    QVectorIterator< QList<QStandardItem*> > it(m_collection);

    while(it.hasNext())
    {
        const QList<QStandardItem*> &ll = it.next();

        if(!ll.isEmpty())
            m_model->appendRow(ll);
    }

    m_collection.clear();

    // refresh headers
    m_treeViewManager->refreshHeaders();
}

void DM_ItemDrawableTreeViewController::updateElemensOfCollection()
{
    if(m_tmpItemsToUpdate.size() == m_collectionUpdate.size())
    {
        QVectorIterator< QList<QStandardItem*> > it(m_collectionUpdate);
        QListIterator< QPair<QStandardItem *, CT_AbstractItemDrawable *> > itL(m_tmpItemsToUpdate);

        while(it.hasNext()
                && itL.hasNext())
        {
            const QList<QStandardItem*> &ll = it.next();
            const QPair<QStandardItem *, CT_AbstractItemDrawable *> &pair = itL.next();

            if(!ll.isEmpty())
            {
                QStandardItem *ii = pair.first;
                QStandardItem *parent = ii->parent();
                int row = ii->row();

                parent->removeRow(row);

                parent->insertRow(row, ll);
            }
        }

        m_collectionUpdate.clear();
        m_tmpItemsToUpdate.clear();

        // refresh headers
        m_treeViewManager->refreshHeaders();

    }
}

void DM_ItemDrawableTreeViewController::constructModel()
{
    m_timerAddToView.stop();
    m_tmpItemsToAdd.clear();

    m_timerRemoveFromView.stop();
    m_tmpItemsToRemove.clear();

    if(m_treeViewManager->canConstructTheModel())
    {
        //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("reConstructModel"));

        // add all CT_AbstractItemDrawable of the type to the QTreeView
        DM_AsyncOperation *aop = GUI_MANAGER->requestExclusiveAsyncOperation();

        m_timerAddToView.stop();
        m_tmpItemsToAdd.clear();

        m_timerRemoveFromView.stop();
        m_tmpItemsToRemove.clear();

        if(aop != NULL)
        {
            aop->progressDialog()->setCanClose(false);
            aop->progressDialog()->setLabelText(tr("Veuillez patienter pendant la construction de la table"));
            aop->progressDialog()->setSecondLabelText("");
            aop->progressDialog()->setValue(0);

            QList<CT_AbstractItemDrawable*> items = m_treeViewManager->itemDrawableForTreeView();

            if(!items.isEmpty())
            {
                aop->progressDialog()->show();

                // create a thread for the builder
                QThread *thread = new QThread();

                // create a builder
                DM_ItemDrawableTreeViewModelBuilder *builder = new DM_ItemDrawableTreeViewModelBuilder();
                builder->moveToThread(thread);
                builder->addData(aop, true);
                builder->setCollection(&m_collection);
                builder->setStandardItemBuilder(m_standardItemBuilder);
                builder->setNLevelToBuild(m_treeViewManager->nLevelToConstruct());
                builder->setItemDrawable(items);

                connect(builder, SIGNAL(finished(bool)), this, SLOT(slotModelBuilderFinished(bool)), Qt::QueuedConnection);
                connect(builder, SIGNAL(progressChanged(int)), aop, SLOT(setProgress(int)), Qt::QueuedConnection);
                connect(aop, SIGNAL(cancel()), builder, SLOT(cancel()), Qt::QueuedConnection);

                DM_AbstractWorker::staticConnectWorkerToThread(builder, true, true, true);

                // start the build (result is directly loaded in the model, show 'slotModelBuilderFinished' method)
                thread->start();
            }
            else
            {
                delete aop;

                m_model->clear();

                // refresh headers
                m_treeViewManager->refreshHeaders();
            }
        }
    }
    else
    {
        m_model->clear();

        // refresh headers
        m_treeViewManager->refreshHeaders();
    }
}

void DM_ItemDrawableTreeViewController::refresh(const QList<QPair<QStandardItem *, CT_AbstractItemDrawable *> > &list)
{
    //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("refresh a item in the Model"));

    // add all CT_AbstractItemDrawable of the type to the QTreeView
    DM_AsyncOperation *aop = GUI_MANAGER->requestExclusiveAsyncOperation();

    if(aop != NULL)
    {
        aop->progressDialog()->setCanClose(false);
        aop->progressDialog()->setLabelText(tr("Veuillez patienter pendant le rafraichissement de la table"));
        aop->progressDialog()->setSecondLabelText("");
        aop->progressDialog()->setValue(0);
        aop->progressDialog()->show();

        m_tmpItemsToUpdate = list;

        // create a thread for the builder
        QThread *thread = new QThread();

        // create a builder
        DM_ItemDrawableTreeViewModelBuilder *builder = new DM_ItemDrawableTreeViewModelBuilder();
        builder->moveToThread(thread);
        builder->addData(aop, true);
        builder->setCollection(&m_collectionUpdate);
        builder->setStandardItemBuilder(m_standardItemBuilder);
        builder->setNLevelToBuild(m_treeViewManager->nLevelToConstruct());
        builder->setQStandardItemToUpdate(list);

        connect(builder, SIGNAL(finished(bool)), this, SLOT(slotModelBuilderUpdateFinished(bool)), Qt::QueuedConnection);
        connect(builder, SIGNAL(progressChanged(int)), aop, SLOT(setProgress(int)), Qt::QueuedConnection);
        connect(aop, SIGNAL(cancel()), builder, SLOT(cancel()), Qt::QueuedConnection);

        DM_AbstractWorker::staticConnectWorkerToThread(builder, true, true, true);

        // start the build (result is directly loaded in the model, show 'slotModelBuilderFinished' method)
        thread->start();
    }
}

void DM_ItemDrawableTreeViewController::slotModelBuilderFinished(bool canceled)
{
    //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotModelBuilderFinished : ") + (canceled ? "true" : "false"));

    // if the builder was canceled
    if(canceled)
    {
        // clear the collection
        m_collection.clear();
    }
    else
    {
        // clear the model
        m_model->clear();

        // add all elements
        addElementsOfCollection();
    }

    // refresh headers
    m_treeViewManager->refreshHeaders();
}

void DM_ItemDrawableTreeViewController::slotModelBuilderUpdateFinished(bool canceled)
{
    // if the builder was not canceled
    if(!canceled)
    {
        // update element
        updateElemensOfCollection();
    }

    m_tmpItemsToUpdate.clear();

    // refresh headers
    m_treeViewManager->refreshHeaders();
}

void DM_ItemDrawableTreeViewController::slotModelBuilderAddFinished(bool canceled)
{
    //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotModelBuilderAddFinished : ") + (canceled ? "true" : "false"));

    // if the builder was canceled
    if(canceled)
    {
        // clear the collection
        m_collection.clear();
        m_tmpItemsToAdd.clear();
    }
    else
    {
        // add new elements
        addElementsOfCollection();
        m_tmpItemsToAdd.clear();
    }

    // refresh headers
    m_treeViewManager->refreshHeaders();
}

void DM_ItemDrawableTreeViewController::slotModelBuilderRemoveFinished(bool canceled)
{
    //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotModelBuilderRemoveFinished : ") + (canceled ? "true" : "false"));

    // if the builder was canceled
    if(canceled)
    {
        // clear the collection
        m_collection.clear();
        m_tmpItemsToRemove.clear();

        // refresh headers
        m_treeViewManager->refreshHeaders();
    }
    else
    {
        // remove all elements
        QVectorIterator< QList<QStandardItem*> > it(m_collection);

        while(it.hasNext())
        {
            const QList<QStandardItem*> &ll = it.next();

            if(!ll.isEmpty())
                m_model->removeRow(ll.first()->row());
        }

        m_collection.clear();
        m_tmpItemsToRemove.clear();

        // refresh headers
        m_treeViewManager->refreshHeaders();
    }
}

void DM_ItemDrawableTreeViewController::slotAddTemporaryItemsInTable()
{
    // add all CT_AbstractItemDrawable in the temporary list to the QTreeView
    DM_AsyncOperation *aop = GUI_MANAGER->requestExclusiveAsyncOperation();

    if(!m_tmpItemsToAdd.isEmpty())
    {
        //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotAddTemporaryItemsInTable"));

        if(aop != NULL)
        {
            aop->progressDialog()->setCanClose(false);
            aop->progressDialog()->setLabelText(tr("Veuillez patienter pendant l'ajout des éléments à la table"));
            aop->progressDialog()->setSecondLabelText("");
            aop->progressDialog()->setValue(0);
            aop->progressDialog()->show();

            // create a thread for the builder
            QThread *thread = new QThread();

            // create a builder
            DM_ItemDrawableTreeViewModelBuilder *builder = new DM_ItemDrawableTreeViewModelBuilder();
            builder->moveToThread(thread);
            builder->addData(aop, true);
            builder->setCollection(&m_collection);
            builder->setStandardItemBuilder(m_standardItemBuilder);
            builder->setNLevelToBuild(m_treeViewManager->nLevelToConstruct());
            builder->setItemDrawable(m_tmpItemsToAdd);

            connect(builder, SIGNAL(finished(bool)), this, SLOT(slotModelBuilderAddFinished(bool)), Qt::QueuedConnection);
            connect(builder, SIGNAL(progressChanged(int)), aop, SLOT(setProgress(int)), Qt::QueuedConnection);
            connect(aop, SIGNAL(cancel()), builder, SLOT(cancel()), Qt::QueuedConnection);

            DM_AbstractWorker::staticConnectWorkerToThread(builder, true, true, true);

            // start the build (result is directly loaded in the model, show 'slotModelBuilderFinished' method)
            thread->start();
        }
    }
    else
    {
        delete aop;
    }
}

void DM_ItemDrawableTreeViewController::slotRemoveTemporaryItemsInTable()
{
    // remvoe all CT_AbstractItemDrawable in the temporary list from the QTreeView
    /*DM_AsyncOperation *aop = GUI_MANAGER->requestExclusiveAsyncOperation();

    if(!m_tmpItemsToRemove.isEmpty())
    {
        GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotRemoveTemporaryItemsInTable : %1").arg(m_tmpItemsToRemove.size()));

        if(aop != NULL)
        {
            aop->progressDialog()->setCanClose(false);
            aop->progressDialog()->setLabelText(tr("Veuillez patienter pendant la suppression des éléments de la table"));
            aop->progressDialog()->setSecondLabelText("");
            aop->progressDialog()->setValue(0);
            aop->progressDialog()->show();

            // create a thread for the builder
            QThread *thread = new QThread();

            // create a builder
            DM_ItemDrawableTreeViewModelResearcher *builder = new DM_ItemDrawableTreeViewModelResearcher();
            builder->moveToThread(thread);
            builder->addData(aop, true);
            builder->setCollection(&m_collection);
            builder->setStandardItemBuilder(m_standardItemBuilder);
            builder->setItemDrawable(m_tmpItemsToRemove);

            connect(builder, SIGNAL(finished(bool)), this, SLOT(slotModelBuilderRemoveFinished(bool)), Qt::QueuedConnection);
            connect(builder, SIGNAL(progressChanged(int)), aop, SLOT(setProgress(int)), Qt::QueuedConnection);
            connect(aop, SIGNAL(cancel()), builder, SLOT(cancel()), Qt::QueuedConnection);

            DM_AbstractWorker::staticConnectWorkerToThread(builder, true, true, true);

            // start the build (result is directly removed from the model, show 'slotModelBuilderRemoveFinished' method)
            thread->start();
        }
    }
    else
    {
        delete aop;
    }*/

    //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotRemoveTemporaryItemsInTable : %1").arg(m_tmpItemsToRemove.size()));

    // if the size of CT_AbstractItemDrawable to removes is too important
    if(m_tmpItemsToRemove.size() > maxRemoveToDoInGuiThread())
    {
        //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotRemoveTemporaryItemsInTable reconstruct"));

        m_tmpItemsToRemove.clear();

        // we reconstruct the model
        if(m_model->rowCount() != 0)
            constructModel();
    }
    // else we must find the QStandardItem in the model to remove it
    else
    {
        //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotRemoveTemporaryItemsInTable removes"));

        while(!m_tmpItemsToRemove.isEmpty())
        {
            CT_AbstractItemDrawable *item = m_tmpItemsToRemove.takeLast();

            QStandardItem *si = m_treeViewManager->itemFromItemDrawable(item);

            if(si != NULL)
                m_model->removeRow(si->row());
        }

        // refresh headers
        m_treeViewManager->refreshHeaders();
    }
}
