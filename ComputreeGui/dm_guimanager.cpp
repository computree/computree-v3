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


#include "dm_guimanager.h"

#include "view/MainView/gmainprogressdialog.h"

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.h"
#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.h"
#include "ct_result/ct_resultgroup.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtConcurrentRun>
#else
#include <QtConcurrent/QtConcurrentRun>
#endif

#include <QEventLoop>

DM_GuiManager* DM_GuiManager::_uniqueInstance = NULL;

DM_GuiManager::~DM_GuiManager()
{
    qDeleteAll(_contextList.begin(), _contextList.end());
}

bool DM_GuiManager::isAsyncOperationInProgress() const
{
    return (m_currentAsyncOperation != NULL);
}

DM_AsyncOperation* DM_GuiManager::requestExclusiveAsyncOperation(const DM_AbstractAsyncOperationOptions *options, bool wait)
{
    DM_AbstractAsyncOperationOptions *notConstOptions = (DM_AbstractAsyncOperationOptions*)options;
    DM_AbstractAsyncOperationOptions defaultOptions;

    if(notConstOptions == NULL)
        notConstOptions = &defaultOptions;

    // mutex to protect the tokens list
    QMutexLocker locker(&m_asyncOperationTokenMutex);

    bool pass = notConstOptions->pass();

    if(!pass
        && !wait)
        return NULL;

    bool canceled = false;

    DM_GuiManagerToken *token = new DM_GuiManagerToken(notConstOptions);
    m_tokens.append(token);

    if(!pass
        || (m_tokens.size() > 1))
    {
        QEventLoop event;

        do
        {
            DM_GuiManagerToken *lastToken = (m_tokens.size() == 1) ? NULL : m_tokens.at(m_tokens.size()-2);

            if(lastToken != NULL)
                connect(lastToken, SIGNAL(destroyed()), &event, SLOT(quit()), Qt::DirectConnection);
            else
                connect(token, SIGNAL(passed()), &event, SLOT(quit()), Qt::DirectConnection);

            locker.unlock();
            event.exec();
            locker.relock();

        }while(!notConstOptions->pass() || canceled);
    }

    if(!canceled)
    {
        _progressDialog->setCanClose(false);
        _progressDialog->setValue(0);
        _progressDialog->setLabelText("");
        _progressDialog->setSecondLabelText("");

        m_currentAsyncOperation = new DM_AsyncOperation();
        m_currentAsyncOperation->setProgressDialog(_progressDialog);

        _progressDialog->setData(m_currentAsyncOperation);

        connect(m_currentAsyncOperation, SIGNAL(destroyed()), this, SLOT(slotCurrentAsyncOperationDestroyed()), Qt::DirectConnection);
        connect(m_currentAsyncOperation, SIGNAL(destroyed()), this, SLOT(slotCurrentAsyncOperationReleased()), Qt::QueuedConnection);

        return m_currentAsyncOperation;
    }
    else
    {
        m_tokens.removeOne(token);
        delete token;
    }

    return NULL;
}

bool DM_GuiManager::asyncAddAllItemDrawableOfResultOnView(CT_AbstractResult &res, DM_Context *context)
{
    if(!getStepManager()->isRunning()
            || !getStepManager()->getOptions().isAutoClearResultFromMemoryEnable())
    {
        DM_AsyncOperation *aop = requestExclusiveAsyncOperation();

        if(aop != NULL)
        {
            if(context != NULL)
                connect(aop, SIGNAL(destroyed()), context, SLOT(setActionFinished()), Qt::DirectConnection);

            ActionItemDrawable infoActionID = ActionItemDrawable(aop,
                                                                 &res,
                                                                 NULL,
                                                                 ".",
                                                                 getDocumentManagerView(),
                                                                 &_asyncProgress);

            initProgressDialog(_progressDialog, tr("Veuillez patienter pendant l'ajout du resultat au document actif."));

            _future = QtConcurrent::run(staticAddAllItemDrawableOfResultOnView, infoActionID);
            _futureWatcher.setFuture(_future);

            return true;
        }
    }

    return false;
}

bool DM_GuiManager::asyncAddAllItemDrawableOfModelOnView(CT_AbstractResult &res,
                                                         CT_OutAbstractItemModel &model,
                                                         DM_DocumentView &view,
                                                         DM_Context *context)
{
    if(model.result() == NULL)
        return false;

    if((!getStepManager()->isRunning()
            || !getStepManager()->getOptions().isAutoClearResultFromMemoryEnable()))
    {
        DM_AsyncOperation *aop = requestExclusiveAsyncOperation();

        if(aop != NULL)
        {
            if(context != NULL)
                connect(aop, SIGNAL(destroyed()), context, SLOT(setActionFinished()), Qt::DirectConnection);

            ActionItemDrawable infoActionID = ActionItemDrawable(aop,
                                                                 &res,
                                                                 NULL,
                                                                 ".",
                                                                 getDocumentManagerView(),
                                                                 &_asyncProgress);
            infoActionID.setItemModel(&model);
            infoActionID.setDocumentView(&view);

            initProgressDialog(_progressDialog, tr("Veuillez patienter pendant l'ajout des items au document actif."));

            _future = QtConcurrent::run(staticAddAllItemDrawableOfModelOnView, infoActionID);
            _futureWatcher.setFuture(_future);

            return true;
        }
    }

    return false;
}

bool DM_GuiManager::asyncAddAllItemDrawableOfListOnView(QList<CT_AbstractItemDrawable*> &itemList, DM_DocumentView *view, DM_Context *context)
{
    if((!getStepManager()->isRunning()
            || !getStepManager()->getOptions().isAutoClearResultFromMemoryEnable()))
    {
        DM_AsyncOperation *aop = requestExclusiveAsyncOperation();

        if(aop != NULL)
        {
            if(context != NULL)
                connect(aop, SIGNAL(destroyed()), context, SLOT(setActionFinished()), Qt::DirectConnection);

            ActionItemDrawable infoActionID = ActionItemDrawable(aop,
                                                                 NULL,
                                                                 &itemList,
                                                                 ".",
                                                                 getDocumentManagerView(),
                                                                 &_asyncProgress);
            infoActionID.m_docView = view;

            initProgressDialog(_progressDialog, tr("Veuillez patienter pendant l'ajout des CT_AbstractItemDrawable au document actif."));

            _future = QtConcurrent::run(staticAddAllItemDrawableOfListOnView, infoActionID);
            _futureWatcher.setFuture(_future);

            return true;
        }
    }

    return false;
}

bool DM_GuiManager::asyncRemoveAllItemDrawableOfResultFromView(CT_AbstractResult &res, DM_Context *context)
{
    DM_AsyncOperation *aop = requestExclusiveAsyncOperation();

    if(aop != NULL)
    {
        if(context != NULL)
            connect(aop, SIGNAL(destroyed()), context, SLOT(setActionFinished()), Qt::DirectConnection);

        ActionItemDrawable infoActionID = ActionItemDrawable(aop,
                                                             &res,
                                                             NULL,
                                                             ".",
                                                             getDocumentManagerView(),
                                                             &_asyncProgress);

        initProgressDialog(_progressDialog, tr("Veuillez patienter pendant la suppression du resultat du(des) document(s)."));

        _future = QtConcurrent::run(staticRemoveAllItemDrawableOfResultFromView, infoActionID);
        _futureWatcher.setFuture(_future);

        return true;
    }

    return false;
}

bool DM_GuiManager::asyncRemoveAllItemDrawableOfModelFromAllViews(CT_OutAbstractItemModel &model, DM_Context *context)
{
    if(model.result() == NULL)
        return false;

    DM_AsyncOperation *aop = requestExclusiveAsyncOperation();

    if(aop != NULL)
    {
        if(context != NULL)
            connect(aop, SIGNAL(destroyed()), context, SLOT(setActionFinished()), Qt::DirectConnection);

        ActionItemDrawable infoActionID = ActionItemDrawable(aop,
                                                             NULL,
                                                             NULL,
                                                             ".",
                                                             getDocumentManagerView(),
                                                             &_asyncProgress);
        infoActionID.setItemModel(&model);

        initProgressDialog(_progressDialog, tr("Veuillez patienter pendant la suppression des items du(des) document(s)."));

        _future = QtConcurrent::run(staticRemoveAllItemDrawablOfModelFromAllViews, infoActionID);
        _futureWatcher.setFuture(_future);

        return true;
    }

    return false;
}

bool DM_GuiManager::asyncRemoveAllItemDrawableOfModelFromView(CT_OutAbstractItemModel &model, DM_DocumentView &view, DM_Context *context)
{
    if(model.result() == NULL)
        return false;

    DM_AsyncOperation *aop = requestExclusiveAsyncOperation();

    if(aop != NULL)
    {
        if(context != NULL)
            connect(aop, SIGNAL(destroyed()), context, SLOT(setActionFinished()), Qt::DirectConnection);

        ActionItemDrawable infoActionID = ActionItemDrawable(aop,
                                                             NULL,
                                                             NULL,
                                                             ".",
                                                             getDocumentManagerView(),
                                                             &_asyncProgress);
        infoActionID.setItemModel(&model);
        infoActionID.setDocumentView(&view);

        initProgressDialog(_progressDialog, tr("Veuillez patienter pendant la suppression des items du document."));

        _future = QtConcurrent::run(staticRemoveAllItemDrawableOfModelFromView, infoActionID);
        _futureWatcher.setFuture(_future);

        return true;
    }

    return false;
}

bool DM_GuiManager::asyncRemoveAllItemDrawableOfListOnView(QList<CT_AbstractItemDrawable*> &itemList, DM_Context *context)
{
    DM_AsyncOperation *aop = requestExclusiveAsyncOperation();

    if(aop != NULL)
    {
        if(context != NULL)
            connect(aop, SIGNAL(destroyed()), context, SLOT(setActionFinished()), Qt::DirectConnection);

        ActionItemDrawable infoActionID = ActionItemDrawable(aop,
                                                             NULL,
                                                             &itemList,
                                                             ".",
                                                             getDocumentManagerView(),
                                                             &_asyncProgress);

        initProgressDialog(_progressDialog, tr("Veuillez patienter pendant la suppression des CT_AbstractItemDrawable du(des) document(s)."));

        _future = QtConcurrent::run(staticRemoveAllItemDrawableOfListFromView, infoActionID);
        _futureWatcher.setFuture(_future);

        return true;
    }

    return false;
}

bool DM_GuiManager::asyncRemoveStep(CT_VirtualAbstractStep &step, DM_Context *context)
{
    DM_AbstractAsyncOperationOptions options;
    options.setWaitUntilStepManagerIsFinished(true);

    DM_AsyncOperation *aop = requestExclusiveAsyncOperation(&options, false);

    if(aop != NULL)
    {
        if(context != NULL)
            connect(aop, SIGNAL(destroyed()), context, SLOT(setActionFinished()), Qt::DirectConnection);

        ActionStep infoActionStep = ActionStep(aop,
                                               &step,
                                               getStepManager(),
                                               getDocumentManagerView(),
                                               &_asyncProgress,
                                               &_asyncSecondProgress,
                                               0,
                                               0);

        initProgressDialog(_progressDialog, tr("Veuillez patienter pendant la suppression de l'etape."));

        recursiveDeleteStepConfigurationDialog(step);

        _future = QtConcurrent::run(staticClearResultMemoryAndRemoveStep, infoActionStep);
        _futureWatcher.setFuture(_future);

        return true;
    }

    return false;
}

int DM_GuiManager::asyncRemoveAllStep(DM_Context *context)
{
    DM_AbstractAsyncOperationOptions options;
    options.setWaitUntilStepManagerIsFinished(true);

    DM_AsyncOperation *aop = requestExclusiveAsyncOperation(&options, false);

    if(aop != NULL)
    {
        if(context != NULL)
            connect(aop, SIGNAL(destroyed()), context, SLOT(setActionFinished()), Qt::DirectConnection);

        ActionStep infoActionStep = ActionStep(aop,
                                               NULL,
                                               getStepManager(),
                                               getDocumentManagerView(),
                                               &_asyncProgress,
                                               &_asyncSecondProgress,
                                               0,
                                               0);

        if(!infoActionStep._stepManager->getStepRootList().isEmpty())
        {
            initProgressDialog(_progressDialog, tr("Veuillez patienter pendant la suppression des etapes."));

            QList<CT_VirtualAbstractStep*> stepRootList = infoActionStep._stepManager->getStepRootList();
            QListIterator<CT_VirtualAbstractStep*> it(stepRootList);

            while(it.hasNext())
            {
                CT_VirtualAbstractStep *step = it.next();

                recursiveDeleteStepConfigurationDialog(*step);
            }

            _future = QtConcurrent::run(staticClearResultMemoryAndRemoveStep, infoActionStep);
            _futureWatcher.setFuture(_future);

            return 1;
        }

        return -1;
    }

    return 0;
}

bool DM_GuiManager::asyncLoadResultStep(CT_AbstractStepSerializable &step, DM_Context *context)
{
    DM_AbstractAsyncOperationOptions options;
    options.setWaitUntilStepManagerIsFinished(true);

    DM_AsyncOperation *aop = requestExclusiveAsyncOperation(&options, false);

    if(aop != NULL)
    {
        if(context != NULL)
            connect(aop, SIGNAL(destroyed()), context, SLOT(setActionFinished()), Qt::DirectConnection);

        initProgressDialog(_progressDialog, tr("Veuillez patienter pendant le chargement des resultats."));

        connect(getStepManager(), SIGNAL(loadResultInProgress(int)), &_asyncProgress, SLOT(setProgress(int)));
        connect(getStepManager(), SIGNAL(completed()), aop, SLOT(deleteLater()));

        getStepManager()->loadSerializedResultFromStep(step);

        return true;
    }

    return false;
}

bool DM_GuiManager::asyncExport(CT_AbstractExporter *exporter,
                                DM_Context *context)
{
    exporter->clearErrorMessage();

    if(!exporter->configureExport())
        return false;

    DM_AsyncOperation *aop = requestExclusiveAsyncOperation();

    if(aop != NULL)
    {
        if(context != NULL)
            connect(aop, SIGNAL(destroyed()), context, SLOT(setActionFinished()), Qt::DirectConnection);

        ActionItemDrawable infoActionID = ActionItemDrawable(aop,
                                                             NULL,
                                                             NULL,
                                                             "",
                                                             NULL,
                                                             &_asyncProgress);

        infoActionID.setIExporter(exporter);

        initProgressDialog(_progressDialog, tr("Veuillez patienter pendant l'exportation."));

        connect(exporter, SIGNAL(exportInProgress(int)), _progressDialog, SLOT(setValue(int)));

        _future = QtConcurrent::run(staticExport, infoActionID);
        _futureWatcher.setFuture(_future);

        return true;
    }

    return false;
}

bool DM_GuiManager::setColorOnAllItemDrawableOnAllDocumentsThatModelCorresponding(const CT_OutAbstractItemModel *model, const QColor &color)
{
    if(!isRunning())
    {
        int size = getDocumentManagerView()->nbDocumentView();

        for(int i=0; i<size; ++i)
        {
            DM_DocumentView *doc = getDocumentManagerView()->getDocumentView(i);

            QListIterator<CT_AbstractItemDrawable*> it(doc->getItemDrawable());

            while(it.hasNext())
            {
                CT_AbstractItemDrawable *item = it.next();

                if(item->model() == model)
                    item->setColor(color);
            }

            doc->redrawGraphics();
        }

        return true;
    }

    return false;
}

bool DM_GuiManager::setColorListOnAllItemDrawableOnAllDocumentsThatModelCorresponding(const CT_OutAbstractItemModel *model, const QList<QColor> &colorList)
{
    if(!isRunning()
            && !colorList.isEmpty())
    {
        int cSize = colorList.size();
        int size = getDocumentManagerView()->nbDocumentView();

        int j = 0;

        for(int i=0; i<size; ++i)
        {
            DM_DocumentView *doc = getDocumentManagerView()->getDocumentView(i);

            QListIterator<CT_AbstractItemDrawable*> it(doc->getItemDrawable());

            while(it.hasNext())
            {
                CT_AbstractItemDrawable *item = it.next();

                if(item->model() == model)
                {
                    item->setColor(colorList.at(j));
                    ++j;

                    if(j == cSize)
                        j = 0;
                }
            }

            doc->redrawGraphics();
        }

        return true;
    }

    return false;
}

bool DM_GuiManager::editItemDrawableModelOfResult(CT_AbstractResult &res)
{
    getItemDrawableModelManager()->addResult(&res);
    return true;
}

bool DM_GuiManager::removeEditItemDrawableModelOfResult(CT_AbstractResult &res)
{
    getItemDrawableModelManager()->removeResult(&res);
    return true;
}

bool DM_GuiManager::cleanItemDrawableOfAllDocuments()
{
    if(!isRunning())
    {
        int size = getDocumentManagerView()->nbDocumentView();

        for(int i=0; i<size; ++i)
        {
            DM_DocumentView *doc = getDocumentManagerView()->getDocumentView(i);

            doc->removeAllItemDrawable();

            doc->redrawGraphics();
        }

        return true;
    }

    return false;
}

bool DM_GuiManager::isRunning()
{
    return _futureWatcher.isRunning() || isAsyncOperationInProgress();
}

void DM_GuiManager::redrawAllDocuments()
{
    getDocumentManagerView()->redrawAllDocument();
}

CDM_PluginManager* DM_GuiManager::getPluginManager() const
{
    return getMainWindow()->getPluginManager();
}

///////////////// PROTECTED /////////////////

void DM_GuiManager::init()
{
    m_currentAsyncOperation = NULL;

    connect(getStepManager(), SIGNAL(resultToBeClearedFromMemory(const CT_AbstractResult*)), this, SLOT(resultToBeClearedFromMemory(const CT_AbstractResult*)), Qt::DirectConnection);
    connect(getStepManager(), SIGNAL(resultToBeRemoved(const CT_AbstractResult*)), this, SLOT(resultToBeRemoved(const CT_AbstractResult*)), Qt::DirectConnection);
    connect(getStepManager(), SIGNAL(completed(bool)), this, SLOT(stepManagerCompletedLoadResultStep()), Qt::DirectConnection);

    _progressDialog = getMainWindow()->createWaitProgressDialog();
    _progressDialog->setWindowModality(Qt::WindowModal);

    m_asyncWaitDialog = getMainWindow()->createWaitProgressDialog();
    m_asyncWaitDialog->setWindowModality(Qt::WindowModal);

    connect(&_futureWatcher, SIGNAL(finished()), &_asyncProgress, SLOT(finish()), Qt::QueuedConnection);

    connect(&_asyncProgress, SIGNAL(inProgress(int)), _progressDialog, SLOT(setValue(int)), Qt::QueuedConnection);
    connect(&_asyncProgress, SIGNAL(textChanged(QString)), _progressDialog, SLOT(setLabelText(QString)), Qt::QueuedConnection);

    connect(&_asyncSecondProgress, SIGNAL(textChanged(QString)), _progressDialog, SLOT(setSecondLabelText(QString)), Qt::QueuedConnection);
    connect(&_asyncSecondProgress, SIGNAL(inProgress(int)), _progressDialog, SLOT(setSecondValue(int)), Qt::QueuedConnection);
}

///////////////// PRIVATE /////////////////

void DM_GuiManager::initProgressDialog(GMainProgressDialog *dialog, QString text, QString secondText)
{
    if(dialog != NULL)
    {
        dialog->setCanClose(false);
        dialog->setLabelText(text);
        dialog->setSecondLabelText(secondText);
        dialog->setValue(0);
        dialog->show();
    }
}

void DM_GuiManager::addNewContext(DM_Context *context)
{
    if(context != NULL)
    {
        _contextList.append(context);

        if(_contextList.size() == 11)
        {
            delete _contextList.takeFirst();
        }
    }
}

void DM_GuiManager::recursiveDeleteStepConfigurationDialog(CT_VirtualAbstractStep &step)
{
    step.aboutToBeDeleted();

    QList<CT_VirtualAbstractStep*> stepList = step.getStepChildList();
    QListIterator<CT_VirtualAbstractStep*> it(stepList);

    while(it.hasNext())
    {
        recursiveDeleteStepConfigurationDialog(*(it.next()));
    }
}

void DM_GuiManager::staticRemoveResultFromOtherView(ActionItemDrawable info)
{
    if(info._res != NULL)
    {
        if(info._res->isVisible())
        {
            info._progress->setText(QObject::tr("Suppression du resultat %1 des autres vues.").arg(info._res->getName()));

            if(info._res->isVisible())
            {
                info._view->removeAllItemDrawableOfResultFromDocuments(*info._res, *info._progress);
            }

            info._progress->setText("");
        }
    }

    delete info._aop;
}

void DM_GuiManager::staticAddAllItemDrawableOfResultOnView(ActionItemDrawable info)
{
    info._view->addAllItemDrawableOfResultToActiveDocument(*(info._res), *(info._progress));

    delete info._aop;
}

void DM_GuiManager::staticAddAllItemDrawableOfModelOnView(ActionItemDrawable info)
{
    info._view->addAllItemDrawableOfModelToDocument(*(info._res), *(info._itemModel), info.m_docView, *(info._progress));

    delete info._aop;
}

void DM_GuiManager::staticAddAllItemDrawableOfListOnView(ActionItemDrawable info)
{
    if(info.m_docView == NULL)
        info._view->addAllItemDrawableOfListToActiveDocument(info._itemList, *(info._progress));
    else
        info._view->addAllItemDrawableOfListToDocument(info._itemList, info.m_docView, *(info._progress));

    delete info._aop;
}

void DM_GuiManager::staticRemoveAllItemDrawableOfResultFromView(ActionItemDrawable info)
{
    info._view->removeAllItemDrawableOfResultFromDocuments(*(info._res), *(info._progress));

    delete info._aop;
}

void DM_GuiManager::staticRemoveAllItemDrawablOfModelFromAllViews(ActionItemDrawable info)
{
    info._view->removeAllItemDrawableOfModelFromDocuments(*(info._itemModel), *(info._progress));

    delete info._aop;
}

void DM_GuiManager::staticRemoveAllItemDrawableOfModelFromView(ActionItemDrawable info)
{
    info._view->removeAllItemDrawableOfModelFromDocument(*(info._itemModel), info.m_docView, *(info._progress));

    delete info._aop;
}

void DM_GuiManager::staticRemoveAllItemDrawableOfListFromView(ActionItemDrawable info)
{
    info._view->removeAllItemDrawableOfListFromDocuments(info._itemList, *(info._progress));

    delete info._aop;
}

void DM_GuiManager::staticClearResultMemoryAndRemoveStep(ActionStep info)
{
    if(info._step == NULL)
    {
        QList<CT_VirtualAbstractStep *> list = info._stepManager->getStepRootList();

        QListIterator<CT_VirtualAbstractStep *> it(list);

        while(it.hasNext())
        {
            info._totalProgressSize += staticRecursiveCountProgress(it.next());
        }

        int nProgress = 0;

        it.toFront();

        while(it.hasNext())
        {
            ActionStep infoRecur = info;
            infoRecur._step = it.next();
            infoRecur._nProgress = nProgress;

            nProgress = staticRecursiveClearResultMemoryAndRemoveStep(infoRecur);
        }
    }
    else
    {
        info._totalProgressSize = staticRecursiveCountProgress(info._step);
        info._nProgress = 0;

        staticRecursiveClearResultMemoryAndRemoveStep(info);
    }

    delete info._aop;
}

int DM_GuiManager::staticRecursiveCountProgress(CT_VirtualAbstractStep *step)
{
    int count = 0;

    QList<CT_VirtualAbstractStep *> list = step->getStepChildList();

    QListIterator<CT_VirtualAbstractStep *> it(list);

    while(it.hasNext())
    {
        count += staticRecursiveCountProgress(it.next());
    }

    count += step->nResult();

    return count;
}

int DM_GuiManager::staticRecursiveClearResultMemoryAndRemoveStep(ActionStep info)
{
    QList<CT_VirtualAbstractStep *> list = info._step->getStepChildList();

    QListIterator<CT_VirtualAbstractStep *> it(list);

    while(it.hasNext())
    {
        ActionStep infoRecur = info;
        infoRecur._step = it.next();

        info._nProgress = staticRecursiveClearResultMemoryAndRemoveStep(infoRecur);
    }

    QList<CT_ResultGroup *> listRes = info._step->getResults();

    QListIterator<CT_ResultGroup *> itRes(listRes);

    while(itRes.hasNext())
    {
        CT_ResultGroup *res = itRes.next();

        staticRemoveResultFromOtherView(ActionItemDrawable(NULL,
                                                           res,
                                                           NULL,
                                                           "",
                                                           info._docManagerView,
                                                           info._secondProgress));

        info._nProgress++;

        info._progress->setProgress(((float)(info._nProgress*100))/((float)info._totalProgressSize));
    }

    info._step->clearOutResult();

    info._stepManager->removeStep(info._step);

    return info._nProgress;
}

void DM_GuiManager::staticExport(ActionItemDrawable info)
{
    info._exporter->exportToFile();

    delete info._aop;

    info._progress->finish();
    delete info._exporter;
}

///////////////// PRIVATE SLOTS /////////////////

void DM_GuiManager::resultToBeClearedFromMemory(const CT_AbstractResult *res)
{
    staticRemoveResultFromOtherView(ActionItemDrawable(NULL,
                                                       (CT_AbstractResult*)res,
                                                       NULL,
                                                       "",
                                                       getDocumentManagerView(),
                                                       &_asyncSecondProgress));
}

void DM_GuiManager::resultToBeRemoved(const CT_AbstractResult *res)
{
    staticRemoveResultFromOtherView(ActionItemDrawable(NULL,
                                                       (CT_AbstractResult*)res,
                                                       NULL,
                                                       "",
                                                       getDocumentManagerView(),
                                                       &_asyncSecondProgress));
}

void DM_GuiManager::stepManagerCompletedLoadResultStep()
{
    if(getStepManager()->getAction() == CDM_StepManager::LoadSerializeResult)
    {
        disconnect(getStepManager(), SIGNAL(loadResultInProgress(int)), &_asyncProgress, SLOT(setProgress(int)));

        _asyncProgress.finish();
    }
}

void DM_GuiManager::slotCurrentAsyncOperationDestroyed()
{
    QMutexLocker locker(&m_asyncOperationTokenMutex);

    m_queueAsyncOperationDestroyed.enqueue(m_currentAsyncOperation);
    m_currentAsyncOperation = NULL;

    if(!m_tokens.isEmpty())
        delete m_tokens.takeFirst();

    locker.unlock();

    emit currentAsyncOperationReleased();
}

void DM_GuiManager::slotCurrentAsyncOperationReleased()
{
    DM_AsyncOperation *obj = m_queueAsyncOperationDestroyed.dequeue();

    QMutexLocker locker(&m_asyncOperationTokenMutex);

    if((_progressDialog->data() != NULL)
            && (_progressDialog->data() == obj))
    {
        _progressDialog->setData(NULL);
        _progressDialog->setCanClose(true);
        _progressDialog->close();
    }
}
