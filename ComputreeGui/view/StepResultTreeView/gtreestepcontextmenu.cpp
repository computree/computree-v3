/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

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


#include <QToolTip>
#include <QFile>

#include "cdm_scriptmanagerabstract.h"

#include "gtreestepcontextmenu.h"
#include "view/MainView/gaboutstepdialog.h"

GTreeStepContextMenu::GTreeStepContextMenu(CDM_StepManager &stepManager, QWidget *parent) : QMenu(parent)
{
    _selectedStep = NULL;
    _stepManager = &stepManager;
}

void GTreeStepContextMenu::setSelectedStep(Step *step)
{
    _selectedStep = step;

    reload();
}

/////////////////// PRIVATE ///////////////////

Step* GTreeStepContextMenu::selectedStep()
{
    return _selectedStep;
}

void GTreeStepContextMenu::addAllStepOnMenu(QList<StepSeparator*> stepAvailable, QMenu *menuStep, bool insert)
{
    QListIterator<StepSeparator*> it(stepAvailable);

    while(it.hasNext())
    {        
        StepSeparator *sep = it.next();
        QList<Step*> stepList = sep->getStepList();
        QListIterator<Step*> itStep(stepList);

        if(itStep.hasNext())
        {
            QMenu *underMenu = NULL;

            if(!sep->getTitle().isEmpty())
            {
                underMenu = new QMenu(sep->getTitle(), this);
                /*underMenu->setStyleSheet("QMenu::item[isManual=\"true\"]{ background-color:rgb(255,0,0); }"
                                        "QMenu::item:selected { background-color: #654321; }");*/
            }

            while(itStep.hasNext())
            {
                Step *step = itStep.next();

                MyQAction *action = new MyQAction(step, tr("%1").arg(step->getStepCustomName()), this);
                action->setToolTip(step->getStepDescription());
                action->setIcon(QIcon(":/Icones/Icones/add.png"));

                if(!insert)
                {
                    action->setEnabled((selectedStep() != NULL) ? step->acceptAddAfterThisStep(selectedStep()) : false);
                }
                else
                {
                    if(!_stepManager->isRunning()
                        && (selectedStep() != NULL)
                        && (step->acceptAddAfterThisStep(selectedStep())))
                    {
                        bool ok = true;

                        QList<Step*> childList = selectedStep()->getStepChildList();
                        QListIterator<Step*> itChild(childList);

                        while(ok
                              && itChild.hasNext())
                        {
                            ok = itChild.next()->acceptAddAfterThisStep(step);
                        }

                        action->setEnabled(ok);
                    }
                }

                connectAction(action, insert);

                if(underMenu == NULL)
                    menuStep->addAction(action);
                else
                    underMenu->addAction(action);
            }

            if(underMenu != NULL)
                menuStep->addMenu(underMenu);

            menuStep->addSeparator();
        }
    }
}

void GTreeStepContextMenu::addAllStepOnMenu(QList<StepCanBeAddedFirstSeparator *> stepAvailable, QMenu *menuStep)
{
    QListIterator<StepCanBeAddedFirstSeparator*> it(stepAvailable);

    while(it.hasNext())
    {
        StepCanBeAddedFirstSeparator *sep = it.next();
        QList<StepCanBeAddedFirst*> stepList = sep->getStepList();
        QListIterator<StepCanBeAddedFirst*> itStep(stepList);

        if(itStep.hasNext())
        {
            QMenu *underMenu = NULL;

            if(!sep->getTitle().isEmpty())
                underMenu = new QMenu(sep->getTitle(), this);

            while(itStep.hasNext())
            {
                StepCanBeAddedFirst *step = itStep.next();

                MyQAction *action = new MyQAction(step, tr("%1").arg(step->getStepCustomName()), this);
                action->setToolTip(step->getStepDescription());
                action->setIcon(QIcon(":/Icones/Icones/add.png"));
                action->setEnabled(true);

                connectAction(action, false);

                if(underMenu == NULL)
                    menuStep->addAction(action);
                else
                    underMenu->addAction(action);
            }

            if(underMenu != NULL)
                menuStep->addMenu(underMenu);

            menuStep->addSeparator();
        }
    }
}

void GTreeStepContextMenu::connectAction(QAction *action, bool insert)
{
    if(insert)
    {
        connect(action, SIGNAL(triggered()), this, SLOT(insertStepRequired()));
    }
    else
    {
        connect(action, SIGNAL(triggered()), this, SLOT(newStepRequired()));
    }

    connect(action, SIGNAL(hovered()), this, SLOT(actionHovered()));
}

/////////////////// SLOT ///////////////////

void GTreeStepContextMenu::reload()
{
    CDM_PluginManager *pluginManager = _stepManager->getScriptManager()->getPluginManager();

    // supprime toutes les actions
    clear();

    MyQAction *action = new MyQAction(selectedStep(), trUtf8("Ex\xc3\xa9""cuter"), this);
    action->setIcon(QIcon(":/Icones/Icones/play.png"));
    action->setEnabled((selectedStep() != NULL)
                       && ((!selectedStep()->needInputResults())
                           || ((selectedStep()->parentStep() != NULL)
                               && (selectedStep()->parentStep()->nResult() > 0))));
    connect(action, SIGNAL(triggered()), this, SLOT(executeStepRequired()));
    addAction(action);
    addSeparator();

    if(selectedStep()->isModifiable())
    {
        action = new MyQAction(selectedStep(), tr("Modifier (mode manuel)"), this);
        action->setIcon(QIcon(":/Icones/Icones/hand.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(executeModifyStepRequired()));
        addAction(action);
        addSeparator();
    }

    action = new MyQAction(selectedStep(), tr("Informations sur l'étape"), this);
    action->setIcon(QIcon(":/Icones/Icones/info.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(showStepInformations()));
    addAction(action);
    addSeparator();

    action = new MyQAction(selectedStep(), (selectedStep()->getStepChildList().isEmpty() && selectedStep()->hasChoiceBetweenMultipleInputResults()) ? trUtf8("Configurer les r\xc3\xa9""sultats d'entr\xc3\xa9""e") : trUtf8("Voir la configuration des r\xc3\xa9""sultats d'entr\xc3\xa9""e"), this);
    action->setIcon(QIcon(":/Icones/Icones/preferences-system.png"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(configureInputResultOfStepRequired()));
    addAction(action);

    action = new MyQAction(selectedStep(), tr("Configurer"), this);
    action->setIcon(QIcon(":/Icones/Icones/preferences-system.png"));
    action->setEnabled(selectedStep() != NULL);
    connect(action, SIGNAL(triggered()), this, SLOT(configureStepRequired()));
    addAction(action);

    addSeparator();

    action = new MyQAction(selectedStep(), tr("Supprimer"), this);
    action->setIcon(QIcon(":/Icones/Icones/delete.png"));
    action->setEnabled(selectedStep() != NULL);
    connect(action, SIGNAL(triggered()), this, SLOT(deleteStepRequired()));
    addAction(action);

    addSeparator();

    /*action = new MyQAction(selectedStep(), tr("Charger à partir du disque dur"), this);
    action->setIcon(QIcon(":/Icones/Icones/upload.png"));
    action->setEnabled(false);
    StepSerializable *selectedStepSerializable = dynamic_cast<StepSerializable*>(selectedStep());

    if(selectedStepSerializable != NULL)
    {
        CDM_ScriptStepObjectUserData *userData = dynamic_cast<CDM_ScriptStepObjectUserData*>(selectedStepSerializable->userData(0));

        action->setEnabled((userData != NULL)
                            && (selectedStepSerializable->canBeDeserialized(userData->getSerializedResultDirPath())));
    }

    connect(action, SIGNAL(triggered()), this, SLOT(loadResultRequired()));
    addAction(action);*/

    action = new MyQAction(selectedStep(), tr("Déplier"), this);
    action->setIcon(QIcon(":/Icones/Icones/expand.png"));
    connect(action, SIGNAL(triggered()), this, SIGNAL(expand()));
    addAction(action);

    action = new MyQAction(selectedStep(), tr("Déplier toutes les étapes"), this);
    action->setIcon(QIcon(":/Icones/Icones/expand.png"));
    connect(action, SIGNAL(triggered()), this, SIGNAL(expandAll()));
    addAction(action);

    action = new MyQAction(selectedStep(), tr("Replier"), this);
    action->setIcon(QIcon(":/Icones/Icones/collapse.png"));
    connect(action, SIGNAL(triggered()), this, SIGNAL(collapse()));
    addAction(action);

    action = new MyQAction(selectedStep(), tr("Replier toutes les étapes"), this);
    action->setIcon(QIcon(":/Icones/Icones/collapse.png"));
    connect(action, SIGNAL(triggered()), this, SIGNAL(collapseAll()));
    addAction(action);

    addSeparator();

    if(pluginManager->isAPluginLoaded())
    {
        int n = pluginManager->countPluginLoaded();

        for(int i=0; i<n; ++i)
        {
            QMenu *menuStep = new QMenu(pluginManager->getPluginName(i), this);
            menuStep->setIcon(QIcon(":/Icones/Icones/add.png"));
            /*menuStep->setStyleSheet("QMenu::item[isManual=\"true\"]{ background-color:rgb(255,0,0); }"
                                    "QMenu::action:selected { background-color: #654321; }");*/

            /*QMenu *insertStep = new QMenu(trUtf8("Ins\xc3\xa9rer"), this);
            insertStep->setIcon(QIcon(":/Icones/Icones/add.png"));*/

            QList<StepSeparator*> stepAvailable = pluginManager->getPlugin(i)->getGenericsStepAvailable();

            /*addAllStepOnMenu(stepAvailable, insertStep, true);

            if(!insertStep->actions().isEmpty())
            {
                menuStep->addMenu(insertStep);
                menuStep->addSeparator();
            }
            else
                delete insertStep;*/


            addAllStepOnMenu(stepAvailable, menuStep);

            QList<StepCanBeAddedFirstSeparator*> stepAddeFirstAvailable = pluginManager->getPlugin(i)->getCanBeAddedFirstStepAvailable();
            addAllStepOnMenu(stepAddeFirstAvailable, menuStep);

            if(menuStep->actions().isEmpty())
            {
                action = new MyQAction(NULL, tr("Aucune action"), this);
                action->setEnabled(false);

                menuStep->addAction(action);
            }

            addMenu(menuStep);
        }
    }
}

void GTreeStepContextMenu::executeStepRequired()
{
    if(selectedStep() != NULL)
    {
        emit executeSelectedStep(selectedStep());
    }
}

void GTreeStepContextMenu::executeModifyStepRequired()
{
    if(selectedStep() != NULL)
    {
        emit executeModifySelectedStep(selectedStep());
    }
}

void GTreeStepContextMenu::configureInputResultOfStepRequired()
{
    if(selectedStep() != NULL)
    {
        emit configureInputResultOfSelectedStep(selectedStep());
    }
}

void GTreeStepContextMenu::configureStepRequired()
{
    if(selectedStep() != NULL)
    {
        emit configureSelectedStep(selectedStep());
    }
}

void GTreeStepContextMenu::showStepInformations()
{
    if(selectedStep() != NULL)
    {
        GAboutStepDialog dialog(selectedStep());
        dialog.exec();
    }
}

void GTreeStepContextMenu::deleteStepRequired()
{
    if(selectedStep() != NULL)
    {
        emit deleteSelectedStep(selectedStep());
    }
}

void GTreeStepContextMenu::loadResultRequired()
{
    if(selectedStep() != NULL)
    {
        emit loadResultOfSelectedStep(dynamic_cast<StepSerializable*>(selectedStep()));
    }
}

void GTreeStepContextMenu::newStepRequired()
{
    MyQAction *action = (MyQAction*)sender();

    if((action->step() != NULL)
        && (selectedStep() != NULL))
    {
        emit addStep(selectedStep(), action->step());
    }
}

void GTreeStepContextMenu::insertStepRequired()
{
    MyQAction *action = (MyQAction*)sender();

    if((action->step() != NULL)
        && (selectedStep() != NULL))
    {
        emit insertStep(selectedStep(), action->step());
    }
}

void GTreeStepContextMenu::actionHovered()
{
    QAction *action = (QAction*)sender();

    if(action->isEnabled())
    {
        QPoint p = QCursor::pos();
        QString s = action->toolTip();

        QToolTip::showText(p, s);
    }
}
