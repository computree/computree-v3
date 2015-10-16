#include "gstepchooserdialog.h"
#include "ui_gstepchooserdialog.h"

#include "dm_guimanager.h"
#include "view/MainView/gfavoritesmenudialog.h"
#include "view/StepResultTreeView/gstepmanager.h"
#include "view/MainView/gaboutstepdialog.h"

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
    m_actionAddStepToFavorites = contextMenuSteps->addAction(QIcon(":/Icones/Icones/favorite.png"), tr("Ajouter aux favoris"), this, SLOT(addStepFromStepChooserToFavoritesMenu()));
    m_actionRemoveStepFromFavorites = contextMenuSteps->addAction(QIcon(":/Icones/Icones/delete.png"), tr("Supprimer des favoris"), this, SLOT(removeStepFromFavoritesMenu()));
    QAction *actionInfoStep = contextMenuSteps->addAction(QIcon(":/Icones/Icones/info.png"), tr("Documentation de l'étape"), this, SLOT(showSelectedStepInformation()));
    actionInfoStep->setShortcut(tr("F1"));

    QMenu *contextMenuLevel = new QMenu(this);
    m_actionRemoveLevelFromFavorites = contextMenuLevel->addAction(QIcon(":/Icones/Icones/delete.png"), tr("Supprimer le niveau des favoris"), this, SLOT(removeLevelFromFavoritesMenu()));

    ui->stepChooserWidget->init(*GUI_MANAGER->getPluginManager());
    ui->stepChooserWidget->proxy()->setPluginsFilterEnabled(false);  // don't filters steps that was not in the plugin list
    ui->stepChooserWidget->proxy()->setShowStepNotCompatible(true); // don't show step not compatible with the parent step
    ui->stepChooserWidget->setDisplayConfiguration(GStepViewDefault::DNC_StepDisplayableName);
    ui->stepChooserWidget->setContextMenuOnType(contextMenuSteps, DM_StepsFromPluginsModelConstructor::IT_Step);
    ui->stepChooserWidget->setContextMenuOnType(contextMenuLevel, DM_StepsFromPluginsModelConstructor::IT_SubLevel);

    connect(GUI_MANAGER->getPluginManager(), SIGNAL(beginLoading()), ui->stepChooserWidget, SLOT(reconstruct()), Qt::DirectConnection);
    connect(GUI_MANAGER->getPluginManager(), SIGNAL(finishLoading()), ui->stepChooserWidget, SLOT(reconstruct()), Qt::QueuedConnection);
    connect(m_stepManager, SIGNAL(stepSelected(CT_VirtualAbstractStep*)), ui->stepChooserWidget->proxy(), SLOT(setParentStep(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(ui->stepChooserWidget, SIGNAL(levelSelected(CT_MenuLevel*)), this, SLOT(refreshContextMenuOfLevel(CT_MenuLevel*)), Qt::DirectConnection);
    connect(ui->stepChooserWidget, SIGNAL(stepSelected(CT_VirtualAbstractStep*)), this, SLOT(refreshContextMenuOfStep(CT_VirtualAbstractStep*)), Qt::DirectConnection);
    connect(ui->stepChooserWidget, SIGNAL(stepDoubleClicked(CT_VirtualAbstractStep*)), m_stepManager, SLOT(addStepToSelectedStepOrToRootAndConfigure(CT_VirtualAbstractStep*)));
}

bool GStepChooserDialog::existInFavorites(CT_VirtualAbstractStep *step)
{
    CT_MenuLevel *favorite = GUI_MANAGER->getPluginManager()->stepsMenu()->levelFromOperation(CT_StepsMenu::LO_Favorites);

    if(favorite != NULL) {
        return existInLevelRecursively(favorite, step);
    }

    return false;
}

bool GStepChooserDialog::existInLevelRecursively(CT_MenuLevel *level, CT_VirtualAbstractStep *step)
{
    QList<CT_VirtualAbstractStep*> steps = level->steps();
    QListIterator<CT_VirtualAbstractStep*> itS(steps);

    while(itS.hasNext()) {
        CT_VirtualAbstractStep *nStep = itS.next();

        if(nStep == step)
            return true;
    }

    QList<CT_MenuLevel*> levels = level->levels();
    QListIterator<CT_MenuLevel*> itL(levels);

    while(itL.hasNext()) {
        if(existInLevelRecursively(itL.next(), step))
            return true;
    }

    return false;
}

void GStepChooserDialog::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);

    emit visibilityChanged(true);
}

void GStepChooserDialog::closeEvent(QCloseEvent *e)
{
    e->ignore();

    hide();

    emit visibilityChanged(false);
}

void GStepChooserDialog::addStepFromStepChooserToFavoritesMenu()
{
    GFavoritesMenuDialog dialog;
    dialog.setStepToAdd(ui->stepChooserWidget->currentStepSelected());
    dialog.init();
    if(dialog.exec() == QDialog::Accepted) {
        ui->stepChooserWidget->reconstruct();
    }
}

void GStepChooserDialog::removeStepFromFavoritesMenu()
{
    delete ui->stepChooserWidget->currentStepSelected();
    ui->stepChooserWidget->reconstruct();
}

void GStepChooserDialog::showSelectedStepInformation()
{
    GAboutStepDialog dialog(ui->stepChooserWidget->currentStepSelected());
    dialog.exec();
}

void GStepChooserDialog::removeLevelFromFavoritesMenu()
{
    GUI_MANAGER->getPluginManager()->stepsMenu()->removeLevelFromFavorites(ui->stepChooserWidget->currentLevelSelected());
    ui->stepChooserWidget->reconstruct();
}

void GStepChooserDialog::refreshContextMenuOfStep(CT_VirtualAbstractStep *step)
{
    m_actionAddStepToFavorites->setEnabled(false);
    m_actionRemoveStepFromFavorites->setEnabled(false);

    if(step != NULL) {
        bool exist = existInFavorites(step);

        m_actionAddStepToFavorites->setEnabled(!exist);
        m_actionRemoveStepFromFavorites->setEnabled(exist);
    }
}

void GStepChooserDialog::refreshContextMenuOfLevel(CT_MenuLevel *level)
{
    m_actionRemoveLevelFromFavorites->setEnabled(false);

    if(level != NULL) {
        CT_MenuLevel *favorites = GUI_MANAGER->getPluginManager()->stepsMenu()->levelFromOperation(CT_StepsMenu::LO_Favorites);

        if(favorites != NULL) {
            CT_MenuLevel *sLevel = level;

            while(sLevel->parentLevel() != NULL) {

                if(sLevel->parentLevel() == favorites) {
                    m_actionRemoveLevelFromFavorites->setEnabled(true);
                    return;
                }

                sLevel = sLevel->parentLevel();
            }
        }
    }
}
