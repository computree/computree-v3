#include "gfavoritesmenudialog.h"
#include "ui_gfavoritesmenudialog.h"

#include "dm_guimanager.h"
#include "ct_step/tools/menu/ct_menulevel.h"

#include <QInputDialog>

GFavoritesMenuDialog::GFavoritesMenuDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GFavoritesMenuDialog)
{
    ui->setupUi(this);
    m_step = NULL;
}

GFavoritesMenuDialog::~GFavoritesMenuDialog()
{
    delete ui;
}

void GFavoritesMenuDialog::init()
{
    ui->comboBoxLevels->clear();

    CT_MenuLevel *favorites = GUI_MANAGER->getPluginManager()->stepsMenu()->getOrCreateRootLevel(CT_StepsMenu::LO_Favorites);

    QList<CT_MenuLevel*> levels = favorites->levels();
    QListIterator<CT_MenuLevel*> it(levels);

    while(it.hasNext()) {
        CT_MenuLevel *level = it.next();
        ui->comboBoxLevels->addItem(level->displayableName());
    }
}

void GFavoritesMenuDialog::setStepToAdd(CT_VirtualAbstractStep *step)
{
    m_step = step;
}

void GFavoritesMenuDialog::accept()
{
    CT_MenuLevel *favorites = GUI_MANAGER->getPluginManager()->stepsMenu()->getOrCreateRootLevel(CT_StepsMenu::LO_Favorites);

    int s = ui->comboBoxLevels->count();

    for(int i=0; i<s; ++i)
        CT_MenuLevel::getOrCreateLevel(ui->comboBoxLevels->itemText(i), favorites);

    if((m_step != NULL) && (ui->comboBoxLevels->currentIndex() >= 0)) {
        CT_MenuLevel *levelChoosed = CT_MenuLevel::getOrCreateLevel(ui->comboBoxLevels->currentText(), GUI_MANAGER->getPluginManager()->stepsMenu()->getOrCreateRootLevel(CT_StepsMenu::LO_Favorites));
        levelChoosed->addStep(m_step->getPlugin()->createNewInstanceOfStep(*m_step, NULL));
    }

    QDialog::accept();
}

void GFavoritesMenuDialog::on_pushButtonAddLevel_clicked()
{
    QString text = QInputDialog::getText(this, tr("Ajouter un niveau"), tr("Nom du niveau"));

    if(!text.isEmpty() && (ui->comboBoxLevels->findText(text, Qt::MatchFixedString) == -1)) {
        ui->comboBoxLevels->addItem(text);
        ui->comboBoxLevels->setCurrentIndex(ui->comboBoxLevels->count()-1);
    }
}
