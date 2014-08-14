#include "gdocumentviewforgraphics.h"

#include "dm_guimanager.h"

#include "view/DocumentView/GraphicsViews/PointsAttributes/gpointsattributesmanager.h"
#include "cdm_tools.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>

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

void GDocumentViewForGraphics::addItemDrawable(ItemDrawable &item)
{
    if(!_graphicsLocked)
    {
        lockGraphics();

        GDocumentView::addItemDrawable(item);

        unlockGraphics();
    }
    else
    {
        GDocumentView::addItemDrawable(item);
    }
}

void GDocumentViewForGraphics::endAddMultipleItemDrawable()
{
    unlockGraphics();
}

void GDocumentViewForGraphics::beginRemoveMultipleItemDrawable()
{
    lockGraphics();
}

void GDocumentViewForGraphics::removeItemDrawable(ItemDrawable &item)
{
    if(!_graphicsLocked)
    {
        lockGraphics();

        GDocumentView::removeItemDrawable(item);

        unlockGraphics();
    }
    else
    {
        GDocumentView::removeItemDrawable(item);
    }
}

void GDocumentViewForGraphics::endRemoveMultipleItemDrawable()
{
    unlockGraphics();
}

void GDocumentViewForGraphics::removeAllItemDrawableOfResult(Result &res)
{
    lockGraphics();

    GDocumentView::removeAllItemDrawableOfResult(res);

    unlockGraphics();
}

void GDocumentViewForGraphics::removeAllItemDrawableOfModel(IItemModel &model)
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

bool GDocumentViewForGraphics::acceptAction(const ActionInterface *action) const
{
    return (action == NULL) || (dynamic_cast<const ActionForGraphicsViewInterface*>(action) != NULL);
}

bool GDocumentViewForGraphics::setCurrentAction(ActionInterface *action, bool deleteAction)
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
            it.next()->setCurrentAction(dynamic_cast<ActionForGraphicsViewInterface*>(action->copy()));
    }

    redrawGraphics();

    if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
        delete action;

    return true;
}

bool GDocumentViewForGraphics::setDefaultAction(ActionInterface *action, bool deleteAction)
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
            it.next()->setDefaultAction(dynamic_cast<ActionForGraphicsViewInterface*>(action->copy()));
    }

    redrawGraphics();

    if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
        delete action;

    return true;
}

ActionInterface* GDocumentViewForGraphics::currentAction() const
{
    if(_listGraphics.isEmpty())
        return NULL;

    return _listGraphics.first()->actionsHandler()->currentAction();
}

ActionInterface *GDocumentViewForGraphics::defaultAction() const
{
    if(_listGraphics.isEmpty())
        return NULL;

    return _listGraphics.first()->actionsHandler()->defaultAction();
}

void GDocumentViewForGraphics::removeActions(const QString &uniqueName) const
{
    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        it.next()->actionsHandler()->removeActions(uniqueName);
    }
}

QString GDocumentViewForGraphics::getType() const
{
    return _type;
}

template<>
void GDocumentViewForGraphics::createColorCloudRegistered<IPointAttributes>()
{
    if(colorCloudRegistered<IPointAttributes>().data() == NULL)
        setColorCloudRegistered<IPointAttributes>(GUI_MANAGER->getPluginsContext()->repository()->createNewColorCloud(RepositoryInterface::SyncWithPointCloud, false));
}

template<>
void GDocumentViewForGraphics::createColorCloudRegistered<IFaceAttributes>()
{
    if(colorCloudRegistered<IFaceAttributes>().data() == NULL)
        setColorCloudRegistered<IFaceAttributes>(GUI_MANAGER->getPluginsContext()->repository()->createNewColorCloud(RepositoryInterface::SyncWithFaceCloud, false));
}

template<>
void GDocumentViewForGraphics::createColorCloudRegistered<IEdgeAttributes>()
{
    if(colorCloudRegistered<IEdgeAttributes>().data() == NULL)
        setColorCloudRegistered<IEdgeAttributes>(GUI_MANAGER->getPluginsContext()->repository()->createNewColorCloud(RepositoryInterface::SyncWithEdgeCloud, false));
}

template<>
void GDocumentViewForGraphics::setColorCloudRegistered<IPointAttributes>(QSharedPointer<ColorCloudRegisteredInterface> cc)
{
    lock();

    m_pColorCloudRegistered = cc;

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        it.next()->setCurrentPointCloudColor(cc);
    }

    unlock();
}

template<>
void GDocumentViewForGraphics::setColorCloudRegistered<IFaceAttributes>(QSharedPointer<ColorCloudRegisteredInterface> cc)
{
    lock();

    m_fColorCloudRegistered = cc;

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        it.next()->setCurrentFaceCloudColor(cc);
    }

    unlock();
}

template<>
void GDocumentViewForGraphics::setColorCloudRegistered<IEdgeAttributes>(QSharedPointer<ColorCloudRegisteredInterface> cc)
{
    lock();

    m_eColorCloudRegistered = cc;

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        it.next()->setCurrentEdgeCloudColor(cc);
    }

    unlock();
}

template<>
QSharedPointer<ColorCloudRegisteredInterface> GDocumentViewForGraphics::colorCloudRegistered<IPointAttributes>() const
{
    return m_pColorCloudRegistered;
}

template<>
QSharedPointer<ColorCloudRegisteredInterface> GDocumentViewForGraphics::colorCloudRegistered<IFaceAttributes>() const
{
    return m_fColorCloudRegistered;
}

template<>
QSharedPointer<ColorCloudRegisteredInterface> GDocumentViewForGraphics::colorCloudRegistered<IEdgeAttributes>() const
{
    return m_eColorCloudRegistered;
}

void GDocumentViewForGraphics::setUseColorCloud(bool use)
{
    m_useColorCloud = use;

    lock();

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        it.next()->setUseCloudColor(m_useColorCloud);
    }

    unlock();
}

bool GDocumentViewForGraphics::useColorCloud() const
{
    return m_useColorCloud;
}

template<>
void GDocumentViewForGraphics::setNormalCloudRegistered<IPointAttributes>(QSharedPointer<NormalCloudRegisteredInterface> nn)
{
    lock();

    m_pNormalCloudRegistered = nn;

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        it.next()->setCurrentPointCloudNormal(nn);
    }

    unlock();
}

template<>
void GDocumentViewForGraphics::setNormalCloudRegistered<IFaceAttributes>(QSharedPointer<NormalCloudRegisteredInterface> nn)
{
    lock();

    m_fNormalCloudRegistered = nn;

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        it.next()->setCurrentFaceCloudNormal(nn);
    }

    unlock();
}

template<>
QSharedPointer<NormalCloudRegisteredInterface> GDocumentViewForGraphics::normalCloudRegistered<IPointAttributes>() const
{
    return m_pNormalCloudRegistered;
}

template<>
QSharedPointer<NormalCloudRegisteredInterface> GDocumentViewForGraphics::normalCloudRegistered<IFaceAttributes>() const
{
    return m_fNormalCloudRegistered;
}

void GDocumentViewForGraphics::setUseNormalCloud(bool use)
{
    m_useNormalCloud = use;

    lock();

    QListIterator<GGraphicsView*> it(_listGraphics);

    while(it.hasNext())
    {
        it.next()->setUseCloudNormal(m_useNormalCloud);
    }

    unlock();
}

bool GDocumentViewForGraphics::useNormalCloud() const
{
    return m_useNormalCloud;
}

void GDocumentViewForGraphics::showOptions()
{
    if(_listGraphics.size() > 0)
    {
        const DM_GraphicsViewOptions &opt = ((const GGraphicsView*)_listGraphics.at(0))->getOptions();

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
    {
        it.next()->setOptions(options);
    }
}


void GDocumentViewForGraphics::addActualPointOfView()
{
    QList<DM_PointOfView> listPof = _pofManager.getPointOfViewList(getKeyForPointOfViewManager());

    // on limite  20 le nombre de point de vue
    if(listPof.size() == 20)
    {
        _pofManager.removePointOfView(getKeyForPointOfViewManager(), listPof.takeLast());
    }

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

void GDocumentViewForGraphics::showPointsAttributesOptions()
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

    IExporter *exporter = dynamic_cast<IExporter*>(sender()->parent());

    IExporter *exCopy = exporter->copy();
    exCopy->init();

    if(exCopy->canExportItems())
        exCopy->setItemDrawableToExport(getSelectedItemDrawable());

    if(exCopy->canExportPoints())
    {
        QList<ICloudIndex*> points;

        QListIterator<GGraphicsView*> it(_listGraphics);

        while(it.hasNext())
            points.append(it.next()->getSelectedPoints()->indexCloud());

        exCopy->setPointsToExport(points);
    }

    if(exCopy->canExportFaces())
    {
        QList<ICloudIndex*> faces;

        QListIterator<GGraphicsView*> it(_listGraphics);

        while(it.hasNext())
            faces.append(it.next()->getSelectedFaces()->indexCloud());

        exCopy->setFacesToExport(faces);
    }

    if(exCopy->canExportEdges())
    {
        QList<ICloudIndex*> edges;

        QListIterator<GGraphicsView*> it(_listGraphics);

        while(it.hasNext())
            edges.append(it.next()->getSelectedEdges()->indexCloud());

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

    // bouton qui permet d'exporter les ItemDrawable selectionnes
    _buttonExport = new QPushButton(widgetContainer);
    _buttonExport->setMaximumWidth(33);
    _buttonExport->setMinimumWidth(33);
    _buttonExport->setToolTip(trUtf8("Exporter les \xc3\xa9l\xc3\xa9ments s\xc3\xa9lectionn\xc3\xa9s"));
    _buttonExport->setIcon(QIcon(":/Icones/Icones/upload.png"));
    _buttonExport->setEnabled(false);

    QPushButton *buttonPointsAttributes = new QPushButton(widgetContainer);
    buttonPointsAttributes->setMaximumWidth(33);
    buttonPointsAttributes->setMinimumWidth(33);
    buttonPointsAttributes->setToolTip(trUtf8("Configurer les couleurs des points"));
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

    layout->addWidget(_pointOfViewButton);
    layout->addWidget(_buttonExport);
    layout->addWidget(buttonShowOptions);
    layout->addWidget(buttonPointsAttributes);
    layout->addWidget(_buttonDrawMode);
    layout->addWidget(_buttonPixelSize);
    layout->addWidget(_cameraOptionsView);

    ((QVBoxLayout*)parent->layout())->insertWidget(0, widgetContainer);

    connect(buttonShowOptions, SIGNAL(clicked()), this, SLOT(showOptions()));
    connect(buttonPointsAttributes, SIGNAL(clicked()), this, SLOT(showPointsAttributesOptions()));
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
