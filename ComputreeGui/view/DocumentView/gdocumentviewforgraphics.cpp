#include "gdocumentviewforgraphics.h"

#include "dm_guimanager.h"

#include "view/DocumentView/GraphicsViews/PointsAttributes/gpointsattributesmanager.h"
#include "view/DocumentView/GraphicsViews/3D/g3dfakepainter.h"
#include "view/DocumentView/GraphicsViews/3D/Octree/octreebuilder.h"
#include "view/DocumentView/GraphicsViews/3D/g3dgraphicsview.h"

#include "cdm_tools.h"

#include "ct_global/ct_context.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "ct_cloudindex/abstract/ct_abstractmodifiablecloudindex.h"

#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_itemdrawable/tools/iterator/ct_itemiterator.h"

#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include "dm_iteminfoforgraphics.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>


DM_VertexVBOManager* GDocumentViewForGraphics::VERTEX_VBO_MANAGER = NULL;
int GDocumentViewForGraphics::N_DOCUMENT_VIEW_FOR_GRAPHICS = 0;

GDocumentViewForGraphics::GDocumentViewForGraphics(GDocumentManagerView &manager, QString title, QString type) : GDocumentView(manager, title)
{
    m_mutex = new QMutex(QMutex::Recursive);
    _graphicsLocked = false;
    _type = type;
    _pofManager.loadDefault();
    m_useColorCloud = true;
    m_useNormalCloud = true;
    _pixelSize = PX_1;
    _drawMode = DM_GraphicsViewOptions::Normal;
    m_colorVboManager = new DM_ColorVBOManager();

    ++N_DOCUMENT_VIEW_FOR_GRAPHICS;

    if(VERTEX_VBO_MANAGER == NULL)
        VERTEX_VBO_MANAGER = new DM_VertexVBOManager();
}

GDocumentViewForGraphics::~GDocumentViewForGraphics()
{
    delete _graphicsOptionsView;
    delete m_mutex;
}

void GDocumentViewForGraphics::init()
{
    GDocumentView::init();

    // le widget avec la config des caméras et des graphiques....
    createAndAddCameraAndGraphicsOptions(getCentralWidget());

    // ...et le widget contenant les graphiques
    createAndAddGraphicsWidgetContainer(getCentralWidget());
}

void GDocumentViewForGraphics::addGraphics(GGraphicsView *graphics)
{
    graphics->setDocumentView(this);
    graphics->setColorVBOManager(m_colorVboManager);
    graphics->setAttributesManager(&m_attributesManager);

    _listGraphics.append(graphics);
    _layoutGraphics->addWidget(graphics->getViewWidget());

    _cameraOptionsView->setCamera(graphics->getCamera());
}

const QList<GGraphicsView*>& GDocumentViewForGraphics::getGraphicsList() const
{
    return _listGraphics;
}

void GDocumentViewForGraphics::beginAddMultipleItemDrawable()
{
    lockGraphics();
}

void GDocumentViewForGraphics::addItemDrawable(CT_AbstractItemDrawable &item)
{
    bool locked = _graphicsLocked;

    if(!locked)
        lockGraphics();

        GDocumentView::addItemDrawable(item);

    if(!locked)
        unlockGraphics();
}

void GDocumentViewForGraphics::endAddMultipleItemDrawable()
{
    unlockGraphics();
}

void GDocumentViewForGraphics::beginRemoveMultipleItemDrawable()
{
    lockGraphics();
}

void GDocumentViewForGraphics::removeItemDrawable(CT_AbstractItemDrawable &item)
{
    bool locked = _graphicsLocked;

    if(!locked)
        lockGraphics();

    GDocumentView::removeItemDrawable(item);

    if(!locked)
        unlockGraphics();
}

void GDocumentViewForGraphics::endRemoveMultipleItemDrawable()
{
    unlockGraphics();
}

void GDocumentViewForGraphics::removeAllItemDrawableOfResult(CT_AbstractResult &res)
{
    lockGraphics();

    GDocumentView::removeAllItemDrawableOfResult(res);

    unlockGraphics();
}

void GDocumentViewForGraphics::removeAllItemDrawableOfModel(CT_OutAbstractModel &model)
{
    lockGraphics();

    GDocumentView::removeAllItemDrawableOfModel(model);

    unlockGraphics();
}

void GDocumentViewForGraphics::removeAllSelectedItemDrawable()
{
    lockGraphics();

    GDocumentView::removeAllSelectedItemDrawable();

    unlockGraphics();
}

void GDocumentViewForGraphics::removeAllItemDrawable()
{
    lockGraphics();

    GDocumentView::removeAllItemDrawable();

    unlockGraphics();
}

QList<InDocumentViewInterface *> GDocumentViewForGraphics::views() const
{
    QList<InDocumentViewInterface *> l;

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
        l.append(it.next());

    return l;
}

void GDocumentViewForGraphics::redrawGraphics()
{
    m_mutex->lock();

    if(!_graphicsLocked)
    {
        QListIterator<GGraphicsView*> it(_listGraphics);

        while(it.hasNext())
        {
            it.next()->redraw();
        }
    }

    m_mutex->unlock();
}

void GDocumentViewForGraphics::lock()
{
    lockGraphics();
}

void GDocumentViewForGraphics::unlock()
{
    unlockGraphics();
}

bool GDocumentViewForGraphics::acceptAction(const CT_AbstractAction *action) const
{
    return (action == NULL) || (dynamic_cast<const CT_AbstractActionForGraphicsView*>(action) != NULL);
}

bool GDocumentViewForGraphics::setCurrentAction(CT_AbstractAction *action, bool deleteAction)
{
    if(!acceptAction(action))
    {
        if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
            delete action;

        return false;
    }

    emit currentActionChanged(action);

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        if(action == NULL)
            it.next()->setCurrentAction(NULL);
        else
            it.next()->setCurrentAction((CT_AbstractActionForGraphicsView*)action->copy());
    }

    redrawGraphics();

    if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
        delete action;

    return true;
}

bool GDocumentViewForGraphics::setDefaultAction(CT_AbstractAction *action, bool deleteAction)
{
    if(!acceptAction(action))
    {
        if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
            delete action;

        return false;
    }

    emit defaultActionChanged(action);

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        if(action == NULL)
            it.next()->setDefaultAction(NULL);
        else
            it.next()->setDefaultAction((CT_AbstractActionForGraphicsView*)action->copy());
    }

    redrawGraphics();

    if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
        delete action;

    return true;
}

CT_AbstractAction* GDocumentViewForGraphics::currentAction() const
{
    if(_listGraphics.isEmpty())
        return NULL;

    return _listGraphics.first()->actionsHandler()->currentAction();
}

CT_AbstractAction *GDocumentViewForGraphics::defaultAction() const
{
    if(_listGraphics.isEmpty())
        return NULL;

    return _listGraphics.first()->actionsHandler()->defaultAction();
}

void GDocumentViewForGraphics::removeActions(const QString &uniqueName) const
{
    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
        it.next()->actionsHandler()->removeActions(uniqueName);
}

QString GDocumentViewForGraphics::getType() const
{
    return _type;
}

bool GDocumentViewForGraphics::useItemColor() const
{
    return true;
}

void GDocumentViewForGraphics::setColor(const CT_AbstractItemDrawable *item, const QColor &color)
{
    const QHash<CT_AbstractResult*, QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*>* > &ii = getItemsInformations();
    QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*> *hash = ii.value(item->result(), NULL);

    if(hash == NULL)
        hash = createItemInformationsForResult(item->result());

    DM_ItemInfoForGraphics *info = static_cast<DM_ItemInfoForGraphics*>(hash->value((CT_AbstractItemDrawable*)item, NULL));

    if(info == NULL)
    {
        info = static_cast<DM_ItemInfoForGraphics*>(createNewItemInformation(item));
        hash->insert((CT_AbstractItemDrawable*)item, info);
    }

    info->setColor(color);

    if(!_listGraphics.isEmpty())
    {
        createColorCloudRegistered<CT_AbstractPointsAttributes>();
        createColorCloudRegistered<CT_AbstractFaceAttributes>();
        createColorCloudRegistered<CT_AbstractEdgeAttributes>();

        G3DFakePainter painter;
        painter.setGraphicsView(dynamic_cast<G3DGraphicsView*>(_listGraphics.first()));
        painter.setDrawMode(G3DFakePainter::ApplyColorPoints | G3DFakePainter::ApplyColorEdges | G3DFakePainter::ApplyColorFaces);
        painter.setApplyColor(color);
        painter.setPointsColorCloud(colorCloudRegistered<CT_AbstractPointsAttributes>());
        painter.setEdgesColorCloud(colorCloudRegistered<CT_AbstractEdgeAttributes>());
        painter.setFacesColorCloud(colorCloudRegistered<CT_AbstractFaceAttributes>());

        ((CT_AbstractItemDrawable*)item)->draw(*_listGraphics.first(), painter);
    }

    CT_AbstractItemGroup *group = dynamic_cast<CT_AbstractItemGroup*>((CT_AbstractItemDrawable*)item);

    if(group != NULL)
        recursiveSetColor(group, color);

    m_colorVboManager->refresh();
}

QColor GDocumentViewForGraphics::getColor(const CT_AbstractItemDrawable *item)
{
    QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*> *hash = getItemsInformations().value(item->result(), NULL);

    if(hash != NULL)
    {
        DM_ItemInfoForGraphics *info = static_cast<DM_ItemInfoForGraphics*>(hash->value((CT_AbstractItemDrawable*)item, NULL));

        if(info != NULL)
            return info->color();
    }

    return QColor();
}

bool GDocumentViewForGraphics::useOctreeOfPoints() const
{
    return true;
}

OctreeInterface* GDocumentViewForGraphics::octreeOfPoints() const
{
    return const_cast<OctreeController*>(&m_octreeController);
}

template<>
void GDocumentViewForGraphics::createColorCloudRegistered<CT_AbstractPointsAttributes>()
{
    if(colorCloudRegistered<CT_AbstractPointsAttributes>().data() == NULL)
        setColorCloudRegistered<CT_AbstractPointsAttributes>(PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithPointCloud, false));
}

template<>
void GDocumentViewForGraphics::createColorCloudRegistered<CT_AbstractFaceAttributes>()
{
    if(colorCloudRegistered<CT_AbstractFaceAttributes>().data() == NULL)
        setColorCloudRegistered<CT_AbstractFaceAttributes>(PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithFaceCloud, false));
}

template<>
void GDocumentViewForGraphics::createColorCloudRegistered<CT_AbstractEdgeAttributes>()
{
    if(colorCloudRegistered<CT_AbstractEdgeAttributes>().data() == NULL)
        setColorCloudRegistered<CT_AbstractEdgeAttributes>(PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithEdgeCloud, false));
}

template<>
void GDocumentViewForGraphics::setColorCloudRegistered<CT_AbstractPointsAttributes>(QSharedPointer<CT_StandardColorCloudRegistered> cc)
{
    lock();

    m_pColorCloudRegistered = cc;
    m_colorVboManager->setCurrentColorCloud(cc);

    unlock();
}

template<>
void GDocumentViewForGraphics::setColorCloudRegistered<CT_AbstractFaceAttributes>(QSharedPointer<CT_StandardColorCloudRegistered> cc)
{
    lock();

    m_fColorCloudRegistered = cc;

    unlock();
}

template<>
void GDocumentViewForGraphics::setColorCloudRegistered<CT_AbstractEdgeAttributes>(QSharedPointer<CT_StandardColorCloudRegistered> cc)
{
    lock();

    m_eColorCloudRegistered = cc;

    unlock();
}

template<>
QSharedPointer<CT_StandardColorCloudRegistered> GDocumentViewForGraphics::colorCloudRegistered<CT_AbstractPointsAttributes>() const
{
    return m_pColorCloudRegistered;
}

template<>
QSharedPointer<CT_StandardColorCloudRegistered> GDocumentViewForGraphics::colorCloudRegistered<CT_AbstractFaceAttributes>() const
{
    return m_fColorCloudRegistered;
}

template<>
QSharedPointer<CT_StandardColorCloudRegistered> GDocumentViewForGraphics::colorCloudRegistered<CT_AbstractEdgeAttributes>() const
{
    return m_eColorCloudRegistered;
}

void GDocumentViewForGraphics::setUseColorCloud(bool use)
{
    m_useColorCloud = use;
}

bool GDocumentViewForGraphics::useColorCloud() const
{
    return m_useColorCloud;
}

template<>
void GDocumentViewForGraphics::setNormalCloudRegistered<CT_AbstractPointsAttributes>(QSharedPointer<CT_StandardNormalCloudRegistered> nn)
{
    lock();

    m_pNormalCloudRegistered = nn;

    unlock();
}

template<>
void GDocumentViewForGraphics::setNormalCloudRegistered<CT_AbstractFaceAttributes>(QSharedPointer<CT_StandardNormalCloudRegistered> nn)
{
    lock();

    m_fNormalCloudRegistered = nn;

    unlock();
}

template<>
QSharedPointer<CT_StandardNormalCloudRegistered> GDocumentViewForGraphics::normalCloudRegistered<CT_AbstractPointsAttributes>() const
{
    return m_pNormalCloudRegistered;
}

template<>
QSharedPointer<CT_StandardNormalCloudRegistered> GDocumentViewForGraphics::normalCloudRegistered<CT_AbstractFaceAttributes>() const
{
    return m_fNormalCloudRegistered;
}

void GDocumentViewForGraphics::setUseNormalCloud(bool use)
{
    m_useNormalCloud = use;
}

bool GDocumentViewForGraphics::useNormalCloud() const
{
    return m_useNormalCloud;
}

void GDocumentViewForGraphics::applyAttributes(DM_AbstractAttributes *dpa)
{
    QProgressDialog dialog(tr("Veuillez patienter pendant le traitement..."), "", 0, 100);
    dialog.setWindowModality(Qt::ApplicationModal);
    dialog.setCancelButton(NULL);
    dialog.show();

    dpa->setDocument(this);
    dpa->checkAndSetNecessaryCloudToDoc();

    QEventLoop event;

    QThread *thread = dpa->thread();

    DM_AbstractWorker::staticConnectWorkerToThread(dpa, false, false, true);

    connect(dpa, SIGNAL(finished()), &event, SLOT(quit()), Qt::DirectConnection);
    connect(thread, SIGNAL(finished()), &event, SLOT(quit()), Qt::DirectConnection);
    connect(dpa, SIGNAL(progressChanged(int)), &dialog, SLOT(setValue(int)), Qt::QueuedConnection);

    thread->start();
    event.exec();

    dialog.close();

    disconnect(thread, NULL, dpa, NULL);
    disconnect(dpa, NULL, thread, NULL);

    m_colorVboManager->refresh();
}

void GDocumentViewForGraphics::showOptions()
{
    if(_listGraphics.size() > 0)
    {
        DM_GraphicsViewOptions &opt = (DM_GraphicsViewOptions&)((const GGraphicsView*)_listGraphics.at(0))->getOptions();
        opt.setOctreeNumberOfCells(m_octreeController.numberOfCells());

        _graphicsOptionsView->setOptions(opt);

        const QRect screen = QApplication::desktop()->screenGeometry();
        _graphicsOptionsView->move(screen.center() - _graphicsOptionsView->rect().center());

        if(_graphicsOptionsView->exec())
        {
            validateOptions();
        }
    }
}

void GDocumentViewForGraphics::validateOptions()
{
    const DM_GraphicsViewOptions &options = _graphicsOptionsView->getOptions();

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
        it.next()->setOptions(options);

    m_octreeController.setNumberOfCells(options.octreeNumberOfCells());
}


void GDocumentViewForGraphics::addActualPointOfView()
{
    QList<DM_PointOfView> listPof = _pofManager.getPointOfViewList(getKeyForPointOfViewManager());

    // on limite  20 le nombre de point de vue
    if(listPof.size() == 20)
        _pofManager.removePointOfView(getKeyForPointOfViewManager(), listPof.takeLast());

    QString name = QInputDialog::getText(getSubWindow(), tr("Point de vue"), tr("Veuillez entrer un nom pour le point de vue :"), QLineEdit::Normal, QString("P%1").arg(_pofManager.numberPointOfViewAddedFromBeginning(getKeyForPointOfViewManager())+1));

    if(!name.isEmpty())
    {
        DM_GraphicsViewCamera *cam = _cameraOptionsView->getCamera();

        double q0, q1, q2, q3;

        cam->getOrientation(q0, q1, q2 ,q3);

        _pofManager.addPointOfView(getKeyForPointOfViewManager(),
                                              DM_PointOfView(name,
                                                             cam->x(),
                                                             cam->y(),
                                                             cam->z(),
                                                             cam->rx(),
                                                             cam->ry(),
                                                             cam->rz(),
                                                             q0,
                                                             q1,
                                                             q2,
                                                             q3));
    }
}

void GDocumentViewForGraphics::setPointOfView(DM_PointOfView *pof)
{
    if(pof != NULL)
    {
        DM_GraphicsViewCamera *cam = _cameraOptionsView->getCamera();

        cam->setPosition(pof->x(), pof->y(), pof->z());
        cam->setOrientation(pof->q0(), pof->q1(), pof->q2(), pof->q3());
    }
}

void GDocumentViewForGraphics::showAttributesOptions()
{
    GPointsAttributesManager dialog;
    dialog.setManager(&m_attributesManager);
    dialog.setDocument(this);

    const QRect screen = QApplication::desktop()->screenGeometry();
    dialog.move(screen.center() - dialog.rect().center());

    dialog.exec();
}

void GDocumentViewForGraphics::constructOctreeOfPoints()
{
    if(m_octreeController.mustBeReconstructed())
    {
        DM_AsyncOperation *aop = GUI_MANAGER->requestExclusiveAsyncOperation();

        if(aop != NULL)
        {
            // create a thread for the builder
            QThread *thread = new QThread();

            // create a builder
            OctreeBuilder *builder = new OctreeBuilder();
            builder->moveToThread(thread);
            builder->addData(aop, true);
            builder->setDocument(this);
            builder->setOctreeController(&m_octreeController);

            aop->progressDialog()->setCanClose(false);
            aop->progressDialog()->setLabelText(QObject::tr("Veuillez patienter pendant la construction de l'octree"));
            aop->progressDialog()->setSecondLabelText("");
            aop->progressDialog()->setValue(0);
            aop->progressDialog()->show();

            connect(builder, SIGNAL(progressChanged(int)), aop, SLOT(setProgress(int)), Qt::QueuedConnection);
            connect(aop, SIGNAL(cancel()), builder, SLOT(cancel()), Qt::QueuedConnection);

            DM_AbstractWorker::staticConnectWorkerToThread(builder, true, true, true);

            QEventLoop event;

            connect(builder, SIGNAL(finished()), &event, SLOT(quit()), Qt::DirectConnection);
            connect(thread, SIGNAL(finished()), &event, SLOT(quit()), Qt::DirectConnection);

            thread->start();
            event.exec();
        }
    }
}

void GDocumentViewForGraphics::changePixelSize()
{
    DM_GraphicsViewOptions& options = (DM_GraphicsViewOptions&) _graphicsOptionsView->getOptions();

    if (_pixelSize == PX_1)
    {
        _pixelSize = PX_2;
        _buttonPixelSize->setIcon(QIcon(":/Icones/Icones/px_2.png"));
        options.setPointSize(2);
    } else if (_pixelSize == PX_2)
    {
        _pixelSize = PX_3;
        _buttonPixelSize->setIcon(QIcon(":/Icones/Icones/px_3.png"));
        options.setPointSize(3);
    } else if (_pixelSize == PX_3)
    {
        _pixelSize = PX_1;
        _buttonPixelSize->setIcon(QIcon(":/Icones/Icones/px_1.png"));
        options.setPointSize(1);
    }
    validateOptions();
}

void GDocumentViewForGraphics::changePixelSize(double size)
{
    if (size == 1)
    {
        _pixelSize = PX_1;
        _buttonPixelSize->setIcon(QIcon(":/Icones/Icones/px_1.png"));
    } else if (size == 2)
    {
        _pixelSize = PX_2;
        _buttonPixelSize->setIcon(QIcon(":/Icones/Icones/px_2.png"));
    } else if (size >= 3)
    {
        _pixelSize = PX_3;
        _buttonPixelSize->setIcon(QIcon(":/Icones/Icones/px_3.png"));
    }
}

void GDocumentViewForGraphics::changeDrawMode()
{
    DM_GraphicsViewOptions& options = (DM_GraphicsViewOptions&) _graphicsOptionsView->getOptions();

    if (_drawMode == DM_GraphicsViewOptions::Normal)
    {
        _drawMode = DM_GraphicsViewOptions::Always;
        _buttonDrawMode->setIcon(QIcon(":/Icones/Icones/fast_always.png"));

    } else if (_drawMode == DM_GraphicsViewOptions::Always)
    {
        _drawMode = DM_GraphicsViewOptions::Never;
        _buttonDrawMode->setIcon(QIcon(":/Icones/Icones/fast_never.png"));

    } else if (_drawMode == DM_GraphicsViewOptions::Never)
    {
        _drawMode = DM_GraphicsViewOptions::Normal;
        _buttonDrawMode->setIcon(QIcon(":/Icones/Icones/fast_onmove.png"));
    }

    options.drawFastest(_drawMode);
    validateOptions();
}

void GDocumentViewForGraphics::changeDrawMode(DM_GraphicsViewOptions::DrawFastestMode mode)
{
    _drawMode = mode;
    if (_drawMode == DM_GraphicsViewOptions::Normal)
    {
        _drawMode = DM_GraphicsViewOptions::Normal;
        _buttonDrawMode->setIcon(QIcon(":/Icones/Icones/fast_onmove.png"));

    } else if (mode == DM_GraphicsViewOptions::Always)
    {
        _drawMode = DM_GraphicsViewOptions::Always;
        _buttonDrawMode->setIcon(QIcon(":/Icones/Icones/fast_always.png"));

    } else if (mode == DM_GraphicsViewOptions::Never)
    {
        _drawMode = DM_GraphicsViewOptions::Never;
        _buttonDrawMode->setIcon(QIcon(":/Icones/Icones/fast_never.png"));
    }
}

void GDocumentViewForGraphics::setTransparencyActivated(bool activated)
{
    DM_GraphicsViewOptions& options = (DM_GraphicsViewOptions&) _graphicsOptionsView->getOptions();

    options.useTransparency(activated);
    validateOptions();
}

void GDocumentViewForGraphics::slotItemDrawableAdded(CT_AbstractItemDrawable &item)
{
    GDocumentView::slotItemDrawableAdded(item);

    if(!getGraphicsList().isEmpty())
    {
        G3DFakePainter fakePainter;
        fakePainter.setGraphicsView(dynamic_cast<G3DGraphicsView*>(getGraphicsList().first()));
        fakePainter.setDrawMode(G3DFakePainter::BackupPointCloudIndex);
        item.draw(*getGraphicsList().first(), fakePainter);
        QListIterator<CT_AbstractCloudIndex*> it(fakePainter.pointCloudIndexBackup());

        while(it.hasNext())
            m_octreeController.addPoints(dynamic_cast<CT_AbstractPointCloudIndex*>(it.next()));
    }
}

void GDocumentViewForGraphics::slotItemToBeRemoved(CT_AbstractItemDrawable &item)
{
    GDocumentView::slotItemToBeRemoved(item);

    if(!getGraphicsList().isEmpty())
    {
        G3DFakePainter fakePainter;
        fakePainter.setGraphicsView(dynamic_cast<G3DGraphicsView*>(getGraphicsList().first()));
        fakePainter.setDrawMode(G3DFakePainter::BackupPointCloudIndex);
        item.draw(*getGraphicsList().first(), fakePainter);
        QListIterator<CT_AbstractCloudIndex*> it(fakePainter.pointCloudIndexBackup());

        while(it.hasNext())
            m_octreeController.removePoints(dynamic_cast<CT_AbstractPointCloudIndex*>(it.next()));
    }
}


void GDocumentViewForGraphics::syncChanged(bool enable)
{
    if(enable)
        emit syncEnabled(this);
    else
        emit syncDisabled(this);
}

void GDocumentViewForGraphics::pluginExporterManagerReloaded()
{
    CDM_Tools tools(GUI_MANAGER->getPluginManager());

    _buttonExport->setMenu(NULL);
    _buttonExport->setEnabled(false);

    QMenu *exportersMenu = tools.createMenuForAllExporters(this, SLOT(exporterActionTriggered()));

    if(exportersMenu != NULL)
    {
        _buttonExport->setMenu(exportersMenu);
        _buttonExport->setEnabled(true);
    }
}

void GDocumentViewForGraphics::exporterActionTriggered()
{
    CDM_Tools tools(GUI_MANAGER->getPluginManager());

    CT_AbstractExporter *exporter = dynamic_cast<CT_AbstractExporter*>(sender()->parent());

    CT_AbstractExporter *exCopy = exporter->copy();
    exCopy->init();

    if(exCopy->canExportItems())
        exCopy->setItemDrawableToExport(getSelectedItemDrawable());

    if(exCopy->canExportPoints())
    {
        QList<CT_AbstractCloudIndex*> points;

        QListIterator<GGraphicsView*> it(_listGraphics);

        while(it.hasNext())
        {
            CT_SPCIR selec = it.next()->getSelectedPoints();

            if(!selec.isNull()
                    && (selec->abstractCloudIndex() != NULL))
                points.append(selec->abstractCloudIndex());
        }

        exCopy->setPointsToExport(points);
    }

    if(exCopy->canExportFaces())
    {
        QList<CT_AbstractCloudIndex*> faces;

        QListIterator<GGraphicsView*> it(_listGraphics);

        while(it.hasNext())
        {
            CT_SFCIR selec = it.next()->getSelectedFaces();

            if(!selec.isNull()
                    && (selec->abstractCloudIndex() != NULL))
                faces.append(selec->abstractCloudIndex());
        }

        exCopy->setFacesToExport(faces);
    }

    if(exCopy->canExportEdges())
    {
        QList<CT_AbstractCloudIndex*> edges;

        QListIterator<GGraphicsView*> it(_listGraphics);

        while(it.hasNext())
        {
            CT_SECIR selec = it.next()->getSelectedEdges();

            if(!selec.isNull()
                    && (selec->abstractCloudIndex() != NULL))
                edges.append(selec->abstractCloudIndex());
        }

        exCopy->setEdgesToExport(edges);
    }

    if(exCopy->hasSomethingToExport())
    {
        QString filepath = QFileDialog::getSaveFileName(NULL, tr("Exporter sous..."), exporter->exportFilePath(), tools.createFileExtensionForExporter(exCopy));

        if(!filepath.isEmpty()
                && exCopy->setExportFilePath(filepath))
        {
            exporter->setExportFilePath(filepath);
            exCopy->setDocumentManager(getManager());
            exCopy->setMyDocument(this);

            GUI_MANAGER->asyncExport(exCopy, NULL);
            return;
        }
    }
    else if(!exCopy->errorMessage().isEmpty())
    {
        QMessageBox::critical(NULL, tr("Erreur"), exCopy->errorMessage(), QMessageBox::Ok);
    }

    delete exCopy;
}

void GDocumentViewForGraphics::closeEvent(QCloseEvent *closeEvent)
{
    if(canClose())
    {
        --N_DOCUMENT_VIEW_FOR_GRAPHICS;

        if(N_DOCUMENT_VIEW_FOR_GRAPHICS == 0)
        {
            delete VERTEX_VBO_MANAGER;
            VERTEX_VBO_MANAGER = NULL;
        }

        delete m_colorVboManager;
        m_colorVboManager = NULL;

        m_mutex->lock();
        qDeleteAll(_listGraphics.begin(), _listGraphics.end());
        _listGraphics.clear();
        m_mutex->unlock();
    }

    GDocumentView::closeEvent(closeEvent);
}

///////////////// PRIVATE //////////////

void GDocumentViewForGraphics::createAndAddCameraAndGraphicsOptions(QWidget *parent)
{
    QWidget *widgetContainer = new QWidget(parent);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 3, 0, 0);

    delete widgetContainer->layout();
    widgetContainer->setLayout(layout);

    // widget avec les X, Y, Z, etc...
    _cameraOptionsView = new GCameraGraphicsOptions(widgetContainer);

    // bouton qui permet d'ouvrir/enregistrer un point de vue
    _pointOfViewButton= new QToolButton(widgetContainer);
    _pointOfViewButton->setMaximumWidth(38);
    _pointOfViewButton->setMinimumWidth(38);
    _pointOfViewButton->setToolTip(tr("Point de vue"));
    _pointOfViewButton->setIcon(QIcon(":/Icones/Icones/eye.png"));
    _pointOfViewButton->setPopupMode(QToolButton::InstantPopup);

    // son menu
    _pointOfViewMenu = new GPointOfViewDocumentManager(_pofManager, getKeyForPointOfViewManager(), _pointOfViewButton);
    _pointOfViewButton->setMenu(_pointOfViewMenu);

    // bouton qui permet d'exporter les CT_AbstractItemDrawable selectionnes
    _buttonExport = new QPushButton(widgetContainer);
    _buttonExport->setMaximumWidth(33);
    _buttonExport->setMinimumWidth(33);
    _buttonExport->setToolTip(tr("Exporter les éléments sélectionnés"));
    _buttonExport->setIcon(QIcon(":/Icones/Icones/upload.png"));
    _buttonExport->setEnabled(false);

    pluginExporterManagerReloaded();

    QPushButton *buttonPointsAttributes = new QPushButton(widgetContainer);
    buttonPointsAttributes->setMaximumWidth(33);
    buttonPointsAttributes->setMinimumWidth(33);
    buttonPointsAttributes->setToolTip(tr("Configurer les couleurs des points"));
    buttonPointsAttributes->setIcon(QIcon(":/Icones/Icones/gradient.png"));

    _buttonPixelSize = new QPushButton(widgetContainer);
    _buttonPixelSize->setMaximumWidth(33);
    _buttonPixelSize->setMinimumWidth(33);
    _buttonPixelSize->setToolTip(tr("Changer la taille des pixels"));
    _buttonPixelSize->setIcon(QIcon(":/Icones/Icones/px_1.png"));
    _buttonPixelSize->setEnabled(true);

    _buttonDrawMode = new QPushButton(widgetContainer);
    _buttonDrawMode->setMaximumWidth(33);
    _buttonDrawMode->setMinimumWidth(33);
    _buttonDrawMode->setToolTip(tr("Changer le mode de dessin :\n- Simplifié lors des déplacements\n- Toujours Simplifié- Jamais Simplifié"));
    _buttonDrawMode->setIcon(QIcon(":/Icones/Icones/fast_onmove.png"));
    _buttonDrawMode->setEnabled(true);

    _buttonConstructOctree = new QPushButton(widgetContainer);
    _buttonConstructOctree->setMaximumWidth(33);
    _buttonConstructOctree->setMinimumWidth(33);
    _buttonConstructOctree->setToolTip(tr("(Re)construire un octree"));
    _buttonConstructOctree->setIcon(QIcon(":/Icones/Icones/octree.png"));
    //_buttonConstructOctree->setText("OCTREE");
    _buttonConstructOctree->setEnabled(false);

    connect(GUI_MANAGER->getPluginManager(), SIGNAL(finishLoading()), this, SLOT(pluginExporterManagerReloaded()));

    // fenetre de configuration des options
    _graphicsOptionsView = new GGraphicsViewOptions();

    // bouton qui appelle la fentre de configuration des options
    QPushButton *buttonShowOptions = new QPushButton(widgetContainer);
    buttonShowOptions->setMaximumWidth(33);
    buttonShowOptions->setMinimumWidth(33);
    buttonShowOptions->setToolTip(tr("Configurer l'affichage"));
    buttonShowOptions->setIcon(QIcon(":/Icones/Icones/preferences-system.png"));

    layout->addWidget(_pointOfViewButton);
    layout->addWidget(_buttonExport);
    layout->addWidget(buttonShowOptions);
    layout->addWidget(buttonPointsAttributes);
    layout->addWidget(_buttonDrawMode);
    layout->addWidget(_buttonPixelSize);
    layout->addWidget(_buttonConstructOctree);
    layout->addWidget(_cameraOptionsView);

    ((QVBoxLayout*)parent->layout())->insertWidget(0, widgetContainer);

    connect(buttonShowOptions, SIGNAL(clicked()), this, SLOT(showOptions()));
    connect(buttonPointsAttributes, SIGNAL(clicked()), this, SLOT(showAttributesOptions()));
    connect(_buttonPixelSize, SIGNAL(clicked()), this, SLOT(changePixelSize()));
    connect(_buttonDrawMode, SIGNAL(clicked()), this, SLOT(changeDrawMode()));
    connect(_pointOfViewMenu, SIGNAL(addActualPointOfView()), this, SLOT(addActualPointOfView()));
    connect(_pointOfViewMenu, SIGNAL(setPointOfView(DM_PointOfView*)), this, SLOT(setPointOfView(DM_PointOfView*)), Qt::DirectConnection);

    connect(_cameraOptionsView, SIGNAL(syncGraphics(bool)), this, SLOT(syncChanged(bool)));

    connect(&m_octreeController, SIGNAL(octreeMustBeReconstructed(bool)), _buttonConstructOctree, SLOT(setEnabled(bool)));
    connect(_buttonConstructOctree, SIGNAL(clicked()), this, SLOT(constructOctreeOfPoints()));

    connect(_graphicsOptionsView, SIGNAL(pointSizeChanged(double)), this, SLOT(changePixelSize(double)));
    connect(_graphicsOptionsView, SIGNAL(drawModeChanged(DM_GraphicsViewOptions::DrawFastestMode)), this, SLOT(changeDrawMode(DM_GraphicsViewOptions::DrawFastestMode)));

}

void GDocumentViewForGraphics::createAndAddGraphicsWidgetContainer(QWidget *parent)
{
    QWidget *graphicsWidgetContainer = new QWidget(parent);

    _layoutGraphics = new QHBoxLayout();
    _layoutGraphics->setContentsMargins(0, 0, 0, 0);
    _layoutGraphics->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));

    delete graphicsWidgetContainer->layout();
    graphicsWidgetContainer->setLayout(_layoutGraphics);

    parent->layout()->addWidget(graphicsWidgetContainer);
}

QString GDocumentViewForGraphics::getKeyForPointOfViewManager()
{
    return _type;
}

void GDocumentViewForGraphics::lockGraphics()
{
    m_mutex->lock();

    _graphicsLocked = true;

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        it.next()->lockPaint();
    }

    m_mutex->unlock();
}

void GDocumentViewForGraphics::unlockGraphics()
{
    m_mutex->lock();

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        it.next()->unlockPaint();
    }

    _graphicsLocked = false;

    m_mutex->unlock();
}

DM_AbstractInfo* GDocumentViewForGraphics::createNewItemInformation(const CT_AbstractItemDrawable *item) const
{
    Q_UNUSED(item)

    return new DM_ItemInfoForGraphics();
}

void GDocumentViewForGraphics::recursiveSetColor(CT_AbstractItemGroup *group,
                                                 const QColor &color)
{
    CT_AbstractResult *lastResult = NULL;
    QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*> *hash = NULL;

    CT_GroupIterator itG(group);

    while(itG.hasNext())
    {
        CT_AbstractItemGroup *child = (CT_AbstractItemGroup*)itG.next();

        if(lastResult != child->result())
        {
            lastResult = child->result();

            hash = getItemsInformations().value(lastResult);

            if(hash == NULL)
                hash = createItemInformationsForResult(lastResult);
        }

        DM_ItemInfoForGraphics *childInfo = static_cast<DM_ItemInfoForGraphics*>(hash->value(child, NULL));

        if(childInfo == NULL)
        {
            childInfo = static_cast<DM_ItemInfoForGraphics*>(createNewItemInformation(child));
            hash->insert(child, childInfo);
        }

        childInfo->setColor(color);

        recursiveSetColor(child, color);
    }

    CT_ItemIterator itI(group);

    while(itI.hasNext())
    {
        CT_AbstractSingularItemDrawable *child = (CT_AbstractSingularItemDrawable*)itI.next();

        if(lastResult != child->result())
        {
            lastResult = child->result();

            hash = getItemsInformations().value(lastResult);

            if(hash == NULL)
                hash = createItemInformationsForResult(lastResult);

        }

        DM_ItemInfoForGraphics *childInfo = static_cast<DM_ItemInfoForGraphics*>(hash->value(child, NULL));

        if(childInfo == NULL)
        {
            childInfo = static_cast<DM_ItemInfoForGraphics*>(createNewItemInformation(child));
            hash->insert(child, childInfo);
        }

        childInfo->setColor(color);
    }
}
