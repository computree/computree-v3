#ifndef DM_ITEMDRAWABLETREEVIEWCONTROLLERT_HPP
#define DM_ITEMDRAWABLETREEVIEWCONTROLLERT_HPP

#include "tools/treeview/dm_itemdrawabletreeviewcontrollert.h"

template<class Model, class Item>
DM_ItemDrawableTreeViewControllerT<Model, Item>::DM_ItemDrawableTreeViewControllerT(QObject *parent) : DM_AbstractItemTreeViewController(parent)
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

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::setModel(const Model *model)
{
    m_model = (Model*)model;
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::setStandardItemBuilder(const DM_IItemDrawableStandardItemBuilderT<Item> *builder)
{
    m_standardItemBuilder = (DM_IItemDrawableStandardItemBuilderT<Item>*)builder;
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::setTreeViewManager(const DM_ITreeViewManagerT<Item> *manager)
{
    m_treeViewManager = (DM_ITreeViewManagerT<Item>*)manager;
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::setMaxRemoveToDoInGuiThread(const int &n)
{
    m_maxRemoveInGuiThread = n;
}

template<class Model, class Item>
int DM_ItemDrawableTreeViewControllerT<Model, Item>::maxRemoveToDoInGuiThread() const
{
    return m_maxRemoveInGuiThread;
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::beginAddMultipleItemDrawable()
{
    m_addMultiple = true;
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::addItemDrawable(CT_AbstractItemDrawable &item)
{
    m_tmpItemsToAdd.append(&item);

    // and if we don't add multiple items we start the timer
    if(!m_addMultiple)
        emit mustStartAddTimer();
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::endAddMultipleItemDrawable()
{
    m_addMultiple = false;

    if(!m_tmpItemsToAdd.isEmpty())
        emit mustStartAddTimer();
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::beginRemoveMultipleItemDrawable()
{
    m_removeMultiple = true;
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::removeItemDrawable(CT_AbstractItemDrawable &item)
{
    m_tmpItemsToRemove.append(&item);

    // and if we don't remove multiple items we start the timer
    if(!m_removeMultiple)
        emit mustStartRemoveTimer();
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::endRemoveMultipleItemDrawable()
{
    m_removeMultiple = false;

    if(!m_tmpItemsToRemove.isEmpty())
        emit mustStartRemoveTimer();
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::addElementsOfCollection()
{
    QVectorIterator< QList<Item*> > it(m_collection);

    while(it.hasNext())
    {
        const QList<Item*> &ll = it.next();

        if(!ll.isEmpty())
            m_model->appendRow(ll);
    }

    m_collection.clear();
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::updateElemensOfCollection()
{
    if(m_tmpItemsToUpdate.size() == m_collectionUpdate.size())
    {
        QVectorIterator< QList<Item*> > it(m_collectionUpdate);
        QListIterator< QPair<Item *, CT_AbstractItemDrawable *> > itL(m_tmpItemsToUpdate);

        while(it.hasNext()
                && itL.hasNext())
        {
            const QList<Item*> &ll = it.next();
            const QPair<Item *, CT_AbstractItemDrawable *> &pair = itL.next();

            if(!ll.isEmpty())
            {
                Item *ii = pair.first;
                Item *parent = ii->parent();
                int row = ii->row();

                parent->replaceRow(row, ll);
            }
        }

        m_collectionUpdate.clear();
        m_tmpItemsToUpdate.clear();
    }
    else
    {
        QVectorIterator< QList<Item*> > it(m_collectionUpdate);

        while(it.hasNext())
        {
            const QList<Item*> &ll = it.next();
            qDeleteAll(ll.begin(), ll.end());
        }

        m_collectionUpdate.clear();
    }
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::constructModel()
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
            aop->progressDialog()->setLabelText(QObject::tr("Veuillez patienter pendant la construction de la table"));
            aop->progressDialog()->setSecondLabelText("");
            aop->progressDialog()->setValue(0);

            QList<CT_AbstractItemDrawable*> items = m_treeViewManager->itemDrawableForTreeView();

            if(!items.isEmpty())
            {
                aop->progressDialog()->show();

                // create a thread for the builder
                QThread *thread = new QThread();

                // create a builder
                DM_ItemDrawableTreeViewModelBuilderT<Item> *builder = new DM_ItemDrawableTreeViewModelBuilderT<Item>();
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

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::refresh(const QList<QPair<Item *, CT_AbstractItemDrawable *> > &list)
{
    //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("refresh a item in the Model"));

    // add all CT_AbstractItemDrawable of the type to the QTreeView
    DM_AsyncOperation *aop = GUI_MANAGER->requestExclusiveAsyncOperation();

    if(aop != NULL)
    {
        aop->progressDialog()->setCanClose(false);
        aop->progressDialog()->setLabelText(QObject::tr("Veuillez patienter pendant le rafraichissement de la table"));
        aop->progressDialog()->setSecondLabelText("");
        aop->progressDialog()->setValue(0);
        aop->progressDialog()->show();

        m_tmpItemsToUpdate = list;

        // create a thread for the builder
        QThread *thread = new QThread();

        // create a builder
        DM_ItemDrawableTreeViewModelBuilderT<Item> *builder = new DM_ItemDrawableTreeViewModelBuilderT<Item>();
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

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::slotModelBuilderFinished(bool canceled)
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

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::slotModelBuilderUpdateFinished(bool canceled)
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

    // refresh expanded
    m_treeViewManager->refreshExpanded();
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::slotModelBuilderAddFinished(bool canceled)
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

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::slotModelBuilderRemoveFinished(bool canceled)
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
        QVectorIterator< QList<Item*> > it(m_collection);

        while(it.hasNext())
        {
            const QList<Item*> &ll = it.next();

            if(!ll.isEmpty())
                m_model->removeRow(ll.first()->row());
        }

        m_collection.clear();
        m_tmpItemsToRemove.clear();

        // refresh headers
        m_treeViewManager->refreshHeaders();
    }
}

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::slotAddTemporaryItemsInTable()
{
    // add all CT_AbstractItemDrawable in the temporary list to the QTreeView
    DM_AsyncOperation *aop = GUI_MANAGER->requestExclusiveAsyncOperation();

    if(!m_tmpItemsToAdd.isEmpty())
    {
        //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotAddTemporaryItemsInTable"));

        if(aop != NULL)
        {
            aop->progressDialog()->setCanClose(false);
            aop->progressDialog()->setLabelText(QObject::tr("Veuillez patienter pendant l'ajout des éléments à la table"));
            aop->progressDialog()->setSecondLabelText("");
            aop->progressDialog()->setValue(0);
            aop->progressDialog()->show();

            // create a thread for the builder
            QThread *thread = new QThread();

            // create a builder
            DM_ItemDrawableTreeViewModelBuilderT<Item> *builder = new DM_ItemDrawableTreeViewModelBuilderT<Item>();
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

template<class Model, class Item>
void DM_ItemDrawableTreeViewControllerT<Model, Item>::slotRemoveTemporaryItemsInTable()
{
    // remvoe all CT_AbstractItemDrawable in the temporary list from the QTreeView
    /*DM_AsyncOperation *aop = GUI_MANAGER->requestExclusiveAsyncOperation();

    if(!m_tmpItemsToRemove.isEmpty())
    {
        GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotRemoveTemporaryItemsInTable : %1").arg(m_tmpItemsToRemove.size()));

        if(aop != NULL)
        {
            aop->progressDialog()->setCanClose(false);
            aop->progressDialog()->setLabelText(QObject::tr("Veuillez patienter pendant la suppression des éléments de la table"));
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

            Item *si = m_treeViewManager->itemFromItemDrawable(item);

            if(si != NULL)
                m_model->removeRow(si->row());
        }

        // refresh headers
        m_treeViewManager->refreshHeaders();
    }
}

#endif // DM_ITEMDRAWABLETREEVIEWCONTROLLERT_HPP
