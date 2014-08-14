/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "gdocumentmanagerview.h"

#include "dm_guimanager.h"

#include "gdocumentviewforgraphics.h"
#include "gdocumentviewforitemmodel.h"
#include "view/DocumentView/ItemModelViews/gtreeview.h"
#include "view/DocumentView/GraphicsViews/3D/g3dgraphicsview.h"
#include "view/DocumentView/GraphicsViews/ggraphicsviewsynchronizedgroup.h"

#include <QMdiSubWindow>

GDocumentManagerView::GDocumentManagerView(QWidget *parent) : QMdiArea(parent), DM_DocumentManagerView()
{
    m_mutex = new QMutex(QMutex::Recursive);
    m_windowCount = 0;
    m_timer.setInterval(100);
    m_syncMan = NULL;
    m_lastDocumentActivated = NULL;
    m_manualModeEnabled = false;
    m_manualModeEnabledByStep = NULL;

    connect(this, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(subWindowHasBeenActivated(QMdiSubWindow*)));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(verifyNumberWindow()));
}

GDocumentManagerView::~GDocumentManagerView()
{
    delete m_mutex;
}

void GDocumentManagerView::setSyncManager(const GGraphicsViewSynchronizedGroup *syncMan)
{
    m_syncMan = (GGraphicsViewSynchronizedGroup*)syncMan;
}

void GDocumentManagerView::addDocumentView(GDocumentView &view, bool fromGui)
{
    view.getSubWindow()->setParent(this);
    view.setDocumentCloseFilter(this);
    view.setDocumentAddFilter(this);

    if(subWindowList().isEmpty())
    {
        view.getSubWindow()->showMaximized();
    }
    else
    {
        if((activeSubWindow() != NULL)
            && activeSubWindow()->isMaximized())
            activeSubWindow()->showNormal();

        view.getSubWindow()->showNormal();
        tileSubWindows();
    }

    connect(&view, SIGNAL(closed(DM_DocumentView*)), this, SIGNAL(documentToBeClosed(DM_DocumentView*)), Qt::DirectConnection);
    connect(&view, SIGNAL(destroyed()), this, SLOT(slotDocumentDestroyed()), Qt::DirectConnection);
    connect(&view, SIGNAL(destroyed()), &m_timer, SLOT(start()));

    if(m_manualModeEnabled)
    {
        m_docToCloseAfterQuitManualMode.append(&view);

        if(fromGui)
            m_docFromGuiAddedInManualMode.append(&view);
    }
    else
    {
        m_docAddedInNormalMode.append(&view);
    }

    emit documentAdded(&view);
    ++m_windowCount;

    m_lastDocumentActivated = view.getSubWindow();
    setActiveSubWindow(view.getSubWindow());

    emit documentActivated(&view);
}

bool GDocumentManagerView::addAllItemDrawableOfResultToActiveDocument(Result &res, DM_AsynchroneProgress &progress)
{
    DM_DocumentView *activeDoc = getActiveDocumentView();

    return addAllItemDrawableOfResultToDocument(res, activeDoc, progress);
}

bool GDocumentManagerView::addAllItemDrawableOfModelToDocument(Result &res, IItemModel &model, DM_DocumentView *doc, DM_AsynchroneProgress &progress)
{
    if((doc != NULL)
            && !res.isBusy())
    {
        progress.setProgress(0);

        res.setBusy(true);

        int n = res.recursiveBeginIterateItemDrawableWithModel(model);

        if(n != 0)
        {
            int i = 0;

            if(n > 1)
                doc->beginAddMultipleItemDrawable();

            ItemDrawable *item = NULL;

            while((item = res.recursiveNextItemDrawable()) != NULL)
            {
                doc->addItemDrawable(*item);

                ++i;
                progress.setProgress(((float)(i*100))/n);
            }

            if(n > 1)
                doc->endAddMultipleItemDrawable();

            doc->redrawGraphics();
        }

        progress.setProgress(100);

        res.setBusy(false);

        return true;
    }

    return false;
}

bool GDocumentManagerView::addAllItemDrawableOfResultToDocument(Result &res, DM_DocumentView *doc, DM_AsynchroneProgress &progress)
{
    if((doc != NULL)
            && !res.isBusy())
    {
        progress.setProgress(0);

        res.setBusy(true);

        int n = res.beginIterateItemDrawableList();

        if(n != 0)
        {
            int i = 0;

            if(n > 1)
                doc->beginAddMultipleItemDrawable();

            ItemDrawable *item = NULL;

            while((item = res.nextItemDrawable()) != NULL)
            {
                doc->addItemDrawable(*item);

                ++i;
                progress.setProgress(((float)(i*100))/n);
            }

            if(n > 1)
                doc->endAddMultipleItemDrawable();

            doc->redrawGraphics();
        }

        progress.setProgress(100);

        res.setBusy(false);

        return true;
    }

    return false;
}

bool GDocumentManagerView::addAllItemDrawableOfListToActiveDocument(QList<ItemDrawable*> &itemList, DM_AsynchroneProgress &progress)
{
    DM_DocumentView *activeDoc = getActiveDocumentView();

    return addAllItemDrawableOfListToDocument(itemList, activeDoc, progress);
}

bool GDocumentManagerView::addAllItemDrawableOfListToDocument(QList<ItemDrawable*> &itemList, DM_DocumentView *doc, DM_AsynchroneProgress &progress)
{
    if(doc != NULL)
    {
        progress.setProgress(0);

        if(!itemList.isEmpty())
        {
            int n = itemList.size();
            int i = 0;

            QListIterator<ItemDrawable*> it(itemList);

            if(n > 1)
                doc->beginAddMultipleItemDrawable();

            while(it.hasNext())
            {
                doc->addItemDrawable(*(it.next()));

                ++i;
                progress.setProgress(((float)(i*100))/n);
            }

            if(n > 1)
                doc->endAddMultipleItemDrawable();

            doc->redrawGraphics();
        }

        progress.setProgress(100);

        return true;
    }

    return false;
}

bool GDocumentManagerView::removeAllItemDrawableOfResultFromDocuments(Result &res, DM_AsynchroneProgress &progress)
{
    progress.setProgress(0);

    QList<QMdiSubWindow *> list = subWindowList();
    QListIterator<QMdiSubWindow *> it(list);

    int i = 0;
    int n = list.size();

    while(it.hasNext())
    {
        DM_DocumentView *dv = ((MyQMdiSubWindow*)it.next())->getDocumentView();
        dv->removeAllItemDrawableOfResult(res);

        ++i;
        progress.setProgress(((float)(i*100))/n);
    }

    redrawAllDocument();

    progress.setProgress(100);

    return true;
}

bool GDocumentManagerView::removeAllItemDrawableOfModelFromDocuments(IItemModel &model, DM_AsynchroneProgress &progress)
{
    progress.setProgress(0);

    QList<QMdiSubWindow *> list = subWindowList();
    QListIterator<QMdiSubWindow *> it(list);

    int i = 0;
    int n = list.size();

    while(it.hasNext())
    {
        DM_DocumentView *dv = ((MyQMdiSubWindow*)it.next())->getDocumentView();
        dv->removeAllItemDrawableOfModel(model);

        ++i;
        progress.setProgress(((float)(i*100))/n);
    }

    redrawAllDocument();

    progress.setProgress(100);

    return true;
}

bool GDocumentManagerView::removeAllItemDrawableOfModelFromDocument(IItemModel &model, DM_DocumentView *doc, DM_AsynchroneProgress &progress)
{
    progress.setProgress(0);

    if(doc != NULL)
    {
        doc->removeAllItemDrawableOfModel(model);

        m_mutex->lock();
        doc->redrawGraphics();
        m_mutex->unlock();
    }

    progress.setProgress(100);

    return true;
}

bool GDocumentManagerView::removeAllItemDrawableOfListFromDocuments(QList<ItemDrawable *> &itemList, DM_AsynchroneProgress &progress)
{
    progress.setProgress(0);

    if(!itemList.isEmpty())
    {
        int i = 0;
        int n = itemList.size();

        QListIterator<ItemDrawable*> itItem(itemList);

        while(itItem.hasNext())
        {
            ItemDrawable *item = itItem.next();

            if(item->isDisplayed())
            {
                QList<DocumentInterface*> itemDocList = item->document();
                QListIterator<DocumentInterface*> itDoc(itemDocList);

                while(itDoc.hasNext())
                {
                    itDoc.next()->removeItemDrawable(*item);
                }
            }

            ++i;
            progress.setProgress(((float)(i*100))/n);
        }

        redrawAllDocument();
    }

    progress.setProgress(100);

    return true;
}

DM_DocumentView* GDocumentManagerView::getActiveDocumentView()
{
    if(activeSubWindow() == NULL)
    {
        QList<QMdiSubWindow *> list = subWindowList();

        if((m_lastDocumentActivated != NULL)
                && list.contains(m_lastDocumentActivated))
        {
            setActiveSubWindow(m_lastDocumentActivated);
        }
        else if(!list.isEmpty())
        {
            m_lastDocumentActivated = list.first();
            setActiveSubWindow(m_lastDocumentActivated);
        }
    }

    if(activeSubWindow() != NULL)
    {
        m_lastDocumentActivated = activeSubWindow();

        return ((MyQMdiSubWindow*)activeSubWindow())->getDocumentView();
    }
    else
    {
        m_lastDocumentActivated = NULL;
    }

    return NULL;
}

DM_DocumentView* GDocumentManagerView::getDocumentView(int index) const
{
    QList<QMdiSubWindow *> list = subWindowList();

    if((index >=0) && (index < list.size()))
        return ((MyQMdiSubWindow*)list.at(index))->getDocumentView();

    return NULL;
}

int GDocumentManagerView::nbDocumentView() const
{
    return subWindowList().size();
}

bool GDocumentManagerView::containsDocument(DM_Document *document) const
{
    QList<QMdiSubWindow *> list = subWindowList();
    QListIterator<QMdiSubWindow *> it(list);

    while(it.hasNext())
    {
        if(((MyQMdiSubWindow*)it.next())->getDocumentView() == document)
            return true;
    }

    return false;
}

void GDocumentManagerView::redrawAllDocument()
{
    m_mutex->lock();

    QList<QMdiSubWindow*> list = subWindowList();
    QListIterator<QMdiSubWindow*> it(list);

    while(it.hasNext())
    {
        ((MyQMdiSubWindow*)it.next())->getDocumentView()->redrawGraphics();
    }

    m_mutex->unlock();
}

DocumentInterface *GDocumentManagerView::new3DDocument(bool fromGui)
{
    GDocumentViewForGraphics *doc = new GDocumentViewForGraphics(*this, tr("Document %1").arg(GDocumentView::NUMBER), "NORMAL");
    doc->init();
    G3DGraphicsView *graphics = new G3DGraphicsView(doc->getSubWindow());

    doc->addGraphics(graphics);

    connect(doc, SIGNAL(syncEnabled(const GDocumentViewForGraphics*)), m_syncMan, SLOT(addAllGraphicsFromDocumentView(const GDocumentViewForGraphics*)));
    connect(doc, SIGNAL(syncDisabled(const GDocumentViewForGraphics*)), m_syncMan, SLOT(removeDocumentView(const GDocumentViewForGraphics*)));

    addDocumentView(*doc, fromGui);

    return doc;
}

DocumentInterface *GDocumentManagerView::new2DDocument(bool fromGui)
{
    GDocumentViewForGraphics *doc = new GDocumentViewForGraphics(*this, tr("Document %1 (2D)").arg(GDocumentView::NUMBER), "2D");
    doc->init();
    G3DGraphicsView *graphics = new G3DGraphicsView(doc->getSubWindow());
    graphics->active2dView(true);

    doc->addGraphics(graphics);

    connect(doc, SIGNAL(syncEnabled(const GDocumentViewForGraphics*)), m_syncMan, SLOT(addAllGraphicsFromDocumentView(const GDocumentViewForGraphics*)));
    connect(doc, SIGNAL(syncDisabled(const GDocumentViewForGraphics*)), m_syncMan, SLOT(removeDocumentView(const GDocumentViewForGraphics*)));

    addDocumentView(*doc, fromGui);

    return doc;
}

DocumentInterface *GDocumentManagerView::newTreeViewDocument(bool fromGui)
{
    GDocumentViewForItemModel *doc = new GDocumentViewForItemModel(*this, tr("Document %1 (ItemModel)").arg(GDocumentView::NUMBER));
    doc->init();
    GTreeView *treeView = new GTreeView(doc->getSubWindow());

    doc->addView(treeView);

    treeView->init();

    connect(treeView, SIGNAL(syncWith(const GItemModelView*)), m_syncMan, SLOT(syncItemModelWith(const GItemModelView*)));

    addDocumentView(*doc, fromGui);

    return doc;
}

void GDocumentManagerView::lockAllDocuments()
{
    m_mutex->lock();

    QList<QMdiSubWindow*> list = subWindowList();
    QListIterator<QMdiSubWindow*> it(list);

    while(it.hasNext())
    {
        ((MyQMdiSubWindow*)it.next())->getDocumentView()->lock();
    }

    m_mutex->unlock();
}

void GDocumentManagerView::unlockAllDocuments()
{
    m_mutex->lock();

    QList<QMdiSubWindow*> list = subWindowList();
    QListIterator<QMdiSubWindow*> it(list);

    while(it.hasNext())
    {
        ((MyQMdiSubWindow*)it.next())->getDocumentView()->unlock();
    }

    m_mutex->unlock();
}

DocumentInterface* GDocumentManagerView::new3DDocument()
{
    return new3DDocument(false);
}

DocumentInterface* GDocumentManagerView::new2DDocument()
{
    return new2DDocument(false);
}

DocumentInterface* GDocumentManagerView::newTreeViewDocument()
{
    return newTreeViewDocument(false);
}

int GDocumentManagerView::nDocuments() const
{
    return nbDocumentView();
}

DocumentInterface* GDocumentManagerView::documentAt(const int &index) const
{
    return getDocumentView(index);
}

QList<DocumentInterface*> GDocumentManagerView::documents() const
{
    QList<QMdiSubWindow*> list = subWindowList();
    QList<DocumentInterface*> lRet;

    while(!list.isEmpty())
    {
        lRet.append(((MyQMdiSubWindow*)list.takeFirst())->getDocumentView());
    }

    return lRet;
}

DocumentInterface* GDocumentManagerView::activeDocument()
{
    return getActiveDocumentView();
}

void GDocumentManagerView::activateDocument(DocumentInterface *doc)
{
    QMdiSubWindow *subW = subWindowFromDocument(doc);

    if(subW != NULL)
        setActiveSubWindow(subW);
}

bool GDocumentManagerView::closeDocument(DocumentInterface *doc)
{
    QMdiSubWindow *subW = subWindowFromDocument(doc);

    if(subW != NULL)
        return subW->close();

    return true;
}

bool GDocumentManagerView::closeDocument(const int &index)
{
    return subWindowList().at(index)->close();
}

bool GDocumentManagerView::canClose(const DM_Document *document) const
{
    if(m_manualModeEnabled)
    {
        if(m_docAddedInNormalMode.contains((GDocumentView*)document))
            return (m_docAddedInNormalMode.size() > 1);

        if(m_docFromGuiAddedInManualMode.contains((GDocumentView*)document))
            return true;

        if(m_docToCloseAfterQuitManualMode.contains((GDocumentView*)document))
            return false;
    }

    return (nbDocumentView() > 1);
}

bool GDocumentManagerView::canAddItemDrawable(const DM_Document *document, const ItemDrawable *item) const
{
    Q_UNUSED(item)

    return ((DM_DocumentView*)document)->isVisible();
}

QMdiSubWindow* GDocumentManagerView::subWindowFromDocument(DocumentInterface *doc) const
{
    QList<QMdiSubWindow*> list = subWindowList();

    while(!list.isEmpty())
    {
        MyQMdiSubWindow *w = (MyQMdiSubWindow*)list.takeFirst();

        if(w->getDocumentView() == doc)
            return w;
    }

    return NULL;
}

void GDocumentManagerView::stepRequiredManualMode(Step *step)
{
    if((m_manualModeEnabledByStep != NULL)
            && (m_manualModeEnabledByStep != step))
    {
        stepFinished(m_manualModeEnabledByStep);
    }

    if(m_manualModeEnabledByStep == NULL)
    {
        m_manualModeEnabled = true;
        m_manualModeEnabledByStep = step;

        // hide all documents
        QListIterator<GDocumentView*> it(m_docAddedInNormalMode);

        while(it.hasNext())
        {
            GDocumentView *dView = it.next();
            dView->getSubWindow()->showNormal();
            dView->getSubWindow()->hide();

            emit documentHidden(dView);
        }
    }
}

void GDocumentManagerView::stepFinished(Step *step)
{
    if((m_manualModeEnabledByStep != NULL)
            && (m_manualModeEnabledByStep == step))
    {
        // close all documents added in manual mode
        while(!m_docToCloseAfterQuitManualMode.isEmpty())
            m_docToCloseAfterQuitManualMode.takeFirst()->getSubWindow()->close();

        m_docFromGuiAddedInManualMode.clear();

        // show all documents
        QListIterator<GDocumentView*> it(m_docAddedInNormalMode);

        while(it.hasNext())
        {
            GDocumentView *dView = it.next();
            dView->getSubWindow()->show();

            emit documentShowned(dView);
        }

        m_manualModeEnabledByStep = NULL;
        m_manualModeEnabled = false;
    }
}

void GDocumentManagerView::slotDocumentDestroyed()
{
    m_docToCloseAfterQuitManualMode.removeOne((GDocumentView*)sender());
    m_docFromGuiAddedInManualMode.removeOne((GDocumentView*)sender());
    m_docAddedInNormalMode.removeOne((GDocumentView*)sender());
}

void GDocumentManagerView::subWindowHasBeenActivated(QMdiSubWindow *window)
{
    Q_UNUSED(window)

    emit documentActivated(getActiveDocumentView());
}

void GDocumentManagerView::verifyNumberWindow()
{
    if(m_windowCount != nbDocumentView())
    {
        m_windowCount = nbDocumentView();
        m_timer.stop();
        emit documentRemoved();
    }
    else
    {
        m_timer.start();
    }
}
