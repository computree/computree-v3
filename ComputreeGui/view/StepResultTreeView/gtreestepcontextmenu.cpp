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

#include "ct_stepseparator.h"
#include "ct_stepcanbeaddedfirstseparator.h"
#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"
#include "ct_abstractstepplugin.h"

#include "view/StepResultTreeView/myqmenu.h"

GTreeStepContextMenu::GTreeStepContextMenu(CDM_StepManager &stepManager, QWidget *parent) : QMenu(parent)
{
    _selectedStep = NULL;
    _stepManager = &stepManager;
}

void GTreeStepContextMenu::setSelectedStep(CT_VirtualAbstractStep *step)
{
    _selectedStep = step;

    reload();
}

/////////////////// PRIVATE ///////////////////

CT_VirtualAbstractStep* GTreeStepContextMenu::selectedStep()
{
    return _selectedStep;
}

void GTreeStepContextMenu::addAllStepOnMenu(QList<CT_StepSeparator*> stepAvailable, MyQMenu *menuStep, bool insert)
{
    QListIterator<CT_StepSeparator*> it(stepAvailable);

    while(it.hasNext())
    {
        CT_StepSeparator *sep = it.next();
        QList<CT_VirtualAbstractStep*> stepList = sep->getStepList();
        QListIterator<CT_VirtualAbstractStep*> itStep(stepList);

        if(itStep.hasNext())
        {
            MyQMenu *underMenu = NULL;

            if(!sep->getTitle().isEmpty())
            {
                underMenu = new MyQMenu(sep->getTitle(), this);
                /*underMenu->setStyleSheet("QMenu::item[isManual=\"true\"]{ background-color:rgb(255,0,0); }"
                                        "QMenu::item:selected { background-color: #654321; }");*/
            }

            while(itStep.hasNext())
            {
                CT_VirtualAbstractStep *step = itStep.next();

                MyQAction *action = new MyQAction(step, tr("%1").arg(step->getStepDescription()), this);
                action->setToolTip(tr("%1 (F1 pour plus d'info)").arg(step->getStepName()));
                action->setIcon(QIcon(":/Icones/Icones/add.png"));

                if (step->isManual())
                {
                    QFont font;
                    font.setItalic(true);
                    action->setFont(font);
                }

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

                        QList<CT_VirtualAbstractStep*> childList = selectedStep()->getStepChildList();
                        QListIterator<CT_VirtualAbstractStep*> itChild(childList);

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

void GTreeStepContextMenu::addAllStepOnMenu(QList<CT_StepCanBeAddedFirstSeparator *> stepAvailable, MyQMenu *menuStep)
{
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
                action->setEnabled(true);

                if (step->isManual())
                {
                    QFont font;
                    font.setItalic(true);
                    action->setFont(font);
                }

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

    // Ajout des étapes par plugin
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

            QList<CT_StepSeparator*> stepAvailable = pluginManager->getPlugin(i)->getGenericsStepAvailable();
            addAllStepOnMenu(stepAvailable, menuStep);

            QList<CT_StepCanBeAddedFirstSeparator*> stepAddeFirstAvailable = pluginManager->getPlugin(i)->getCanBeAddedFirstStepAvailable();
            addAllStepOnMenu(stepAddeFirstAvailable, menuStep);

            if(menuStep->actions().isEmpty())
            {
                MyQAction* action = new MyQAction(NULL, tr("Aucune action"), this);
                action->setEnabled(false);

                menuStep->addAction(action);
            }
            addMenu(menuStep);
        }
    }
    addSeparator();


    MyQAction *action = new MyQAction(selectedStep(), tr("Exécuter"), this);
    action->setIcon(QIcon(":/Icones/Icones/play.png"));
    action->setEnabled((selectedStep() != NULL)
                       && ((!selectedStep()->needInputResults())
                           || ((selectedStep()->parentStep() != NULL)
                               && (selectedStep()->parentStep()->nResult() > 0))));
    connect(action, SIGNAL(triggered()), this, SLOT(executeStepRequired()));
    addAction(action);

    action = new MyQAction(selectedStep(), tr("Config. paramètres"), this);
    action->setIcon(QIcon(":/Icones/Icones/preferences-system.png"));
    action->setEnabled(selectedStep() != NULL);
    connect(action, SIGNAL(triggered()), this, SLOT(configureStepRequired()));
    addAction(action);

    action = new MyQAction(selectedStep(), tr("Config. résultats d'entrée"), this);
    action->setIcon(QIcon(":/Icones/Icones/preferences-system.png"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(configureInputResultOfStepRequired()));
    addAction(action);

    if(selectedStep()->isModifiable())
    {
        action = new MyQAction(selectedStep(), tr("Modifier (mode manuel)"), this);
        action->setIcon(QIcon(":/Icones/Icones/hand.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(executeModifyStepRequired()));
        addAction(action);
    }

    action = new MyQAction(selectedStep(), tr("Supprimer"), this);
    action->setIcon(QIcon(":/Icones/Icones/delete.png"));
    action->setEnabled(selectedStep() != NULL);
    connect(action, SIGNAL(triggered()), this, SLOT(deleteStepRequired()));
    addAction(action);

    action = new MyQAction(selectedStep(), tr("Documentation de l'étape"), this);
    action->setIcon(QIcon(":/Icones/Icones/info.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(showStepInformations()));
    addAction(action);

    addSeparator();

    //    action = new MyQAction(selectedStep(), tr("Déplier"), this);
    //    action->setIcon(QIcon(":/Icones/Icones/expand.png"));
    //    connect(action, SIGNAL(triggered()), this, SIGNAL(expand()));
    //    addAction(action);

    action = new MyQAction(selectedStep(), tr("Déplier toutes les étapes"), this);
    action->setIcon(QIcon(":/Icones/Icones/expand.png"));
    connect(action, SIGNAL(triggered()), this, SIGNAL(expandAll()));
    addAction(action);

    //    action = new MyQAction(selectedStep(), tr("Replier"), this);
    //    action->setIcon(QIcon(":/Icones/Icones/collapse.png"));
    //    connect(action, SIGNAL(triggered()), this, SIGNAL(collapse()));
    //    addAction(action);

    action = new MyQAction(selectedStep(), tr("Replier toutes les étapes"), this);
    action->setIcon(QIcon(":/Icones/Icones/collapse.png"));
    connect(action, SIGNAL(triggered()), this, SIGNAL(collapseAll()));
    addAction(action);

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
        emit loadResultOfSelectedStep(dynamic_cast<CT_AbstractStepSerializable*>(selectedStep()));
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

    QPoint p = QCursor::pos();
    QString s = action->toolTip();

    p.setY(p.y() + 10);

    QToolTip::showText(p, s);
}
