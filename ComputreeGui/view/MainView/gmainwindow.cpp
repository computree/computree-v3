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


#include "gmainwindow.h"
#include "ui_gmainwindow.h"

#include "dm_guimanager.h"

#include "tools/graphicsview/dm_pointofviewmanager.h"

#include "gdocumentviewforgraphics.h"
#include "gdocumentviewforitemmodel.h"

#include "GraphicsViews/3D/g3dgraphicsview.h"

#include "gaboutdialog.h"
#include "gaboutpluginsdialog.h"
#include "gineedhelpdialog.h"

#include "myqaction.h"
#include "myqmenu.h"

#include "ct_steploadfileseparator.h"
#include "ct_stepcanbeaddedfirstseparator.h"
#include "ct_abstractstepplugin.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"
#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"
#include "ct_global/ct_context.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

#include <QSpinBox>

GMainWindow::GMainWindow(QWidget *parent) :
    QMainWindow(parent), DM_MainWindow(),
    ui(new Ui::GMainWindow)
{
    ui->setupUi(this);

    initUI();

    arrangeDocksInColumn();
}

GMainWindow::~GMainWindow()
{
    delete ui;

    writeConfiguration();

    delete _stepManagerView;
    delete _docManagerView;
}

void GMainWindow::closeEvent(QCloseEvent *ev)
{
    DM_Context *context = new DM_Context();
    //connect(context, SIGNAL(actionFinished(DM_Context*)), this, SLOT(close()), Qt::QueuedConnection);

    int res;

    if((res = GUI_MANAGER->asyncRemoveAllStep(context)) != -1)
    {
        if(res == 0)
        {
            if(GUI_MANAGER->getStepManager()->isInManualMode())
                QMessageBox::warning(this, tr("Attention"), tr("Vous êtes dans le mode manuel, veuillez quitter ce mode avant de fermer l'application."), QMessageBox::Ok);
            else if(GUI_MANAGER->getStepManager()->isRunning())
                QMessageBox::warning(this, tr("Attention"), tr("Une étape est en cours de traiements, veuillez terminer les traitements avant de fermer l'application."), QMessageBox::Ok);
        }

        ev->ignore();
    }
}

DocumentManagerInterface* GMainWindow::documentManager() const
{
    return getDocumentManagerView();
}

DM_DocumentManagerView* GMainWindow::getDocumentManagerView() const
{
    return _docManagerView;
}

DM_MultipleItemDrawableModelManager* GMainWindow::getItemDrawableModelManager() const
{
    return _itemDrawableModelView;
}

void GMainWindow::newDocument()
{
    _docManagerView->new3DDocument(true);
}

void GMainWindow::new2DDocument()
{
    _docManagerView->new2DDocument(true);
}

void GMainWindow::newItemModelDocument()
{
    _docManagerView->newTreeViewDocument(true);
}

void GMainWindow::openFile()
{
    openFile(_defaultOpenDirPath);
}

void GMainWindow::openFile(QString &loadDirectory)
{
    QString s = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"), loadDirectory, createFileExtensionAvailable());

    if(!s.isEmpty())
    {
        QFileInfo info(s);

        loadDirectory = info.path();

        if(getScriptManager()->acceptFile(s))
        {
            QString error = getScriptManager()->loadScript(s, *getStepManager());

            if(!error.isEmpty())
            {
                QMessageBox::critical(this, tr("Erreur chargement du script"), tr("Une erreur est survenu lors de la lecture du script :\r\n\r\n%1").arg(error));
            }
        }
        else
        {
            _stepManagerView->addOpenFileStep(s);
        }
    }
}

void GMainWindow::saveScript()
{
    saveScript(_defaultSaveDirPath);
}

void GMainWindow::saveScript(QString &saveDirectory)
{
    QString s = QFileDialog::getSaveFileName(this, tr("Sauvegarder l'arbre des tapes sous..."), saveDirectory, createScriptManagerExtension(""));

    if(!s.isEmpty())
    {
        QFileInfo info(s);

        if(info.suffix().isEmpty())
        {
            s += getScriptManager()->getFileExtensionAccepted().first();
        }

        saveDirectory = info.path();

        if(getScriptManager()->acceptFile(s))
        {
            getScriptManager()->writeScript(s, true, *getStepManager());
        }
    }
}

void GMainWindow::showAboutDialog()
{
    GAboutDialog dialog;
    dialog.exec();
}

void GMainWindow::showAboutPluginsDialog()
{
    CDM_PluginManager *pluginManager = getPluginManager();

    GAboutPluginsDialog dialog(*pluginManager,
                               this);

    dialog.exec();

    if(dialog.mustReloadStepPlugins())
    {
        loadPlugins();
    }
}

void GMainWindow::configureCurrentCoordinateSystem()
{
    PS_COORDINATES_SYS->configure();
}

void GMainWindow::cleanItemDrawableOfAllDocuments()
{
    GUI_MANAGER->cleanItemDrawableOfAllDocuments();
}

void GMainWindow::showINeedHelpDialog()
{
    GINeedHelpDialog dialog;
    dialog.exec();
}

GMainProgressDialog* GMainWindow::createWaitProgressDialog()
{
    return new GMainProgressDialog(this);
}

void GMainWindow::arrangeDocksInTabs()
{
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetStepManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableModelManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetActionsManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetLog);

    tabifyDockWidget(ui->dockWidgetItemDrawableModelManager, ui->dockWidgetActionsManager);
    tabifyDockWidget(ui->dockWidgetActionsManager, ui->dockWidgetLog);

    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableConfigurationManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetGraphicsViewSynchronizedGroup);
    tabifyDockWidget(ui->dockWidgetItemDrawableConfigurationManager, ui->dockWidgetGraphicsViewSynchronizedGroup);

    ui->dockWidgetItemDrawableModelManager->raise();
    ui->dockWidgetItemDrawableConfigurationManager->raise();
    ui->dockWidgetStepManager->raise();
}

void GMainWindow::arrangeDocksInColumnWithLogAtBottom()
{
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetStepManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableModelManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableConfigurationManager);
    addDockWidget(Qt::RightDockWidgetArea, ui->dockWidgetActionsManager);
    addDockWidget(Qt::RightDockWidgetArea, ui->dockWidgetGraphicsViewSynchronizedGroup);
    addDockWidget(Qt::BottomDockWidgetArea, ui->dockWidgetLog);

    ui->dockWidgetStepManager->raise();
}

void GMainWindow::arrangeDocksInColumn()
{
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetStepManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableModelManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableConfigurationManager);
    addDockWidget(Qt::RightDockWidgetArea, ui->dockWidgetActionsManager);
    addDockWidget(Qt::RightDockWidgetArea, ui->dockWidgetLog);

    tabifyDockWidget(ui->dockWidgetActionsManager, ui->dockWidgetGraphicsViewSynchronizedGroup);

    ui->dockWidgetActionsManager->raise();
    ui->dockWidgetStepManager->raise();
}

///////////// PRIVATE ////////////

void GMainWindow::initUI()
{
    getPluginManager()->log()->addNormalLogListener(ui->widgetLog);

    _docManagerView = new GDocumentManagerView(this);
    getActionsManager()->setDocumentManagerView(_docManagerView);

    ui->widgetActionsManager->setActionsManager(getActionsManager());
    ui->widgetActionsManager->setDocumentManagerView(_docManagerView);

    QAction *actionOpenFile = new QAction(tr("Ouvrir un fichier"), this);
    actionOpenFile->setIcon(QIcon(":/Icones/Icones/folder_add_32.png"));

    menuNewStepCanBeAddedFirst = new QMenu(tr("Ajouter une étape"), this);
    menuNewStepCanBeAddedFirst->setToolTip(tr("Ajouter une étape qui n'a pas besoin de résultat en entrée"));
    menuNewStepCanBeAddedFirst->setIcon(QIcon(":/Icones/Icones/add.png"));

    QAction *actionStart = new QAction(tr("Lancer les traitements"), this);
    actionStart->setIcon(QIcon(QPixmap(":/Icones/Icones/play.png").scaled(QSize(20,20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    QAction *actionStop = new QAction(tr("Stop"), this);
    actionStop->setIcon(QIcon(QPixmap(":/Icones/Icones/stop.png").scaled(QSize(20,20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    QAction *actionAckManualMode = new QAction(tr("Valider le mode manuel et continuer les traitements automatiques"), this);
    actionAckManualMode->setIcon(QIcon(QPixmap(":/Icones/Icones/hand.png").scaled(QSize(20,20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    actionAckManualMode->setDisabled(true);

    QAction *actionForwardOneStepDebug = new QAction(tr("Lancer les traitements en mode debug ou avancer d'un pas"), this);
    actionForwardOneStepDebug->setIcon(QIcon(":/Icones/Icones/play_debug.png"));

    QAction *actionForwardFastStepDebug = new QAction(tr("Lancer les traitements en mode debug ou avancer de N pas"), this);
    actionForwardFastStepDebug->setIcon(QIcon(":/Icones/Icones/play_debug_fast.png"));

    QAction *actionNewDocument = new QAction(tr("Ajouter un nouveau document"), this);
    actionNewDocument->setIcon(QIcon(QPixmap(":/Icones/Icones/new-document.png").scaled(QSize(20,20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    QAction *actionNew2DDocument = new QAction(tr("Ajouter un nouveau document 2D"), this);
    actionNew2DDocument->setIcon(QIcon(":/Icones/Icones/new-document-2d.png"));

    QAction *actionNewItemModelDocument = new QAction(tr("Ajouter un nouveau document de type tableur"), this);
    actionNewItemModelDocument->setIcon(QIcon(":/Icones/Icones/new-document-treeview.png"));

    QAction *actionStepManagerConfiguration = new QAction(tr("Configurer"), this);
    actionStepManagerConfiguration->setIcon(QIcon(":/Icones/Icones/preferences-system.png"));

    QAction *actionCoordinateSystem = new QAction(tr("Système de coordonnées"), this);
    actionCoordinateSystem->setIcon(QIcon(":/Icones/Icones/maps.png"));

    QAction *actionCleanAllDocuments = new QAction(tr("Nettoyer toutes les vues"), this);
    actionCleanAllDocuments->setIcon(QIcon(":/Icones/Icones/broom.png"));

    QAction *actionINeedHelp = new QAction(tr("J'ai besoin d'aide !!!"), this);
    actionINeedHelp->setIcon(QIcon(":/Icones/Icones/help.png"));

    QAction *actionQuitter = new QAction(tr("Quitter"), this);

    QAction *actionShowStepManagerView = new QAction(ui->dockWidgetStepManager->windowTitle(), this);
    actionShowStepManagerView->setDisabled(true);
    QAction *actionShowItemDrawableModelManagerView = new QAction(ui->dockWidgetItemDrawableModelManager->windowTitle(), this);
    actionShowItemDrawableModelManagerView->setDisabled(true);
    QAction *actionShowItemDrawableConfigurationManagerView = new QAction(ui->dockWidgetItemDrawableConfigurationManager->windowTitle(), this);
    actionShowItemDrawableConfigurationManagerView->setDisabled(true);
    QAction *actionShowGraphicsSyncManagerView = new QAction(ui->dockWidgetGraphicsViewSynchronizedGroup->windowTitle(), this);
    actionShowGraphicsSyncManagerView->setDisabled(true);
    QAction *actionShowActionsManagerView = new QAction(ui->dockWidgetActionsManager->windowTitle(), this);
    actionShowActionsManagerView->setDisabled(true);
    QAction *actionShowLog = new QAction(ui->dockWidgetLog->windowTitle(), this);
    actionShowLog->setDisabled(true);

    QAction *actionArrangeDocksInTabs = new QAction(tr("Composants en onglets"), this);
    QAction *actionArrangeDocksInColumn = new QAction(tr("Composants en colonne"), this);
    QAction *actionArrangeDocksInColumnWithLogAtBottom = new QAction(tr("Composants en colonne (Log en bas)"), this);

    QAction *actionAPropos = new QAction(tr("A propos de Computree..."), this);
    actionAProposPlugin = new QAction(tr("A propos des plugins..."), this);

    actionSaveScript = new QAction(tr("Sauvegarder l'arbre des etapes"), this);
    actionSaveScript->setIcon(QIcon(":/Icones/Icones/media-floppy.png"));

    QToolButton *toolButton = new QToolButton(this);
    toolButton->setIcon(menuNewStepCanBeAddedFirst->icon());
    toolButton->setToolTip(menuNewStepCanBeAddedFirst->toolTip());
    toolButton->setMenu(menuNewStepCanBeAddedFirst);
    toolButton->setPopupMode(QToolButton::InstantPopup);

    ui->toolBar->addAction(actionOpenFile);
    ui->toolBar->addWidget(toolButton);
    ui->toolBar->addAction(actionStart);
    ui->toolBar->addAction(actionStop);
    ui->toolBar->addAction(actionSaveScript);


    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionAckManualMode);

    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionForwardOneStepDebug);
    ui->toolBar->addAction(actionForwardFastStepDebug);

    QSpinBox *fastForwardSpinBox = new QSpinBox(this);
    fastForwardSpinBox->setMinimum(1);
    fastForwardSpinBox->setMaximum(9999999);

    ui->toolBar->addWidget(fastForwardSpinBox);

    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionNewDocument);
    ui->toolBar->addAction(actionNew2DDocument);
    ui->toolBar->addAction(actionNewItemModelDocument);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionStepManagerConfiguration);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionCoordinateSystem);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionCleanAllDocuments);
    ui->toolBar->addAction(actionINeedHelp);

    ui->menuFichier->addAction(actionOpenFile);
    ui->menuFichier->addSeparator();
    ui->menuFichier->addAction(actionSaveScript);
    ui->menuFichier->addSeparator();
    ui->menuFichier->addAction(actionNewDocument);
    ui->menuFichier->addAction(actionNew2DDocument);
    ui->menuFichier->addAction(actionNewItemModelDocument);
    ui->menuFichier->addSeparator();
    ui->menuFichier->addMenu(menuNewStepCanBeAddedFirst);
    ui->menuFichier->addSeparator();
    ui->menuFichier->addAction(actionQuitter);

    ui->menuEdition->addAction(actionStart);
    ui->menuEdition->addAction(actionStop);
    ui->menuEdition->addSeparator();
    ui->menuEdition->addAction(actionAckManualMode);
    ui->menuEdition->addSeparator();
    ui->menuEdition->addAction(actionForwardOneStepDebug);
    ui->menuEdition->addAction(actionForwardFastStepDebug);

    ui->menuFenetre->addAction(actionShowStepManagerView);
    ui->menuFenetre->addAction(actionShowItemDrawableModelManagerView);
    ui->menuFenetre->addAction(actionShowItemDrawableConfigurationManagerView);
    ui->menuFenetre->addAction(actionShowGraphicsSyncManagerView);
    ui->menuFenetre->addAction(actionShowActionsManagerView);
    ui->menuFenetre->addAction(actionShowLog);
    ui->menuFenetre->addAction(actionArrangeDocksInTabs);
    ui->menuFenetre->addAction(actionArrangeDocksInColumnWithLogAtBottom);
    ui->menuFenetre->addAction(actionArrangeDocksInColumn);

    ui->menuAide->addAction(actionAPropos);
    ui->menuAide->addAction(actionAProposPlugin);
    ui->menuAide->addAction(actionINeedHelp);

    ui->horizontalLayoutDocumentView->addWidget(_docManagerView);

    _stepManagerView = new GStepManager(*getStepManager(), this);
    _stepManagerView->getStepManager()->setGuiContext(this);
    ui->verticalLayoutStepTreeView->addWidget(_stepManagerView);

    _itemDrawableModelView = new GMultipleItemDrawableModelManager(this);
    _itemDrawableModelView->setDocumentManagerView(_docManagerView);
    ui->verticalLayoutItemDrawableModelView->addWidget(_itemDrawableModelView);

    _itemDrawableConfigurationView = new GItemDrawableConfigurationManagerView(this);
    ui->verticalLayoutItemDrawableConfigurationManager->addWidget(_itemDrawableConfigurationView);

    _graphicsViewSyncGroupView = new GGraphicsViewSynchronizedGroup(this);
    ui->verticalLayoutGraphicsViewSyncGroup->addWidget(_graphicsViewSyncGroupView);

    _docManagerView->setSyncManager(_graphicsViewSyncGroupView);

    _permanentLabelForMessage = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(_permanentLabelForMessage);

    actionStop->setEnabled(false);
    actionSaveScript->setEnabled(false);

    QStringList dLA = GUI_MANAGER->getLanguageManager()->displayableLanguageAvailable();
    QStringList lA = GUI_MANAGER->getLanguageManager()->languageAvailable();
    int i = 0;

    QActionGroup *actGroup = new QActionGroup(this);
    actGroup->setExclusive(true);

    foreach (QString l, dLA) {
        QAction *languageAction = ui->menuLangue->addAction(QIcon(QString("./img") + QDir::separator() + "flag_" + lA.at(i) + ".png"), l, this, SLOT(changeLanguage()));
        languageAction->setData(i);
        languageAction->setCheckable(true);
        languageAction->setChecked(GUI_MANAGER->getLanguageManager()->currentLanguage() == i);
        actGroup->addAction(languageAction);
        ++i;
    }

    connect(actionNewDocument, SIGNAL(triggered()), this, SLOT(newDocument()));
    connect(actionNew2DDocument, SIGNAL(triggered()), this, SLOT(new2DDocument()));
    connect(actionNewItemModelDocument, SIGNAL(triggered()), this, SLOT(newItemModelDocument()));
    connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(actionQuitter, SIGNAL(triggered()), this, SLOT(close()));

    connect(actionStart, SIGNAL(triggered()), _stepManagerView, SLOT(executeStep()));
    connect(actionStop, SIGNAL(triggered()), _stepManagerView->getStepManager(), SLOT(stop()));
    connect(actionSaveScript, SIGNAL(triggered()), this, SLOT(saveScript()));

    connect(actionAckManualMode, SIGNAL(triggered()), getStepManager(), SLOT(quitManualMode()));

    connect(actionForwardOneStepDebug, SIGNAL(triggered()), _stepManagerView, SLOT(executeOrForwardStepInDebugMode()));
    connect(actionForwardFastStepDebug, SIGNAL(triggered()), _stepManagerView, SLOT(executeOrForwardStepFastInDebugMode()));
    connect(fastForwardSpinBox, SIGNAL(valueChanged(int)), _stepManagerView->getStepManager(), SLOT(setFastForwardJumpInDebugMode(int)));

    connect(actionStepManagerConfiguration, SIGNAL(triggered()), _stepManagerView, SLOT(showStepManagerOptions()));

    connect(actionCoordinateSystem, SIGNAL(triggered()), this, SLOT(configureCurrentCoordinateSystem()));

    connect(actionCleanAllDocuments, SIGNAL(triggered()), this, SLOT(cleanItemDrawableOfAllDocuments()));
    connect(actionINeedHelp, SIGNAL(triggered()), this, SLOT(showINeedHelpDialog()));

    connect(actionShowStepManagerView, SIGNAL(triggered()), ui->dockWidgetStepManager, SLOT(showNormal()));
    connect(actionShowItemDrawableModelManagerView, SIGNAL(triggered()), ui->dockWidgetItemDrawableModelManager, SLOT(showNormal()));
    connect(actionShowItemDrawableConfigurationManagerView, SIGNAL(triggered()), ui->dockWidgetItemDrawableConfigurationManager, SLOT(showNormal()));
    connect(actionShowGraphicsSyncManagerView, SIGNAL(triggered()), ui->dockWidgetGraphicsViewSynchronizedGroup, SLOT(showNormal()));
    connect(actionShowActionsManagerView, SIGNAL(triggered()), ui->dockWidgetActionsManager, SLOT(showNormal()));
    connect(actionShowLog, SIGNAL(triggered()), ui->dockWidgetLog, SLOT(showNormal()));
    connect(actionArrangeDocksInTabs, SIGNAL(triggered()), this, SLOT(arrangeDocksInTabs()));
    connect(actionArrangeDocksInColumnWithLogAtBottom, SIGNAL(triggered()), this, SLOT(arrangeDocksInColumnWithLogAtBottom()));
    connect(actionArrangeDocksInColumn, SIGNAL(triggered()), this, SLOT(arrangeDocksInColumn()));

    connect(ui->actionCascade, SIGNAL(triggered()), _docManagerView, SLOT(cascadeSubWindows()));
    connect(ui->actionTile, SIGNAL(triggered()), _docManagerView, SLOT(tileSubWindows()));

    connect(actionAPropos, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(actionAProposPlugin, SIGNAL(triggered()), this, SLOT(showAboutPluginsDialog()));
    connect(menuNewStepCanBeAddedFirst, SIGNAL(aboutToShow()), this, SLOT(menuNewStepCanBeAddedFirstAboutToShow()));

    connect(_stepManagerView->getStepManager(), SIGNAL(started(bool)), actionStart, SLOT(setDisabled(bool)));
    connect(_stepManagerView->getStepManager(), SIGNAL(started(bool)), actionStop, SLOT(setEnabled(bool)));
    connect(_stepManagerView->getStepManager(), SIGNAL(started(bool)), actionStepManagerConfiguration, SLOT(setDisabled(bool)));
    connect(_stepManagerView->getStepManager(), SIGNAL(started(bool)), actionForwardOneStepDebug, SLOT(setDisabled(bool)));
    connect(_stepManagerView->getStepManager(), SIGNAL(started(bool)), actionForwardFastStepDebug, SLOT(setDisabled(bool)));

    connect(_stepManagerView->getStepManager(), SIGNAL(completed(bool)), actionStart, SLOT(setEnabled(bool)));
    connect(_stepManagerView->getStepManager(), SIGNAL(completed(bool)), actionStop, SLOT(setDisabled(bool)));
    connect(_stepManagerView->getStepManager(), SIGNAL(completed(bool)), actionStepManagerConfiguration, SLOT(setEnabled(bool)));
    connect(_stepManagerView->getStepManager(), SIGNAL(completed(bool)), actionForwardOneStepDebug, SLOT(setEnabled(bool)));
    connect(_stepManagerView->getStepManager(), SIGNAL(completed(bool)), actionForwardFastStepDebug, SLOT(setEnabled(bool)));

    connect(_stepManagerView->getStepManager(), SIGNAL(stepInManualMode(bool)), actionAckManualMode, SLOT(setEnabled(bool)));

    connect(_stepManagerView->getStepManager(), SIGNAL(stepFinishExecuted(CT_VirtualAbstractStep*)), _docManagerView, SLOT(stepFinished(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(_stepManagerView->getStepManager(), SIGNAL(stepRequiredManualMode(CT_VirtualAbstractStep*)), _docManagerView, SLOT(stepRequiredManualMode(CT_VirtualAbstractStep*)), Qt::DirectConnection);

    connect(_stepManagerView->getStepManager(), SIGNAL(stepWaitForAckInDebugMode(bool)), actionStart, SLOT(setEnabled(bool)));
    connect(_stepManagerView->getStepManager(), SIGNAL(stepWaitForAckInDebugMode(bool)), actionForwardOneStepDebug, SLOT(setEnabled(bool)));
    connect(_stepManagerView->getStepManager(), SIGNAL(stepWaitForAckInDebugMode(bool)), actionForwardFastStepDebug, SLOT(setEnabled(bool)));

    connect(_stepManagerView->getStepManager(), SIGNAL(stepAdded(CT_VirtualAbstractStep*)), this, SLOT(stepAdded(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(_stepManagerView->getStepManager(), SIGNAL(stepToBeRemoved(CT_VirtualAbstractStep*)), this, SLOT(stepToBeRemoved(CT_VirtualAbstractStep*)), Qt::QueuedConnection);

    connect(_stepManagerView->getStepManager(), SIGNAL(stepNeedShowMessage(QString)), _permanentLabelForMessage, SLOT(setText(QString)));

    connect(_docManagerView, SIGNAL(documentToBeClosed(DM_DocumentView*)), this, SLOT(documentToBeClosed(DM_DocumentView*)), Qt::DirectConnection);
    connect(_docManagerView, SIGNAL(documentActivated(DM_DocumentView*)), _itemDrawableConfigurationView, SLOT(setDocument(DM_DocumentView*)), Qt::DirectConnection);

    connect(ui->dockWidgetStepManager, SIGNAL(visibilityChanged(bool)), actionShowStepManagerView, SLOT(setDisabled(bool)));
    connect(ui->dockWidgetItemDrawableModelManager, SIGNAL(visibilityChanged(bool)), actionShowItemDrawableModelManagerView, SLOT(setDisabled(bool)));
    connect(ui->dockWidgetItemDrawableConfigurationManager, SIGNAL(visibilityChanged(bool)), actionShowItemDrawableConfigurationManagerView, SLOT(setDisabled(bool)));
    connect(ui->dockWidgetGraphicsViewSynchronizedGroup, SIGNAL(visibilityChanged(bool)), actionShowGraphicsSyncManagerView, SLOT(setDisabled(bool)));
    connect(ui->dockWidgetActionsManager, SIGNAL(visibilityChanged(bool)), actionShowActionsManagerView, SLOT(setDisabled(bool)));
    connect(ui->dockWidgetLog, SIGNAL(visibilityChanged(bool)), actionShowLog, SLOT(setDisabled(bool)));

    fastForwardSpinBox->setValue(_stepManagerView->getStepManager()->getOptions().getFastJumpValueInDebugMode());
}

void GMainWindow::loadPlugins(bool showMessageIfNoPluginsFounded)
{
    CDM_PluginManager *pluginManager = getPluginManager();

    bool reload = true;
    bool first = true;

    while(reload)
    {
        pluginManager->load();

        if(showMessageIfNoPluginsFounded)
        {
            if((reload = (!pluginManager->isAPluginLoaded())) == true)
            {
                QMessageBox msgBox(this);
                msgBox.setText(tr("Attention aucun plugin n'a été trouvé dans :\n%1").arg(pluginManager->getSearchPath()));
                msgBox.setInformativeText(tr("Voulez-vous spécifier dans quel dossier rechercher les plugins ?"));
                msgBox.setDetailedText(QString("%1").arg(pluginManager->getError()));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.addButton(tr("Parcourir..."), QMessageBox::AcceptRole);
                QPushButton *rejectButton = msgBox.addButton(tr("Annuler"), QMessageBox::RejectRole);

                msgBox.exec();

                if(msgBox.clickedButton() == rejectButton)
                {
                    reload = false;
                }
                else
                {
                    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Dossier contenant les plugins..."), pluginManager->getSearchPath());

                    if(dirPath.isNull() == false)
                    {
                        pluginManager->setSearchPath(dirPath);
                    }
                    else
                    {
                        reload = false;
                    }
                }

                first = false;
            }
            else if(!pluginManager->getError().isEmpty())
            {
                QMessageBox msgBox(this);

                msgBox.setText(tr("Attention il y a eu des erreurs lors du chargement des plugins du dossier :\n%1").arg(pluginManager->getSearchPath()));
                msgBox.setDetailedText(QString("%1").arg(pluginManager->getError()));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
            }
            else if(!first)
            {
                QMessageBox::information(this, tr("Succès"), tr("Le plugin %1 a été chargé avec succès !").arg(pluginManager->getPluginName()));
            }
        }
        else
        {
            reload = false;
        }
    }
}

void GMainWindow::loadConfiguration()
{
    CONFIG_FILE->beginGroup("MainWindow");

    _defaultOpenDirPath = CONFIG_FILE->value("defaultOpenDirPath", "").toString();
    _defaultSaveDirPath = CONFIG_FILE->value("defaultSaveDirPath", "").toString();
    restoreState(CONFIG_FILE->value("windowState").toByteArray());

    CONFIG_FILE->beginGroup("Document");
    bool ok;
    int nDoc = CONFIG_FILE->value("nDocument", 0).toInt(&ok);
    CONFIG_FILE->endGroup();


    CONFIG_FILE->endGroup();

    if(ok)
    {
        for(int i=0; i<nDoc; ++i)
        {
            CONFIG_FILE->beginGroup("MainWindow");
            CONFIG_FILE->beginGroup("Document");
            CONFIG_FILE->beginGroup(QString("Doc%1").arg(i));

            QString type =CONFIG_FILE->value("Type", "").toString();
            QByteArray geometry = CONFIG_FILE->value("Geometry", "").toByteArray();

            CONFIG_FILE->endGroup();
            CONFIG_FILE->endGroup();
            CONFIG_FILE->endGroup();

            if(type == "2D")
                new2DDocument();
            else if(type == "3D")
                newDocument();
            else if(type == "TAB")
                newItemModelDocument();
            else
                ok = false;

            DM_DocumentView *view = getDocumentManagerView()->getDocumentView(i);

            if(view != NULL)
                view->restoreGeometry(geometry);
        }
    }

    loadPlugins();

    if(getDocumentManagerView()->nbDocumentView() == 0)
        newDocument();
}

void GMainWindow::writeConfiguration()
{
    CONFIG_FILE->beginGroup("MainWindow");

    CONFIG_FILE->setValue("defaultOpenDirPath", _defaultOpenDirPath);
    CONFIG_FILE->setValue("defaultSaveDirPath", _defaultSaveDirPath);
    CONFIG_FILE->setValue("windowState", saveState());

    CONFIG_FILE->beginGroup("Document");
    CONFIG_FILE->setValue("nDocument", getDocumentManagerView()->nbDocumentView());

    for(int i=0; i<getDocumentManagerView()->nbDocumentView(); ++i)
    {
        DM_DocumentView *docV = getDocumentManagerView()->getDocumentView(i);
        QString type = "unknown";

        CONFIG_FILE->beginGroup(QString("Doc%1").arg(i));

        if(dynamic_cast<GDocumentViewForGraphics*>(docV) != NULL)
        {
            if(!((GDocumentViewForGraphics*)docV)->getGraphicsList().isEmpty())
            {
                if(((G3DGraphicsView*)((GDocumentViewForGraphics*)docV)->getGraphicsList().first())->is2DViewActived())
                    type = "2D";
                else
                    type = "3D";
            }
        }
        else if(dynamic_cast<GDocumentViewForItemModel*>(docV) != NULL)
        {
            type = "TAB";
        }

        CONFIG_FILE->setValue("Type", type);
        CONFIG_FILE->setValue("Geometry", docV->saveGeometry());
        CONFIG_FILE->endGroup();
    }

    CONFIG_FILE->endGroup();

    CONFIG_FILE->endGroup();
}

QString GMainWindow::createFileExtensionAvailable()
{
    CDM_PluginManager *pluginManager = getStepManager()->getScriptManager()->getPluginManager();

    QString fileExtension;

    fileExtension += tr("All Valid Files (");

    // VALID FILES

    QList<QString> listScript = getScriptManager()->getFileExtensionAccepted();
    QListIterator<QString> itScript(listScript);

    if(itScript.hasNext())
    {
        fileExtension += "*" + itScript.next();

        while(itScript.hasNext())
        {
            fileExtension += " *" + itScript.next();
        }
    }

    if(pluginManager->isAPluginLoaded())
    {
        int count = pluginManager->countPluginLoaded();

        for(int i=0; i<count; ++i)
        {
            QList<CT_StepLoadFileSeparator*> listSeparator = pluginManager->getPlugin(i)->getOpenFileStepAvailable();

            QListIterator<CT_StepLoadFileSeparator*> it(listSeparator);

            while(it.hasNext())
            {
                QList<CT_AbstractStepLoadFile *> listStep = it.next()->getStepList();
                QListIterator<CT_AbstractStepLoadFile*> itStep(listStep);

                while(itStep.hasNext())
                {
                    CT_AbstractStepLoadFile *step = itStep.next();

                    QList<QString> ext = step->getFileExtensionAccepted();
                    QListIterator<QString> itExt(ext);

                    while(itExt.hasNext())
                    {
                        fileExtension += " *" + itExt.next();
                    }
                }
            }
        }
    }

    fileExtension += ");;";

    // FILE BY TYPE

    fileExtension = createScriptManagerExtension(fileExtension);

    if(pluginManager->isAPluginLoaded())
    {
        int count = pluginManager->countPluginLoaded();

        for(int i=0; i<count; ++i)
        {
            QList<CT_StepLoadFileSeparator*> listSeparator = pluginManager->getPlugin(i)->getOpenFileStepAvailable();

            QListIterator<CT_StepLoadFileSeparator*> it(listSeparator);

            while(it.hasNext())
            {
                CT_StepLoadFileSeparator *sep = it.next();

                fileExtension += sep->typeOfFile() + " (";

                QList<CT_AbstractStepLoadFile *> listStep = sep->getStepList();
                QListIterator<CT_AbstractStepLoadFile*> itStep(listStep);

                while(itStep.hasNext())
                {
                    CT_AbstractStepLoadFile *step = itStep.next();

                    QList<QString> ext = step->getFileExtensionAccepted();
                    QListIterator<QString> itExt(ext);

                    while(itExt.hasNext())
                    {
                        fileExtension += " *" + itExt.next();
                    }
                }

                fileExtension += ");;";
            }
        }
    }

    fileExtension += tr("All files") + " (*.*)";

    return fileExtension;
}

QString GMainWindow::createScriptManagerExtension(QString fileExtension)
{
    QList<QString> listScript = getScriptManager()->getFileExtensionAccepted();
    QListIterator<QString> itScript(listScript);

    if(itScript.hasNext())
    {
        fileExtension += tr("Script File (*") + itScript.next();

        while(itScript.hasNext())
        {
            fileExtension += " *" + itScript.next();
        }

        fileExtension += ");;";
    }

    return fileExtension;
}

void GMainWindow::changeLanguage()
{
    int index = ((QAction*)sender())->data().toInt();

    if(GUI_MANAGER->getLanguageManager()->currentLanguage() != index)
    {
        QMessageBox::information(this, tr("Information"), tr("Voud devez redémarrer l'application pour prendre en compte le changement de langue."));

        GUI_MANAGER->getLanguageManager()->useLanguage(index);
    }
}

void GMainWindow::documentToBeClosed(DM_DocumentView *view)
{
    if(_itemDrawableConfigurationView->getDocumentView() == view)
    {
        _itemDrawableConfigurationView->setDocument(NULL);
    }
}

void GMainWindow::stepAdded(CT_VirtualAbstractStep *step)
{
    Q_UNUSED(step)

    actionSaveScript->setEnabled(getStepManager()->getStepRootList().size() > 0);
}

void GMainWindow::stepToBeRemoved(CT_VirtualAbstractStep *step)
{
    QList<CT_VirtualAbstractStep*> list = getStepManager()->getStepRootList();

    int n = list.size();

    if(list.contains(step))
    {
        --n;
    }

    actionSaveScript->setEnabled(n > 0);
}

void GMainWindow::menuNewStepCanBeAddedFirstAboutToShow()
{
    CDM_PluginManager *pluginManager = getStepManager()->getScriptManager()->getPluginManager();

    menuNewStepCanBeAddedFirst->clear();

    if(pluginManager->isAPluginLoaded())
    {
        int n = pluginManager->countPluginLoaded();

        for(int i=0; i<n; ++i)
        {
            QString pluginName = pluginManager->getPluginName(i);
            if (pluginName.left(5) == "plug_")
            {
                pluginName.remove(0, 5);
            }

            MyQMenu *menuStep = new MyQMenu(pluginName, this);
            menuStep->setIcon(QIcon(":/Icones/Icones/add.png"));

            QList<CT_StepCanBeAddedFirstSeparator*> stepAvailable = pluginManager->getPlugin(i)->getCanBeAddedFirstStepAvailable();
            QListIterator<CT_StepCanBeAddedFirstSeparator*> it(stepAvailable);

            while(it.hasNext())
            {
                CT_StepCanBeAddedFirstSeparator *sep = it.next();
                QList<CT_AbstractStepCanBeAddedFirst*> stepList = sep->getStepList();
                QListIterator<CT_AbstractStepCanBeAddedFirst*> itStep(stepList);

                if(itStep.hasNext())
                {
                    MyQMenu *underMenu = NULL;

                    if(!sep->getTitle().isEmpty())
                        underMenu = new MyQMenu(sep->getTitle(), this);

                    while(itStep.hasNext())
                    {
                        CT_AbstractStepCanBeAddedFirst *step = itStep.next();

                        MyQAction *action = new MyQAction(step, tr("%1").arg(step->getStepDescription()), this);
                        action->setToolTip(tr("%1 (F1 pour plus d'info)").arg(step->getStepName()));
                        action->setIcon(QIcon(":/Icones/Icones/add.png"));

                        if (step->isManual())
                        {
                            QFont font;
                            font.setItalic(true);
                            action->setFont(font);
                        }

                        if(underMenu == NULL)
                            menuStep->addAction(action);
                        else
                            underMenu->addAction(action);

                        connect(action, SIGNAL(triggered()), this, SLOT(addCanBeAddedFirstStepFromMyQAction()));
                        connect(action, SIGNAL(hovered()), this, SLOT(actionHovered()));
                    }

                    if(underMenu != NULL)
                        menuStep->addMenu(underMenu);

                    menuStep->addSeparator();
                }
            }

            if(menuStep->actions().isEmpty())
            {
                MyQAction *action = new MyQAction(NULL, tr("Aucune action"), this);
                action->setEnabled(false);

                menuStep->addAction(action);
            }

            menuNewStepCanBeAddedFirst->addMenu(menuStep);
        }
    }
}

void GMainWindow::addCanBeAddedFirstStepFromMyQAction()
{
    MyQAction *action = (MyQAction*)sender();

    if(action->step() != NULL)
    {
        _stepManagerView->addCanBeAddedFirstStepAndConfigure(dynamic_cast<CT_AbstractStepCanBeAddedFirst*>(action->step()));
    }
}

void GMainWindow::actionHovered()
{
    QAction *action = (QAction*)sender();

    QPoint p = QCursor::pos();
    QString s = action->toolTip();

    p.setY(p.y() + 10);

    QToolTip::showText(p, s);
}
