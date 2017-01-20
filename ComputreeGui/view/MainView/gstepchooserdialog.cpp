#include "gstepchooserdialog.h"
#include "ui_gstepchooserdialog.h"

#include "dm_guimanager.h"
#include "view/MainView/gfavoritesmenudialog.h"
#include "view/StepResultTreeView/gstepmanager.h"
#include "view/MainView/gaboutstepdialog.h"

#include <QFileDialog>
#include <QMessageBox>

GStepChooserDialog::GStepChooserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GStepChooserDialog)
{
    ui->setupUi(this);
}

GStepChooserDialog::~GStepChooserDialog()
{
    delete ui;
}

void GStepChooserDialog::setStepManager(GStepManager *stepManager)
{
    m_stepManager = stepManager;
}

void GStepChooserDialog::init()
{
    QMenu *contextMenuSteps = new QMenu(this);
    m_actionAddStepToFavorites = contextMenuSteps->addAction(QIcon(":/Icones/Icones/favorite.png"), tr("Ajouter aux favoris"), this, SLOT(editFavorites()));
    m_actionRemoveStepFromFavorites = contextMenuSteps->addAction(QIcon(":/Icones/Icones/delete.png"), tr("Supprimer des favoris"), this, SLOT(removeStepFromFavoritesMenu()));
    m_actionRaiseStep = contextMenuSteps->addAction(QIcon(":/Icones/Icones/upper.png"), tr("Monter l'étape dans le menu"), this, SLOT(raiseStepInFavoritesMenu()));
    m_actionLowerStep = contextMenuSteps->addAction(QIcon(":/Icones/Icones/lower.png"), tr("Descendre l'étape dans le menu"), this, SLOT(lowerStepInFavoritesMenu()));
    QAction *actionInfoStep = contextMenuSteps->addAction(QIcon(":/Icones/Icones/info.png"), tr("Documentation de l'étape"), this, SLOT(showSelectedStepInformation()));
    actionInfoStep->setShortcut(tr("F1"));

    m_contextMenuRootLevel = new QMenu(this);
    m_contextMenuRootLevel->addAction(QIcon(":/Icones/Icones/favorite.png"), tr("Editer"), this, SLOT(editFavorites()));
    m_contextMenuRootLevel->addAction(QIcon(":/Icones/Icones/download.png"), tr("Charger"), this, SLOT(loadFavorites()));
    m_contextMenuRootLevel->addAction(QIcon(":/Icones/Icones/upload.png"), tr("Exporter"), this, SLOT(exportFavorites()));

    ui->stepChooserWidget->init(*GUI_MANAGER->getPluginManager());
    ui->stepChooserWidget->proxy()->setPluginsFilterEnabled(false);  // don't filters steps that was not in the plugin list
    ui->stepChooserWidget->proxy()->setShowStepNotCompatible(true); // don't show step not compatible with the parent step
    ui->stepChooserWidget->setDisplayConfiguration(GStepViewDefault::DNC_StepShortDescription);
    ui->stepChooserWidget->setContextMenuOnType(contextMenuSteps, DM_StepsFromPluginsModelConstructor::IT_Step);
    ui->stepChooserWidget->setContextMenuOnType(m_contextMenuRootLevel, DM_StepsFromPluginsModelConstructor::IT_RootLevel);

    connect(GUI_MANAGER->getPluginManager(), SIGNAL(beginLoading()), ui->stepChooserWidget, SLOT(reconstruct()), Qt::DirectConnection);
    connect(GUI_MANAGER->getPluginManager(), SIGNAL(finishLoading()), ui->stepChooserWidget, SLOT(reconstruct()), Qt::QueuedConnection);
    connect(m_stepManager, SIGNAL(stepSelected(CT_VirtualAbstractStep*)), ui->stepChooserWidget->proxy(), SLOT(setParentStep(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(ui->stepChooserWidget, SIGNAL(levelSelected(CT_MenuLevel*,int)), this, SLOT(refreshContextMenuOfLevel(CT_MenuLevel*,int)), Qt::DirectConnection);
    connect(ui->stepChooserWidget, SIGNAL(stepSelected(CT_VirtualAbstractStep*)), this, SLOT(refreshContextMenuOfStep(CT_VirtualAbstractStep*)), Qt::DirectConnection);
    connect(ui->stepChooserWidget, SIGNAL(stepDoubleClicked(CT_VirtualAbstractStep*)), m_stepManager, SLOT(addStepToSelectedStepOrToRootAndConfigure(CT_VirtualAbstractStep*)));
    connect(ui->stepChooserWidget, SIGNAL(replaceToDefault()), this, SIGNAL(replaceToDefault()));

    setWindowFlags(Qt::Dialog
                   | Qt::WindowTitleHint
                   | Qt::CustomizeWindowHint
                   | Qt::WindowCloseButtonHint);

    QAction *closeAction = new QAction(this);
    closeAction->setShortcut(Qt::Key_F2);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));
    this->addAction(closeAction);
}

GStepViewDefault *GStepChooserDialog::stepsChooserWidget() const
{
    return ui->stepChooserWidget;
}

CT_MenuLevel* GStepChooserDialog::existInFavorites(CT_VirtualAbstractStep *step)
{
    CT_MenuLevel *favorite = GUI_MANAGER->getPluginManager()->stepsMenu()->levelFromOperation(CT_StepsMenu::LO_Favorites);

    if(favorite != NULL) {
        return existInLevelRecursively(favorite, step);
    }

    return NULL;
}

CT_MenuLevel *GStepChooserDialog::existInLevelRecursively(CT_MenuLevel *level, CT_VirtualAbstractStep *step)
{
    QList<CT_MenuLevel*> levels = level->levels();
    QListIterator<CT_MenuLevel*> itL(levels);

    while(itL.hasNext()) {
        CT_MenuLevel* lev = itL.next();

        CT_MenuLevel* levelWithStep = existInLevelRecursively(lev, step);
        if(levelWithStep != NULL)
        {
            return levelWithStep;
        }
    }


    QList<CT_VirtualAbstractStep*> steps = level->steps();
    QListIterator<CT_VirtualAbstractStep*> itS(steps);

    while(itS.hasNext()) {
        CT_VirtualAbstractStep *nStep = itS.next();

        if(nStep == step)
        {
            return level;
        }
    }

    return NULL;
}

bool GStepChooserDialog::event(QEvent *e)
{
    if(e->type() == QEvent::WindowStateChange) {

        if(isMinimized())
            emit maximizedChanged(false);
        else
            emit maximizedChanged(true);
    }

    return QDialog::event(e);
}

void GStepChooserDialog::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);

    emit visibilityChanged(true);
}

void GStepChooserDialog::hideEvent(QHideEvent *e)
{
    QDialog::hideEvent(e);

    emit visibilityChanged(false);
}

void GStepChooserDialog::closeEvent(QCloseEvent *e)
{
    e->ignore();

    hide();
}

void GStepChooserDialog::editFavorites()
{
    GFavoritesMenuDialog dialog;
    dialog.setStepToAdd(ui->stepChooserWidget->currentStepSelected(), stepsChooserWidget()->displayConfiguration());
    dialog.init();
    if(dialog.exec() == QDialog::Accepted) {
        ui->stepChooserWidget->reconstruct();
    }
}

void GStepChooserDialog::loadFavorites()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier favoris"), "", tr("Fichier favoris (*.%1)").arg(GUI_MANAGER->getPluginManager()->stepsMenuManager()->favoriteDefaultFileExtension()));

    if(!filepath.isEmpty()) {
        GUI_MANAGER->getPluginManager()->stepsMenuManager()->loadFavoritesFrom(filepath);
        ui->stepChooserWidget->reconstruct();
    }
}

void GStepChooserDialog::exportFavorites()
{
    QString filepath = QFileDialog::getSaveFileName(this, tr("Sauvegarder les favoris"), "", tr("Fichier favoris (*.%1)").arg(GUI_MANAGER->getPluginManager()->stepsMenuManager()->favoriteDefaultFileExtension()));

    if(!filepath.isEmpty())
        GUI_MANAGER->getPluginManager()->stepsMenuManager()->saveFavoritesTo(filepath);
}

void GStepChooserDialog::removeStepFromFavoritesMenu()
{
    delete ui->stepChooserWidget->currentStepSelected();
    ui->stepChooserWidget->reconstruct();
}

void GStepChooserDialog::raiseStepInFavoritesMenu()
{
    CT_VirtualAbstractStep* step = ui->stepChooserWidget->currentStepSelected();
    CT_MenuLevel* level = existInFavorites(step);
    if (level != NULL)
    {
        level->raiseStepInMenu(step);
        ui->stepChooserWidget->reconstruct();
        ui->stepChooserWidget->searchOriginalStepAndExpandParent(step);
    }
}

void GStepChooserDialog::lowerStepInFavoritesMenu()
{
    CT_VirtualAbstractStep* step = ui->stepChooserWidget->currentStepSelected();
    CT_MenuLevel* level = existInFavorites(step);
    if (level != NULL)
    {
        level->lowerStepInMenu(step);
        ui->stepChooserWidget->reconstruct();
        ui->stepChooserWidget->searchOriginalStepAndExpandParent(step);
    }
}

void GStepChooserDialog::showSelectedStepInformation()
{
    CT_VirtualAbstractStep *step = ui->stepChooserWidget->currentStepSelected();

    if(step != NULL) {
        GAboutStepDialog dialog(step);
        dialog.exec();
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'afficher les informations de cette étape. L'étape n'a pas été trouvée dans le plugin ou le plugin n'est pas présent."));
    }
}

void GStepChooserDialog::refreshContextMenuOfStep(CT_VirtualAbstractStep *step)
{
    m_actionAddStepToFavorites->setEnabled(false);
    m_actionRemoveStepFromFavorites->setEnabled(false);
    m_actionRaiseStep->setEnabled(false);
    m_actionLowerStep->setEnabled(false);

    if(step != NULL) {
        bool exist = (existInFavorites(step) != NULL);

        m_actionAddStepToFavorites->setEnabled(!exist);
        m_actionRemoveStepFromFavorites->setEnabled(exist);
        m_actionRaiseStep->setEnabled(exist);
        m_actionLowerStep->setEnabled(exist);
    }
}

void GStepChooserDialog::refreshContextMenuOfLevel(CT_MenuLevel *level, int typeOfLevel)
{
    if(typeOfLevel == DM_StepsFromPluginsModelConstructor::IT_RootLevel)
    {
        CT_MenuLevel *favorites = GUI_MANAGER->getPluginManager()->stepsMenu()->levelFromOperation(CT_StepsMenu::LO_Favorites);
        m_contextMenuRootLevel->setEnabled(level == favorites);
    }
}

void GStepChooserDialog::toggleVisibility()
{
    if(isHidden())
        showNormal();
    else
        hide();
}
