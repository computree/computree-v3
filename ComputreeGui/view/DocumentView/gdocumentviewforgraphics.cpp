#include "gdocumentviewforgraphics.h"

#include "dm_guimanager.h"

#include "view/DocumentView/GraphicsViews/PointsAttributes/gpointsattributesmanager.h"

#include "cdm_tools.h"

#include "ct_global/ct_context.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "ct_cloudindex/abstract/ct_abstractmodifiablecloudindex.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"

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
#include <QProgressDialog>

GDocumentViewForGraphics::GDocumentViewForGraphics(GDocumentManagerView &manager, QString title, QString type) : GDocumentView(manager, title)
{
    m_mutex = new QMutex(QMutex::Recursive);
    _graphicsLocked = false;
    _type = type;
    _pofManager.loadDefault();
    m_useNormalCloud = true;
    _pixelSize = PX_1;
    _drawMode = DM_GraphicsViewOptions::Normal;

    m_pointsColorCloudRegistered = CT_CCR(NULL);
    m_pointsAttribCloudRegistered = QSharedPointer< AttribCloudRegisteredType >(NULL);

    m_timerUpdateColors.setSingleShot(true);
    m_timerUpdateColors.setInterval(50);

    m_timerDirtyColorsOfPoints.setSingleShot(true);
    m_timerDirtyColorsOfPoints.setInterval(50);

    connect(&m_timerUpdateColors, SIGNAL(timeout()), this, SLOT(mustUpdateItemDrawablesThatColorWasModified()), Qt::QueuedConnection);
    connect(&m_timerDirtyColorsOfPoints, SIGNAL(timeout()), this, SLOT(mustDirtyColorsOfItemDrawablesWithPoints()), Qt::QueuedConnection);

    connect(this, SIGNAL(startUpdateColorsTimer()), &m_timerUpdateColors, SLOT(start()), Qt::QueuedConnection);
    connect(this, SIGNAL(startDirtyColorsOfPointTimer()), &m_timerDirtyColorsOfPoints, SLOT(start()), Qt::QueuedConnection);
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

void GDocumentViewForGraphics::updateDrawing3DOfItemDrawablesInGraphicsView(const QList<CT_AbstractItemDrawable *> &items)
{
    lockGraphics();

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
        it.next()->updateDrawing3DOfItemDrawables(items);

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

void GDocumentViewForGraphics::redrawGraphics(DocumentInterface::RedrawOptions opt)
{
    m_mutex->lock();

    if(!_graphicsLocked)
    {
        QListIterator<GGraphicsView*> it(_listGraphics);

        while(it.hasNext())
            it.next()->redraw(opt.testFlag(DocumentInterface::RO_WaitForConversionCompleted) ? DM_GraphicsView::RO_WaitForConversionCompleted : DM_GraphicsView::RO_NoOptions);
    }

    m_mutex->unlock();
}

void GDocumentViewForGraphics::updateItems(const QList<CT_AbstractItemDrawable *> &items)
{
    updateDrawing3DOfItemDrawablesInGraphicsView(items);
}

void GDocumentViewForGraphics::dirtyColorsOfPoints()
{
    emit startDirtyColorsOfPointTimer();
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
    if(item->result() == NULL) {
        GUI_LOG->addErrorMessage(LogInterface::gui, tr("Impossible d'affecter une couleur à un item dont le résultat est NULL"));
        return;
    }

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

    CT_AbstractItemGroup *group = dynamic_cast<CT_AbstractItemGroup*>((CT_AbstractItemDrawable*)item);

    if(group != NULL)
        recursiveSetColor(group, color);

    emit startUpdateColorsTimer();
}

bool GDocumentViewForGraphics::isColorModified(const CT_AbstractItemDrawable *item)
{
    QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*> *hash = getItemsInformations().value(item->result(), NULL);

    if(hash != NULL)
    {
        DM_ItemInfoForGraphics *info = static_cast<DM_ItemInfoForGraphics*>(hash->value((CT_AbstractItemDrawable*)item, NULL));

        if(info != NULL)
            return info->isColorModified();
    }

    return false;
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
    else
    {
        GUI_LOG->addErrorMessage(LogInterface::gui, tr("Impossible de récupérer une couleur d'un item dont le résultat est NULL"));
    }

    return QColor();
}

GOsgGraphicsView::ColorArrayType *GDocumentViewForGraphics::getOrCreateGlobalColorArrayForPoints()
{
    if(m_pointsColorCloudRegistered.isNull())
        m_pointsColorCloudRegistered = PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithPointCloud);

    return m_pointsColorCloudRegistered->cloudT()->osgArray();
}

CT_CCR GDocumentViewForGraphics::getGlobalColorArrayRegisteredForPoints() const
{
    return m_pointsColorCloudRegistered;
}

GDocumentViewForGraphics::AttribCloudType::AType* GDocumentViewForGraphics::getOrCreateGlobalAttribArrayForPoints()
{
    if(m_pointsAttribCloudRegistered.isNull())
        m_pointsAttribCloudRegistered = PS_REPOSITORY->createNewCloudT< AttribCloudRegisteredType, AttribCloudType >(CT_Repository::SyncWithPointCloud);

    return m_pointsAttribCloudRegistered->cloudT()->osgArray();
}

bool GDocumentViewForGraphics::canChangeVisibility() const
{
    return true;
}

void GDocumentViewForGraphics::setVisible(const CT_AbstractItemDrawable *item, bool visible)
{
    lockGraphics();

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
        it.next()->setVisible(item, visible);

    unlockGraphics();
}

bool GDocumentViewForGraphics::isVisible(const CT_AbstractItemDrawable *item) const
{
    bool isVisible = false;

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext()) {
        if(it.next()->isVisible(item))
            isVisible = true;
    }

    return isVisible;
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
}

void GDocumentViewForGraphics::showOptions()
{
    if(_listGraphics.size() > 0)
    {
        DM_GraphicsViewOptions opt;
        opt.updateFromOtherOptions(((const GGraphicsView*)_listGraphics.at(0))->constGetOptionsInternal());

        _graphicsOptionsView->setOptions(opt);

        const QRect screen = QApplication::desktop()->screenGeometry();
        _graphicsOptionsView->move(screen.center() - _graphicsOptionsView->rect().center());

        if(_graphicsOptionsView->exec())
            validateOptions();
    }
}

void GDocumentViewForGraphics::validateOptions()
{
    const DM_GraphicsViewOptions &options = _graphicsOptionsView->getOptions();

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
        it.next()->setOptions(options);
}

void GDocumentViewForGraphics::takeAndSaveScreenshot()
{
    if(!_listGraphics.isEmpty())
        _listGraphics.at(0)->takeAndSaveScreenshot();
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
                                                             cam->cx(),
                                                             cam->cy(),
                                                             cam->cz(),
                                                             cam->focusDistance(),
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

        cam->setPointOfView(pof->x(), pof->y(), pof->z(), pof->distance(), pof->q0(), pof->q1(), pof->q2(), pof->q3(), true);
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

void GDocumentViewForGraphics::changePixelSize()
{
    DM_GraphicsViewOptions opt;
    opt.updateFromOtherOptions(((const GGraphicsView*)_listGraphics.at(0))->constGetOptionsInternal());

    if (_pixelSize == PX_1)
    {
        _pixelSize = PX_2;
        _buttonPixelSize->setIcon(QIcon(":/Icones/Icones/px_2.png"));
        opt.setPointSize(2);
    } else if (_pixelSize == PX_2)
    {
        _pixelSize = PX_3;
        _buttonPixelSize->setIcon(QIcon(":/Icones/Icones/px_3.png"));
        opt.setPointSize(3);
    } else if (_pixelSize == PX_3)
    {
        _pixelSize = PX_1;
        _buttonPixelSize->setIcon(QIcon(":/Icones/Icones/px_1.png"));
        opt.setPointSize(1);
    }

    opt.drawFastest(_drawMode);

    _graphicsOptionsView->setOptions(opt);

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

    DM_GraphicsViewOptions opt;
    opt.updateFromOtherOptions(((const GGraphicsView*)_listGraphics.at(0))->constGetOptionsInternal());
    opt.drawFastest(_drawMode);

    _graphicsOptionsView->setOptions(opt);

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
    DM_GraphicsViewOptions opt;
    opt.updateFromOtherOptions(((const GGraphicsView*)_listGraphics.at(0))->constGetOptionsInternal());
    opt.useTransparency(activated);

    _graphicsOptionsView->setOptions(opt);

    validateOptions();
}

void GDocumentViewForGraphics::setCameraType(bool orthographic)
{
    DM_GraphicsViewOptions opt;
    opt.updateFromOtherOptions(((const GGraphicsView*)_listGraphics.at(0))->constGetOptionsInternal());
    if (orthographic)
    {
        opt.setCameraType(CameraInterface::ORTHOGRAPHIC);
    } else {
        opt.setCameraType(CameraInterface::PERSPECTIVE);
    }

    _graphicsOptionsView->setOptions(opt);

    validateOptions();
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

void GDocumentViewForGraphics::mustUpdateItemDrawablesThatColorWasModified()
{
    if(!_listGraphics.isEmpty())
        _listGraphics.first()->updateItemDrawablesThatColorWasModified();

    QHashIterator<CT_AbstractResult *, QHash<CT_AbstractItemDrawable *, DM_AbstractInfo *> *> it = getItemsInformations();

    while(it.hasNext())
    {
        it.next();
        QHashIterator<CT_AbstractItemDrawable *, DM_AbstractInfo *> it2(*it.value());

        while(it2.hasNext())
            static_cast<DM_ItemInfoForGraphics*>(it2.next().value())->setColorModified(false);
    }
}

void GDocumentViewForGraphics::mustDirtyColorsOfItemDrawablesWithPoints()
{
    if(!_listGraphics.isEmpty())
        _listGraphics.first()->dirtyColorsOfItemDrawablesWithPoints();
}

void GDocumentViewForGraphics::closeEvent(QCloseEvent *closeEvent)
{
    if(canClose())
    {
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

    QPushButton *screenshotButton = new QPushButton(widgetContainer);
    screenshotButton->setToolTip(tr("Enregistrer une capture d'écran"));
    screenshotButton->setIcon(QIcon(":/Icones/Icones/screenshot.png"));

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

    connect(GUI_MANAGER->getPluginManager(), SIGNAL(finishLoading()), this, SLOT(pluginExporterManagerReloaded()));

    // fenetre de configuration des options
    _graphicsOptionsView = new GGraphicsViewOptions();

    // bouton qui appelle la fentre de configuration des options
    QPushButton *buttonShowOptions = new QPushButton(widgetContainer);
    buttonShowOptions->setMaximumWidth(33);
    buttonShowOptions->setMinimumWidth(33);
    buttonShowOptions->setToolTip(tr("Configurer l'affichage"));
    buttonShowOptions->setIcon(QIcon(":/Icones/Icones/preferences-system.png"));

    layout->addWidget(screenshotButton);
    layout->addWidget(_pointOfViewButton);
    layout->addWidget(_buttonExport);
    layout->addWidget(buttonShowOptions);
    layout->addWidget(buttonPointsAttributes);
    layout->addWidget(_buttonDrawMode);
    layout->addWidget(_buttonPixelSize);
    layout->addWidget(_cameraOptionsView);

    ((QVBoxLayout*)parent->layout())->insertWidget(0, widgetContainer);

    connect(screenshotButton, SIGNAL(clicked()), this, SLOT(takeAndSaveScreenshot()));
    connect(buttonShowOptions, SIGNAL(clicked()), this, SLOT(showOptions()));
    connect(buttonPointsAttributes, SIGNAL(clicked()), this, SLOT(showAttributesOptions()));
    connect(_buttonPixelSize, SIGNAL(clicked()), this, SLOT(changePixelSize()));
    connect(_buttonDrawMode, SIGNAL(clicked()), this, SLOT(changeDrawMode()));
    connect(_pointOfViewMenu, SIGNAL(addActualPointOfView()), this, SLOT(addActualPointOfView()));
    connect(_pointOfViewMenu, SIGNAL(setPointOfView(DM_PointOfView*)), this, SLOT(setPointOfView(DM_PointOfView*)), Qt::DirectConnection);

    connect(_cameraOptionsView, SIGNAL(syncGraphics(bool)), this, SLOT(syncChanged(bool)));

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

            hash = getItemsInformations().value(lastResult, NULL);

            if(hash == NULL)
                hash = createItemInformationsForResult(lastResult);
        }

        if(hash != NULL) {
            DM_ItemInfoForGraphics *childInfo = static_cast<DM_ItemInfoForGraphics*>(hash->value(child, NULL));

            if(childInfo == NULL)
            {
                childInfo = static_cast<DM_ItemInfoForGraphics*>(createNewItemInformation(child));
                hash->insert(child, childInfo);
            }

            childInfo->setColor(color);

            recursiveSetColor(child, color);
        }

    }

    CT_ItemIterator itI(group);

    while(itI.hasNext())
    {
        CT_AbstractSingularItemDrawable *child = (CT_AbstractSingularItemDrawable*)itI.next();

        if(lastResult != child->result())
        {
            lastResult = child->result();

            hash = getItemsInformations().value(lastResult, NULL);

            if(hash == NULL)
                hash = createItemInformationsForResult(lastResult);

        }

        if(hash != NULL) {
            DM_ItemInfoForGraphics *childInfo = static_cast<DM_ItemInfoForGraphics*>(hash->value(child, NULL));

            if(childInfo == NULL)
            {
                childInfo = static_cast<DM_ItemInfoForGraphics*>(createNewItemInformation(child));
                hash->insert(child, childInfo);
            }

            childInfo->setColor(color);
        }
    }
}
