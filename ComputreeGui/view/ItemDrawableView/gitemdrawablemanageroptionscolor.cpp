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


#include "gitemdrawablemanageroptionscolor.h"
#include "ui_gitemdrawablemanageroptionscolor.h"

#include "cdm_configfile.h"

#include <QMessageBox>
#include <QFileDialog>

GItemDrawableManagerOptionsColor::GItemDrawableManagerOptionsColor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GItemDrawableManagerOptionsColor)
{
    ui->setupUi(this);

    _colorDialog = new QColorDialog(this);
    ui->listWidgetCouleur->setDragDropMode(QAbstractItemView::InternalMove);

    loadSettings();
}

GItemDrawableManagerOptionsColor::~GItemDrawableManagerOptionsColor()
{
    delete ui;

    saveSettings();
}

void GItemDrawableManagerOptionsColor::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GItemDrawableManagerOptionsColor::setOptions(DM_ItemDrawableManagerOptions option)
{
    _option = option;

    setColorList(option.getColorList());
}

QList<QColor> GItemDrawableManagerOptionsColor::getColorList()
{
    return createColorListFromListWidget();
}

void GItemDrawableManagerOptionsColor::setColorList(QList<QColor> colorList)
{
    ui->listWidgetCouleur->clear();

    QListIterator<QColor> it(colorList);

    while(it.hasNext())
    {
        ui->listWidgetCouleur->addItem(createItemForColor(it.next()));
    }
}

QListWidgetItem* GItemDrawableManagerOptionsColor::createItemForColor(QColor color)
{
    QListWidgetItem *item = new QListWidgetItem(ui->listWidgetCouleur);
    item->setBackgroundColor(color);

    return item;
}

QList<QColor> GItemDrawableManagerOptionsColor::createColorListFromListWidget()
{
    QList<QColor> list;

    int rowCount = ui->listWidgetCouleur->model()->rowCount(ui->listWidgetCouleur->rootIndex());

    for(int i=0; i<rowCount; ++i)
    {
        list.append(ui->listWidgetCouleur->item(i)->backgroundColor());
    }

    return list;
}

QString GItemDrawableManagerOptionsColor::createExtensionStringForColorFile()
{
    QList<QString> list = _option.getColorListFileExtension();
    QListIterator<QString> it(list);

    QString ext = "";

    if(it.hasNext())
    {
        ext = tr("Color File (*%1").arg(it.next());

        while(it.hasNext())
        {
            ext += " *" + it.next();
        }

        ext += ");;";
    }

    return ext;
}

QString GItemDrawableManagerOptionsColor::createFilePathWithExtension(QString filePath)
{
    QFileInfo info(filePath);

    QList<QString> list = _option.getColorListFileExtension();

    if(info.suffix().isEmpty()
            || !list.contains("."+info.suffix()))
    {
        if(!list.isEmpty())
        {
            return info.dir().path() + "/" + info.fileName() + list.first();
        }
    }

    return filePath;
}

void GItemDrawableManagerOptionsColor::loadSettings()
{
    _defaultOpenDir = ".";
    _defaultSaveDir = ".";

    CONFIG_FILE->beginGroup("GItemDrawableManagerOptionsColor");

    _defaultOpenDir = CONFIG_FILE->value("defaultOpenDir", _defaultOpenDir).toString();
    _defaultSaveDir = CONFIG_FILE->value("defaultSaveDir", _defaultSaveDir).toString();

    CONFIG_FILE->endGroup();
}

void GItemDrawableManagerOptionsColor::saveSettings()
{
    CONFIG_FILE->beginGroup("GItemDrawableManagerOptionsColor");

    CONFIG_FILE->setValue("defaultOpenDir", _defaultOpenDir);
    CONFIG_FILE->setValue("defaultSaveDir", _defaultSaveDir);

    CONFIG_FILE->endGroup();
}

////////////// PRIVATE SLOTS //////////////

void GItemDrawableManagerOptionsColor::on_pushButtonAjouter_clicked()
{
    if(_colorDialog->exec())
    {
        ui->listWidgetCouleur->addItem(createItemForColor(_colorDialog->selectedColor()));
    }
}

void GItemDrawableManagerOptionsColor::on_pushButtonSupprimer_clicked()
{
    QList<QListWidgetItem*> list = ui->listWidgetCouleur->selectedItems();
    QListIterator<QListWidgetItem*> it(list);

    while(it.hasNext())
    {
        QListWidgetItem *item = it.next();

        delete item;
    }
}

void GItemDrawableManagerOptionsColor::on_pushButtonOuvrir_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier de couleur"), _defaultOpenDir, createExtensionStringForColorFile());

    if(!filePath.isEmpty())
    {
        _defaultOpenDir = filePath;

        DM_ItemDrawableManagerOptions tempOption;

        if(tempOption.loadColorListFromFile(filePath))
        {
            setColorList(tempOption.getColorList());
        }
    }
}

void GItemDrawableManagerOptionsColor::on_pushButtonSauvegarder_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Sauvegarder un fichier de couleur"), _defaultSaveDir, createExtensionStringForColorFile());

    if(!filePath.isEmpty())
    {
        _defaultSaveDir = filePath;

        DM_ItemDrawableManagerOptions tempOption;
        tempOption.setColorList(createColorListFromListWidget());

        if(tempOption.saveColorListToFile(createFilePathWithExtension(filePath)))
        {
            QMessageBox::information(this, tr("Sauvegarde russi"), tr("La sauvegarde de la liste des couleurs a russi."));
        }
        else
        {
            QMessageBox::critical(this, tr("Erreur lors de la sauvegarde"), tr("La sauvegarde de la liste des couleurs a chou."));
        }
    }
}

void GItemDrawableManagerOptionsColor::on_pushButtonSauvegarderDefaut_clicked()
{
    DM_ItemDrawableManagerOptions tempOption;
    tempOption.setColorList(createColorListFromListWidget());

    if(tempOption.save())
    {
        QMessageBox::information(this, tr("Sauvegarde russi"), tr("La sauvegarde de la liste des couleurs par dfaut a russi."));
    }
    else
    {
        QMessageBox::critical(this, tr("Erreur lors de la sauvegarde"), tr("La sauvegarde de la liste des couleurs par defaut a chou."));
    }
}
