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


#include "gpointofviewdocumentmanager.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

GPointOfViewDocumentManager::GPointOfViewDocumentManager(DM_PointOfViewManager &pofManager,
                                                         QString key,
                                                         QWidget *parent) :
    QMenu(parent)
{
    _key = key;
    _pofManager = &pofManager;

    connect(this, SIGNAL(aboutToShow()), this, SLOT(reload()), Qt::DirectConnection);
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(actionTriggered(QAction*)), Qt::DirectConnection);
}

void GPointOfViewDocumentManager::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier contenant des point de vue..."), _pofManager->defaultFilePath(), createFileExtensionAvailable());

    if(!filename.isEmpty())
    {
        _pofManager->loadFromFile(filename);
        reload();
    }
}

void GPointOfViewDocumentManager::saveToFile()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Enregistrer sous..."), _pofManager->defaultFilePath(), createFileExtensionAvailable());

    if(!filename.isEmpty())
    {
        QFileInfo info(filename);

        if(info.suffix().isEmpty())
        {
            filename += _pofManager->extensionOfFile().first();
        }

        _pofManager->saveToFile(filename);
    }
}

QString GPointOfViewDocumentManager::createFileExtensionAvailable()
{
    QString fileExtension;

    fileExtension += tr("Point of view Files (");

    QList<QString> listExt = _pofManager->extensionOfFile();
    QListIterator<QString> itExt(listExt);

    if(itExt.hasNext())
    {
        fileExtension += "*" + itExt.next();

        while(itExt.hasNext())
        {
            fileExtension += " *" + itExt.next();
        }
    }

    return fileExtension + ")";
}

void GPointOfViewDocumentManager::createNew()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Nouveau fichier de point de vue"), QFileInfo(_pofManager->defaultFilePath()).dir().path(), createFileExtensionAvailable());

    if(!filename.isEmpty())
    {
        _pofManager->createNewFile(filename);
    }
}

void GPointOfViewDocumentManager::reload()
{
    clear();

    QAction *actionTitle = new QAction(this);
    actionTitle->setEnabled(false);
    actionTitle->setText(tr("DEFAULT"));

    QFileInfo info(_pofManager->defaultFilePath());
    QString baseName = info.completeBaseName();

    if(!baseName.isEmpty())
    {
        actionTitle->setText(baseName);
    }

    addAction(actionTitle);

    addSeparator();

    QList<DM_PointOfView> list = _pofManager->getPointOfViewList(_key);
    QListIterator<DM_PointOfView> it(list);

    while(it.hasNext())
    {
        DM_PointOfView pof = it.next();

        QAction *action = new QAction(pof.toString(), this);
        action->setData(pof.toSaveString());

        addAction(action);
    }

    addSeparator();

    QAction *actionSavePointOfView = new QAction(tr("Ajouter"), this);
    addAction(actionSavePointOfView);

    addSeparator();

    QAction *actionCreateNewPointOfView = new QAction(tr("Nouveau"), this);
    addAction(actionCreateNewPointOfView);

    QAction *actionRemoveAllPointOfView = new QAction(tr("Tout supprimer"), this);
    addAction(actionRemoveAllPointOfView);

    addSeparator();

    QAction *actionOpenFile = new QAction(tr("Ouvrir le fichier..."), this);
    addAction(actionOpenFile);
    QAction *actionSaveFile = new QAction(tr("Enregistrer sous..."), this);
    addAction(actionSaveFile);

    connect(actionSavePointOfView, SIGNAL(triggered()), this, SIGNAL(addActualPointOfView()));
    connect(actionRemoveAllPointOfView, SIGNAL(triggered()), this, SLOT(removeAllPointOfView()));
    connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(actionSaveFile, SIGNAL(triggered()), this, SLOT(saveToFile()));
    connect(actionCreateNewPointOfView, SIGNAL(triggered()), this, SLOT(createNew()));
}

void GPointOfViewDocumentManager::actionTriggered(QAction *action)
{
    bool ok;

    _pofToSet = DM_PointOfView::loadFromString(action->data().toString(), ok);

    if(ok)
    {
        emit setPointOfView(&_pofToSet);
    }
}

void GPointOfViewDocumentManager::removeAllPointOfView()
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("Attention"));
    msgBox.setText(tr("Vous allez supprimer tous les points de vue du fichier actuel."));
    msgBox.setInformativeText(tr("Voulez vous continuer et supprimer les points de vue ou crer un nouveau fichier ?"));
    QAbstractButton *newButton = msgBox.addButton(tr("Nouveau"), QMessageBox::AcceptRole);
    QAbstractButton *deleteButton = msgBox.addButton(tr("Supprimer"), QMessageBox::ActionRole);
    msgBox.addButton(tr("Annuler"), QMessageBox::RejectRole);

    msgBox.exec();

    if(msgBox.clickedButton() == deleteButton)
    {
        _pofManager->removeAll(_key);
    }
    else if(msgBox.clickedButton() == newButton)
    {
        createNew();
    }
}
