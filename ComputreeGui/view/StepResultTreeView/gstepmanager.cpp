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


#include "gstepmanager.h"

#include "dm_guimanager.h"

#include "cdm_configfile.h"

#include "gstepmanageroptions.h"

#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"
#include "ct_abstractstepplugin.h"
#include "gaboutstepdialog.h"

#include <QPushButton>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <typeinfo>
#include <QHeaderView>

GStepManager::GStepManager(CDM_StepManager &stepManager,
                           QWidget *parent) : QWidget(parent)
{
    _stepManager = &stepManager;

    _delegate = new MyTreeDelegate(&_view);

    _view.setModel(&_model);
    _view.setItemDelegate(_delegate);

    QStringList header;
    header << tr("Nom");
    header << tr("Progression");
    header << tr("Temps / Afficher");
    header << tr("Debug");

    _model.setHorizontalHeaderLabels(header);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    _view.header()->setResizeMode(0, QHeaderView::ResizeToContents);
#else
    _view.header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
#endif

    delete layout();
    setLayout(new QVBoxLayout());

    layout()->addWidget(&_view);

    _contextMenuStep = new GTreeStepContextMenu(*_stepManager, this);
    m_contextMenuGroupResult = new QMenu(this);

    m_contextMenuGroupResult->addAction(QIcon(":/Icones/Icones/expand.png"), tr("Déplier"), this, SLOT(expandSelected()));
    m_contextMenuGroupResult->addAction(QIcon(":/Icones/Icones/expand.png"), tr("Déplier tous les résultats"), this, SLOT(expandAllTypeOfSelected()));
    m_contextMenuGroupResult->addAction(QIcon(":/Icones/Icones/collapse.png"), tr("Replier"), this, SLOT(collapseSelected()));
    m_contextMenuGroupResult->addAction(QIcon(":/Icones/Icones/collapse.png"), tr("Replier tous les résultats"), this, SLOT(collapseAllTypeOfSelected()));

    _view.setContextMenuPolicy(Qt::CustomContextMenu);
    //_view.setExpandsOnDoubleClick(true);

    connect(_contextMenuStep, SIGNAL(executeSelectedStep(CT_VirtualAbstractStep*)), this, SLOT(executeStep(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(_contextMenuStep, SIGNAL(executeModifySelectedStep(CT_VirtualAbstractStep*)), this, SLOT(executeModifyStep(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(_contextMenuStep, SIGNAL(configureInputResultOfSelectedStep(CT_VirtualAbstractStep*)), this, SLOT(configureInputResultOfStep(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(_contextMenuStep, SIGNAL(configureSelectedStep(CT_VirtualAbstractStep*)), this, SLOT(configureStep(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(_contextMenuStep, SIGNAL(deleteSelectedStep(CT_VirtualAbstractStep*)), this, SLOT(removeStep(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(_contextMenuStep, SIGNAL(loadResultOfSelectedStep(CT_AbstractStepSerializable*)), this, SLOT(loadResultStep(CT_AbstractStepSerializable*)), Qt::QueuedConnection);
    connect(_contextMenuStep, SIGNAL(expand()), this, SLOT(expandSelected()));
    connect(_contextMenuStep, SIGNAL(expandAll()), this, SLOT(expandAllTypeOfSelected()));
    connect(_contextMenuStep, SIGNAL(collapse()), this, SLOT(collapseSelected()));
    connect(_contextMenuStep, SIGNAL(collapseAll()), this, SLOT(collapseAllTypeOfSelected()));

    connect(_stepManager, SIGNAL(stepAdded(CT_VirtualAbstractStep*)), this, SLOT(stepAdded(CT_VirtualAbstractStep*)), Qt::DirectConnection);
    connect(_stepManager, SIGNAL(stepInserted(int,CT_VirtualAbstractStep*)), this, SLOT(stepInserted(int,CT_VirtualAbstractStep*)), Qt::DirectConnection);
    connect(_stepManager, SIGNAL(stepToBeRemoved(CT_VirtualAbstractStep*)), this, SLOT(stepToBeRemoved(CT_VirtualAbstractStep*)), Qt::DirectConnection);

    connect(_stepManager, SIGNAL(resultToBeSerialized(const CT_AbstractResult*)), this, SLOT(resultToBeSerialized(const CT_AbstractResult*)), Qt::DirectConnection);

    connect(&_view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showViewContextMenu(QPoint)), Qt::QueuedConnection);
    connect(&_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(indexDoubleClicked(QModelIndex)), Qt::QueuedConnection);
    connect(_view.selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged(QItemSelection,QItemSelection)));

    connect(this, SIGNAL(addResult(QStandardItem*,CT_AbstractResult*)), this, SLOT(resultToAdd(QStandardItem*,CT_AbstractResult*)), Qt::QueuedConnection);
    connect(this, SIGNAL(removeResult(QStandardItem*,MyQStandardItem*)), this, SLOT(resultToRemove(QStandardItem*,MyQStandardItem*)), Qt::QueuedConnection);
    connect(this, SIGNAL(removeItem(QStandardItem*)), this, SLOT(itemToRemove(QStandardItem*)), Qt::QueuedConnection);
}

GStepManager::~GStepManager()
{
    delete m_contextMenuGroupResult;
}

CDM_StepManager* GStepManager::getStepManager() const
{
    return _stepManager;
}

////////////////// PRIVATE /////////////////

QList<QStandardItem *> GStepManager::createItemsForStep(CT_VirtualAbstractStep &step)
{
    QList<QStandardItem *> list;

    // nom de l'tape
    MyQStandardItem *item = new MyQStandardItem(&step, NULL, MyQStandardItem::StepName, GStepManager::staticGetStepName(step));

    item->setToolTip(step.getToolTip());

    // editable pour un nom customizé
    item->setEditable(true);
    connect(item, SIGNAL(dataChanged(QStandardItem*)), this, SLOT(itemDataChanged(QStandardItem*)));
    list.append(item);

    // progression
    item = new MyQStandardItem(&step, NULL, MyQStandardItem::StepProgress, step.getProgress());
    item->setEditable(false);
    connect(&step, SIGNAL(inProgress(int)), item, SLOT(setIntData(int)), Qt::QueuedConnection);
    list.append(item);

    // temps coul
    item = new MyQStandardItem(&step, NULL, MyQStandardItem::StepElapsedTime, step.getExecuteTime());
    item->setEditable(false);
    connect(&step, SIGNAL(elapsed(int)), item, SLOT(setIntData(int)), Qt::QueuedConnection);
    list.append(item);

    // debug
    item = new MyQStandardItem(&step, NULL, MyQStandardItem::StepDebug, QString(""));
    item->setEditable(false);
    item->setCheckable(true);
    item->setBoolData(step.isDebugModeOn());
    connect(item, SIGNAL(dataChanged(QStandardItem*)), this, SLOT(itemDataChanged(QStandardItem*)));
    list.append(item);

    setStepItemBackgroundColor(step, list);

    return list;
}

QList<QStandardItem *> GStepManager::createItemsForResult(CT_AbstractResult &res)
{
    QList<QStandardItem *> list;

    // nom du rsultat
    MyQStandardItem *item = new MyQStandardItem(NULL, &res, MyQStandardItem::ResultName, res.getName());

    item->setToolTip(QString("%1 (%2)").arg(res.getName()).arg(res.getToolTip()));

    item->setEditable(false);
    list.append(item);

    // déchargement de la mémoire ou sérialisation
    item = new MyQStandardItem(NULL, &res, MyQStandardItem::ResultProgress, res.getClearFromMemoryProgress());
    item->setEditable(false);
    connect(&res, SIGNAL(clearFromMemoryInProgress(int)), item, SLOT(setIntData(int)), Qt::QueuedConnection);
    list.append(item);

    // affichage ou non des modèles
    item = new MyQStandardItem(NULL, &res, MyQStandardItem::ResultVisibility, QString(""));
    item->setCheckable(true);
    item->setEditable(false);
    connect(&res, SIGNAL(busyStateChanged(bool)), item, SLOT(slotSetDisabled(bool)));
    item->setEnabled(!res.isBusy());
    //connect(&res, SIGNAL(busyStateChanged(bool)), item, SLOT(setBoolDataToFalseWhenDataIsTrue(bool)));
    connect(item, SIGNAL(dataChanged(QStandardItem*)), this, SLOT(itemDataChanged(QStandardItem*)));
    list.append(item);

    item = new MyQStandardItem(NULL, &res, MyQStandardItem::ResultEmpty, QString(""));
    item->setEditable(false);
    list.append(item);

    setResultItemBackgroundColor(ResultInfo(res.getClearFromMemoryProgress() < 100, false), list);

    return list;
}

void GStepManager::setStepItemBackgroundColor(CT_VirtualAbstractStep &step, QList<QStandardItem *> &list)
{
    Q_UNUSED(step)

    QListIterator<QStandardItem*> it(list);

    while(it.hasNext())
    {
        QStandardItem *item = it.next();

        item->setData(step.isManual() ? QColor(183, 225, 170) : QColor(170, 186, 225), Qt::BackgroundColorRole);
    }
}

void GStepManager::setResultItemBackgroundColor(ResultInfo info, QList<QStandardItem *> &list)
{
    QListIterator<QStandardItem*> it(list);

    while(it.hasNext())
    {
        QStandardItem *item = it.next();

        if(info._isClearedFromMemory)
        {
            item->setData(QColor(236, 208, 209), Qt::BackgroundColorRole);
        }
        else if(info._isSerialized)
        {
            item->setData(QColor(206, 238, 211), Qt::BackgroundColorRole);
        }
        else
        {
            item->setData(QColor(220, 220, 220), Qt::BackgroundColorRole);
        }
    }
}

MyQStandardItem* GStepManager::findItem(CT_VirtualAbstractStep *step)
{
    MyQStandardItem *item = getSelectedItem();

    if((item != NULL)
        && (item->step() == step))
    {
        return item;
    }

    return recursiveFindItem(step);
}

MyQStandardItem* GStepManager::findItem(CT_AbstractResult *res)
{
    MyQStandardItem *item = getSelectedItem();

    if((item != NULL)
        && (item->result() == res))
    {
        return item;
    }

    return recursiveFindItem(res);
}

MyQStandardItem* GStepManager::getSelectedItem()
{
    QModelIndexList list = _view.selectionModel()->selectedRows();

    if(!list.isEmpty())
    {
        return (MyQStandardItem*)_model.itemFromIndex(list.first());
    }

    return NULL;
}

MyQStandardItem* GStepManager::recursiveFindItem(CT_VirtualAbstractStep *step)
{
    CT_VirtualAbstractStep* parent = step->parentStep();
    QStandardItem *parentItem = NULL;

    if(parent != NULL)
    {
        // on recherche le parent
        parentItem = recursiveFindItem(parent);
    }
    // on est arriver au dbut de l'arbre
    else
    {
        // on recherche l'tape dans les enfants
        // de l'item root
        parentItem = _model.invisibleRootItem();
    }

    if(parentItem != NULL)
    {
        int n = parentItem->rowCount();

        for(int i=0; i<n; ++i)
        {
            MyQStandardItem *item = (MyQStandardItem*)parentItem->child(i);

            if(item->step() == step)
            {
                return item;
            }
        }
    }

    return NULL;
}

MyQStandardItem* GStepManager::recursiveFindItem(CT_AbstractResult *res)
{
    CT_VirtualAbstractStep* parent = res->parentStep();

    MyQStandardItem *stepItem = recursiveFindItem(parent);

    if(stepItem != NULL)
    {
        return getItemForResult(stepItem, res);
    }

    return NULL;
}

MyQStandardItem* GStepManager::getItemForResult(QStandardItem *stepItem, CT_AbstractResult *res)
{
    int n = stepItem->rowCount();

    for(int i=0; i<n; ++i)
    {
        MyQStandardItem *item = (MyQStandardItem*)stepItem->child(i);

        if(item->result() == res)
        {
            return item;
        }
    }

    return NULL;
}

QList<MyQStandardItem *> GStepManager::getItemsForResult(QStandardItem *stepItem, CT_AbstractResult *res)
{
    QList<MyQStandardItem *> list;

    int n = stepItem->rowCount();

    for(int i=0; i<n; ++i)
    {
        MyQStandardItem *item = (MyQStandardItem*)stepItem->child(i);

        if(item->result() == res)
        {
            int nc = _model.columnCount();

            for(int j=0; j<nc; ++j)
            {
                item = (MyQStandardItem*)stepItem->child(i, j);
                list.append(item);
            }

            return list;
        }
    }

    return list;
}

void GStepManager::stepDataChanged(MyQStandardItem *item)
{
    CT_VirtualAbstractStep *step = item->step();

    if(step != NULL)
    {
        switch(item->columnType())
        {
        case MyQStandardItem::StepDebug :   {   bool debugOn = (item->checkState() == Qt::Checked);

                                                if(debugOn && !step->isDebuggable())
                                                {
                                                    item->setData(false, Qt::CheckStateRole);

                                                    showMessageStepNotDebuggable();
                                                }
                                                else if(step->isDebugModeOn() != debugOn)
                                                {
                                                    getStepManager()->setStepDebugModeOn(step, debugOn);
                                                }
                                                break;
                                            }

        case MyQStandardItem::StepName : {
                                            QString newName = item->data(Qt::DisplayRole).toString();
                                            int index = newName.indexOf("*");

                                            if(index != -1)
                                                newName = newName.remove(0, index+1);

                                            if(step->getStepCustomName() != newName)
                                            {
                                                step->setStepCustomName(newName);
                                                item->setStringData(GStepManager::staticGetStepName(*step));
                                            }

                                            break;
                                         }
        }
    }
}

void GStepManager::resultDataChanged(MyQStandardItem *item)
{
    _mutexItemRes.lock();

    CT_AbstractResult *res = item->result();

    _mutexItemRes.unlock();

    if(res != NULL)
    {
        switch(item->columnType())
        {
        case MyQStandardItem::ResultVisibility :    if(item->checkState() == Qt::Checked)
                                                    {
                                                        editItemDrawableModelOfResult(item->result());
                                                    }
                                                    else
                                                    {
                                                        removeEditItemDrawableModelOfResult(item->result());
                                                        removeItemDrawableOfResult(item->result());
                                                    }

                                                    break;
        }
    }

}

QString GStepManager::staticGetStepName(CT_VirtualAbstractStep &step)
{
    CT_AbstractStepLoadFile *stepLF = dynamic_cast<CT_AbstractStepLoadFile*>(&step);

    if(stepLF != NULL)
    {
        QString filePath = stepLF->getFilePath();
        QFileInfo info(filePath);

        return QString("%1%2").arg(step.isSettingsModified() ? "*" : "").arg(((step.getStepCustomName() == step.getStepExtendedDisplayableName()) && !info.fileName().isEmpty()) ? info.fileName() : step.getStepCustomName());
    }

    return QString("%1%2").arg(step.isSettingsModified() ? "*" : "").arg(step.getStepCustomName() == step.getStepExtendedDisplayableName() ? step.getStepExtendedDisplayableName() : step.getStepCustomName());
}

void GStepManager::showMessageStepNotDebuggable()
{
    QMessageBox::warning(this, tr("Debug"), tr("L'étape ne semble pas être débogable."));
}

bool GStepManager::checkExecuteStepAndShowWarningMessage(CT_VirtualAbstractStep *step, bool debugMode)
{
    bool continueExecution = true;

    if(_stepManager->getOptions().isAutoSaveEnable()
            && (step != NULL)
            && _stepManager->checkAutoSaveDisabledIfExecuteFromStep(*step))
    {
        continueExecution = false;

        QMessageBox msg(QMessageBox::Warning, tr("Attention"), tr("La srialisation semble tre active, si vous executez"
                                                                  " l'opration  partir de cette tape elle sera dsactive."), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);

        msg.setInformativeText(tr("Voulez-vous quand même continuer ?"));

        if(msg.clickedButton() == msg.button(QMessageBox::Yes))
        {
            continueExecution = true;
        }
    }

    if(continueExecution)
    {
        bool oneStepInDebugMode = _stepManager->checkOneStepIsInDebugModeFromStep(step);

        if(oneStepInDebugMode && !debugMode)
        {
            continueExecution = false;

            QMessageBox msg(QMessageBox::Warning, tr("Attention"), tr("Une ou plusieurs étapes sont en mode debug or vous allez lancer"
                                                                      " les traitements en mode normal."), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);

            msg.setInformativeText(tr("Voulez-vous quand même continuer ?"));

            msg.exec();

            if(msg.clickedButton() == msg.button(QMessageBox::Yes))
            {
                continueExecution = true;
            }
        }
        else if(!oneStepInDebugMode && debugMode)
        {
            continueExecution = false;

            QMessageBox msg(QMessageBox::Warning, tr("Attention"), tr("Aucune étape n'est en mode debug or vous allez lancer"
                                                                      " les traitements dans ce mode."), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);

            msg.setInformativeText(tr("Voulez-vous quand même continuer ?"));

            msg.exec();

            if(msg.clickedButton() == msg.button(QMessageBox::Yes))
            {
                continueExecution = true;
            }
        }
    }

    return continueExecution;
}

bool GStepManager::configureStepAndAdd(CT_VirtualAbstractStep *newStep, CT_VirtualAbstractStep *parentStep)
{
    if(newStep != NULL)
    {
        if(newStep->showPreConfigurationDialog())
        {
            if(newStep->initInResultModelList())
            {
                if(newStep->showInputResultConfigurationDialog())
                {
                    if(newStep->showPostConfigurationDialog())
                    {
                        if(newStep->initAfterConfiguration())
                        {
                            _stepManager->addStep(newStep, parentStep);

                            MyQStandardItem *item = findItem(newStep);

                            if(item != NULL)
                                _view.selectionModel()->select(_model.indexFromItem(item), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

                            return true;
                        }
                    }
                }
            }
        }

        delete newStep;
    }

    return false;
}

void GStepManager::recursiveExpandCollapseItemOfStep(MyQStandardItem *item, bool expand)
{
    if(item->step() != NULL)
    {
        if(expand)
            _view.expand(_model.indexFromItem(item));
        else
            _view.collapse(_model.indexFromItem(item));
    }

    int size = item->rowCount();

    for(int i=0; i<size; ++i)
        recursiveExpandCollapseItemOfStep((MyQStandardItem*)item->child(i), expand);
}

void GStepManager::recursiveExpandCollapseItemOfResultsGroup(MyQStandardItem *item, bool expand)
{
    if((item->step() == NULL) && (item->result() == NULL))
    {
        if(expand)
            _view.expand(_model.indexFromItem(item));
        else
            _view.collapse(_model.indexFromItem(item));
    }

    int size = item->rowCount();

    for(int i=0; i<size; ++i)
        recursiveExpandCollapseItemOfResultsGroup((MyQStandardItem*)item->child(i), expand);
}

void GStepManager::addStepToSelectedStepOrToRootAndConfigure(CT_VirtualAbstractStep *stepToCopy)
{
    MyQStandardItem *item = getSelectedItem();

    if((item == NULL) || (item->step() == NULL)) {
        CT_AbstractStepLoadFile *lfStep = dynamic_cast<CT_AbstractStepLoadFile*>(stepToCopy);

        if(lfStep != NULL) {
            addStepToParentAndConfigure(NULL, lfStep);
            return;
        }

        CT_AbstractStepCanBeAddedFirst *cbafStep = dynamic_cast<CT_AbstractStepCanBeAddedFirst*>(stepToCopy);

        if(cbafStep != NULL) {
            addStepToParentAndConfigure(NULL, cbafStep);
            return;
        }

        GUI_LOG->addErrorMessage(LogInterface::gui, tr("Impossible d'ajouter l'étape %1 à la racine car elle n'est pas du type CT_AbstractStepLoadFile ou CT_AbstractStepCanBeAddedFirst").arg(staticGetStepName(*stepToCopy)));
    } else if(item->step() != NULL) {
        addStepToParentAndConfigure(item->step(), stepToCopy);
    }
}

////////////// PUBLIC SLOTS //////////////

void GStepManager::addOpenFileStep(QString filePath)
{
    CDM_PluginManager *pluginManager = getStepManager()->getScriptManager()->getPluginManager();

    if(pluginManager->isAPluginLoaded())
    {
        QList<CT_AbstractStepLoadFile*>    stepLfList;
        CT_AbstractStepLoadFile            *stepToCopy = NULL;

        int count = pluginManager->countPluginLoaded();

        for(int i =0; i<count; ++i)
        {
            QList<CT_AbstractStepLoadFile*> steps = pluginManager->getPlugin(i)->getOpenFileStep(filePath);

            if(!steps.isEmpty())
            {
                stepToCopy = steps.first();
                stepLfList.append(steps);
            }
        }

        if(stepLfList.size() > 1)
        {
            stepToCopy = NULL;

            QDialog dialog(this);
            dialog.setLayout(new QVBoxLayout());

            QComboBox *cb = new QComboBox(&dialog);
            QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok
                                                         | QDialogButtonBox::Cancel,
                                                         Qt::Horizontal,
                                                         &dialog);

            connect(dbb, SIGNAL(accepted()), &dialog, SLOT(accept()));
            connect(dbb, SIGNAL(rejected()), &dialog, SLOT(reject()));

            dialog.layout()->addWidget(cb);
            dialog.layout()->addWidget(dbb);

            QListIterator<CT_AbstractStepLoadFile*> itLf(stepLfList);

            while(itLf.hasNext())
            {
                CT_AbstractStepLoadFile *stepLf = itLf.next();

                cb->addItem(GStepManager::staticGetStepName(*stepLf) + " (" + pluginManager->getPluginName(stepLf->getPlugin()) + ")");
            }

            if(dialog.exec() == 1)
            {
                stepToCopy = stepLfList.at(cb->currentIndex());
            }
        }

        if(stepToCopy != NULL)
        {
            stepLfList.removeOne(stepToCopy);

            CT_VirtualAbstractStep *newStep = stepToCopy->getPlugin()->createNewInstanceOfStep(*stepToCopy, NULL);

            CT_AbstractStepLoadFile *newStepLF = dynamic_cast<CT_AbstractStepLoadFile*>(newStep);

            if(newStepLF != NULL)
            {
                if(newStepLF->setFilePath(filePath))
                {
                    if(newStepLF->showPreConfigurationDialog())
                    {
                        if(newStepLF->initInResultModelList())
                        {
                            if(newStepLF->showInputResultConfigurationDialog())
                            {
                                if(newStepLF->initAfterConfiguration())
                                {
                                    _stepManager->addStep(newStepLF);

                                    return;
                                }
                            }
                        }
                    }
                }
            }

            delete newStep;
        }
    }
}

void GStepManager::addStepToParentAndConfigure(CT_VirtualAbstractStep *parentStep, CT_VirtualAbstractStep *stepToCopy)
{
    if(stepToCopy->getPlugin() != NULL)
    {
        if((parentStep == NULL) && stepToCopy->needInputResults())
            return;

        CT_VirtualAbstractStep *newStep = stepToCopy->getPlugin()->createNewInstanceOfStep(*stepToCopy, parentStep);

        configureStepAndAdd(newStep, parentStep);
    }
}

void GStepManager::insertStepInParentAndConfigure(CT_VirtualAbstractStep *parentStep, CT_VirtualAbstractStep *stepToCopy)
{
    if((parentStep != NULL)
            && (stepToCopy->getPlugin() != NULL))
    {
        CT_VirtualAbstractStep *newStep = stepToCopy->getPlugin()->createNewInstanceOfStep(*stepToCopy, parentStep);

        configureStepAndAdd(newStep, parentStep);
    }
}

bool GStepManager::executeStep(CT_VirtualAbstractStep *step)
{
    if(!_stepManager->isRunning())
    {
        if(checkExecuteStepAndShowWarningMessage(step, false))
            return _stepManager->executeStep(step);
        else
            return false;
    }

    return _stepManager->executeStep(step);
}

bool GStepManager::executeModifyStep(CT_VirtualAbstractStep *step)
{
    if(!_stepManager->isRunning())
        return _stepManager->executeModifyStep(step);

    return _stepManager->executeModifyStep(step);
}

bool GStepManager::executeOrForwardStepInDebugMode(CT_VirtualAbstractStep *step)
{
    if(!_stepManager->isRunning())
    {
        if(checkExecuteStepAndShowWarningMessage(step, true))
            return _stepManager->executeOrForwardStepInDebugMode(step);
        else
            return false;
    }

    return _stepManager->executeOrForwardStepInDebugMode(step);
}

bool GStepManager::executeOrForwardStepFastInDebugMode(CT_VirtualAbstractStep *step)
{
    if(!_stepManager->isRunning())
    {
        if(checkExecuteStepAndShowWarningMessage(step, true))
            return _stepManager->executeOrForwardStepFastInDebugMode(step);
        else
            return false;
    }

    return _stepManager->executeOrForwardStepFastInDebugMode(step);
}

bool GStepManager::executeOrForwardStepAutoInDebugMode(CT_VirtualAbstractStep *step)
{
    if(!_stepManager->isRunning())
    {
        if(checkExecuteStepAndShowWarningMessage(step, true))
            return _stepManager->executeOrForwardStepAutoInDebugMode(step);
        else
            return false;
    }

    return _stepManager->executeOrForwardStepAutoInDebugMode(step);
}

bool GStepManager::configureInputResultOfStep(CT_VirtualAbstractStep *step)
{
    if((step != NULL)
        && (!_stepManager->isRunning()))
    {
        if(step->showInputResultConfigurationDialog())
        {
            return step->initAfterConfiguration();
        }
    }

    return false;
}

bool GStepManager::configureStep(CT_VirtualAbstractStep *step)
{
    if((step != NULL)
        && (!_stepManager->isRunning()))
    {
        if(step->showPostConfigurationDialog())
        {
            return step->initAfterConfiguration();
        }
    }

    return false;
}

bool GStepManager::removeStep(CT_VirtualAbstractStep *step)
{
    if(step != NULL)
    {
        return GUI_MANAGER->asyncRemoveStep(*step, NULL);
    }

    return false;
}

bool GStepManager::loadResultStep(CT_AbstractStepSerializable *step)
{
    if(step != NULL)
    {
        return GUI_MANAGER->asyncLoadResultStep(*step, NULL);
    }

    return false;
}

bool GStepManager::editItemDrawableModelOfResult(CT_AbstractResult *res)
{
    if(res != NULL)
    {
        return GUI_MANAGER->editItemDrawableModelOfResult(*res);
    }

    return false;
}

bool GStepManager::removeEditItemDrawableModelOfResult(CT_AbstractResult *res)
{
    if(res != NULL)
    {
        return GUI_MANAGER->removeEditItemDrawableModelOfResult(*res);
    }

    return false;
}

bool GStepManager::removeItemDrawableOfResult(CT_AbstractResult *res)
{
    if(res != NULL)
    {
        return GUI_MANAGER->asyncRemoveAllItemDrawableOfResultFromView(*res, NULL);
    }

    return false;
}

void GStepManager::showStepManagerOptions()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Configuration"));

    QVBoxLayout *layout = new QVBoxLayout();

    GStepManagerOptions *widget = new GStepManagerOptions(&dialog);
    widget->setOptions(_stepManager->getOptions());

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                       Qt::Horizontal,
                                                       &dialog);

    layout->addWidget(widget);
    layout->addWidget(buttonBox);

    delete dialog.layout();
    dialog.setLayout(layout);

    connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if(dialog.exec())
    {
        _stepManager->setOptions(widget->getOptions());
    }
}

////////////// PRIVATE SLOTS //////////////

void GStepManager::stepAdded(CT_VirtualAbstractStep *step)
{
    CT_VirtualAbstractStep *parentStep = step->parentStep();
    QStandardItem *item = NULL;

    if(parentStep == NULL)
    {
        item = _model.invisibleRootItem();
    }
    else
    {
        item = findItem(parentStep);
    }

    if(item != NULL)
    {
        connect(step, SIGNAL(resultAdded(const CT_AbstractResult*)), this, SLOT(resultAdded(const CT_AbstractResult*)), Qt::DirectConnection);
        connect(step, SIGNAL(resultToBeClearedFromMemory(const CT_AbstractResult*)), this, SLOT(resultToBeClearedFromMemory(const CT_AbstractResult*)), Qt::DirectConnection);
        connect(step, SIGNAL(resultToBeRemoved(const CT_AbstractResult*)), this, SLOT(resultToBeRemoved(const CT_AbstractResult*)), Qt::DirectConnection);
        connect(step, SIGNAL(settingsModified()), this, SLOT(stepSettingsModified()), Qt::QueuedConnection);

        QList<QStandardItem *> newItems = createItemsForStep(*step);

        item->appendRow(newItems);

        _view.expand(_model.indexFromItem(newItems.first()));
    }
}

void GStepManager::stepInserted(int row, CT_VirtualAbstractStep *step)
{
    CT_VirtualAbstractStep *parentStep = step->parentStep();
    QStandardItem *item = NULL;

    if(parentStep == NULL)
    {
        item = _model.invisibleRootItem();
    }
    else
    {
        item = findItem(parentStep);
    }

    if(item != NULL)
    {
        connect(step, SIGNAL(resultAdded(CT_AbstractResult*)), this, SLOT(resultAdded(CT_AbstractResult*)), Qt::DirectConnection);
        connect(step, SIGNAL(resultToBeClearedFromMemory(CT_AbstractResult*)), this, SLOT(resultToBeClearedFromMemory(CT_AbstractResult*)), Qt::DirectConnection);
        connect(step, SIGNAL(resultToBeRemoved(CT_AbstractResult*)), this, SLOT(resultToBeRemoved(CT_AbstractResult*)), Qt::DirectConnection);
        connect(step, SIGNAL(settingsModified()), this, SLOT(stepSettingsModified()), Qt::QueuedConnection);

        QList<QStandardItem*> list = createItemsForStep(*step);

        if(row > 0)
        {
            item->insertRow(row, list);
        }
        else
        {
            QStandardItem *newItem = list.first();

            int nResult = ((MyQStandardItem*)item)->step()->nResult();

            while(item->rowCount() > nResult)
            {
                QList<QStandardItem*> tmpList = item->takeRow(item->rowCount()-1);
                newItem->insertRow(0, tmpList);
            }

            item->insertRow(item->rowCount(), list);
        }

        _view.expandAll();
    }
}

void GStepManager::stepToBeRemoved(CT_VirtualAbstractStep *step)
{
    QStandardItem *item = findItem(step);

    if(item != NULL)
    {
        // on emet un signal pour qu'on supprime
        // l'item dans une boucle d'vnement. Que se passerai-t-il
        // si on ne faisait pas a : lorsqu'un thread supprime une tape
        // du step manager et qu'on reoit directement le signal,
        // on n'est pas dans la boucle d'vnement donc la vue est peut tre
        // en train d'tre mis  jour, donc si on supprime l'item pendant la
        // mise  jour du qteeview il peut y avoir un plantage.
        emit removeItem(item);
    }
}

void GStepManager::stepSettingsModified()
{
    CT_VirtualAbstractStep *step = dynamic_cast<CT_VirtualAbstractStep*>(sender());

    if(step != NULL)
    {
        MyQStandardItem *item = findItem(step);

        if(item != NULL)
        {
            item->setDataWithoutSignal(GStepManager::staticGetStepName(*step), Qt::DisplayRole);
            item->setToolTip(step->getToolTip());
        }
    }
}

void GStepManager::resultAdded(const CT_AbstractResult *res)
{
    QStandardItem *item = findItem(res->parentStep());

    if(item != NULL)
    {
        _mutexResList.lock();
        _resToBeAddedList.append((CT_AbstractResult*)res);
        _mutexResList.unlock();

        // on emet un signal pour les mmes raisons que
        // lors de la suppression d'tape
        emit addResult(item, (CT_AbstractResult*)res);
    }
}

void GStepManager::resultToBeClearedFromMemory(const CT_AbstractResult *res)
{
    QStandardItem *item = findItem((CT_AbstractResult*)res);

    if(item != NULL)
    {
        QStandardItem *parent = item->parent();
        int row = item->row();
        int col = parent->columnCount();

        QList<QStandardItem*> list;

        for(int i=0; i<col; ++i)
        {
            list.append(parent->child(row, i));
        }

        setResultItemBackgroundColor(ResultInfo(true, false), list);
    }
}

void GStepManager::resultToBeRemoved(const CT_AbstractResult *res)
{
    // Dans un cas bien particulier il peut y avoir une
    // erreur d'accs au rsultat, ce cas est le suivant :
    //
    // Si on a reu un signal qu'on doit ajouter un rsultat,
    // on emet un signal d'ajout du rsultat. Si juste aprs on reoit
    // un signal qu'un rsultat va tre supprim on emet un signal pour
    // supprimer le rsultat. SI avant de recevoir NOTRE signal d'ajout
    // du rsultat le rsultat est supprimer alors on aura une erreur
    // d'accs mmoire si on essaye de crer des items pour ce rsultat.
    //
    // C'est pourquoi j'ai cr cette liste qui permet de vrifier si on
    // a reu aussi un signal de suppression du rsultat aprs le signal d'ajout
    // comme a on ne cre pas le rsultat.
    //
    //
    // Je rapelle qu'on reoit un signal de l'tape quelle va ajouter un rsultat
    // en Qt::DirectConnection et qu'on met un signal en Qt::QueuedConnection
    // pour ce remettre dans l'event loop de Qt et ne pas avoir d'erreur si la vue
    // est en train de ce mettre  jour.
    _mutexResList.lock();
    _resToBeAddedList.removeOne((CT_AbstractResult*)res);
    _mutexResList.unlock();

    QStandardItem *item = findItem(res->parentStep());
    MyQStandardItem *itemRes = NULL;

    QList<MyQStandardItem*> itemResList = getItemsForResult(item, (CT_AbstractResult*)res);
    QListIterator<MyQStandardItem*> it(itemResList);

    if(it.hasNext())
    {
        itemRes = it.next();

        it.toFront();

        while(it.hasNext())
        {
            _mutexItemRes.lock();
            it.next()->removeResult();
            _mutexItemRes.unlock();
        }
    }

    if((item != NULL)
            && (itemRes != NULL))
    {
        // on emet un signal pour les mmes raisons que
        // lors de la suppression d'tape
        emit removeResult(item, itemRes);
    }
}

void GStepManager::resultToBeSerialized(const CT_AbstractResult *res)
{
    QStandardItem *item = findItem((CT_AbstractResult*)res);

    if(item != NULL)
    {
        QStandardItem *parent = item->parent();
        int row = item->row();
        int col = parent->columnCount();

        QList<QStandardItem*> list;

        for(int i=0; i<col; ++i)
        {
            list.append(parent->child(row, i));
        }

        setResultItemBackgroundColor(ResultInfo(false, true), list);
    }
}

void GStepManager::itemDataChanged(QStandardItem *item)
{
    MyQStandardItem *myItem = (MyQStandardItem*)item;

    if(myItem->step() != NULL)
    {
        stepDataChanged(myItem);
    }
    else if(myItem->result() != NULL)
    {
        resultDataChanged(myItem);
    }
}

void GStepManager::indexDoubleClicked(QModelIndex index)
{
    MyQStandardItem *myItem = (MyQStandardItem*)_model.itemFromIndex(index);

    if((myItem != NULL)
            && (myItem->result() != NULL))
    {
        QStandardItem *it = ((QStandardItem*)myItem)->parent()->child(myItem->row(), MyQStandardItem::ResultVisibility-MyQStandardItem::ResultName);

        if(it->isEnabled())
            it->setCheckState(it->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    }
    else if((myItem != NULL)
            && (myItem->step() != NULL)
            && (myItem->columnType() != MyQStandardItem::StepName))
    {
        GAboutStepDialog dialog(myItem->step());
        dialog.exec();
    }
}

void GStepManager::resultToAdd(QStandardItem *parentItem, CT_AbstractResult *res)
{
    _mutexResList.lock();

    int index;

    // si le rsultat est toujours dans la liste.
    if((index = _resToBeAddedList.indexOf(res)) >= 0)
    {
        QList<QStandardItem*> newItems = createItemsForResult(*res);

        parentItem->insertRow(0, newItems);

        _resToBeAddedList.removeAt(index);

        _view.expand(_model.indexFromItem(newItems.first()));
    }

    _mutexResList.unlock();
}

void GStepManager::resultToRemove(QStandardItem *parentItem, MyQStandardItem *resItem)
{
    if(resItem != NULL)
    {
        parentItem->removeRow(resItem->row());
    }
}

void GStepManager::itemToRemove(QStandardItem *item)
{
    if(item->parent() != NULL)
    {
        item->parent()->removeRow(item->row());
    }
    else
    {
        _model.invisibleRootItem()->removeRow(item->row());
    }
}

void GStepManager::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    MyQStandardItem *item = getSelectedItem();

    if((item != NULL) && (item->step() != NULL))
        emit stepSelected(item->step());
    else
        emit stepSelected(NULL);
}

void GStepManager::expandSelected()
{
    QModelIndexList list = _view.selectionModel()->selectedRows();

    if(!list.isEmpty())
        _view.expand(list.first());
}

void GStepManager::expandAllTypeOfSelected()
{
    MyQStandardItem *item = getSelectedItem();

    if(item != NULL)
    {
        if(item->step() != NULL)
        {
            QStandardItem *root = _model.invisibleRootItem();
            int size = root->rowCount();

            for(int i=0; i<size; ++i)
                recursiveExpandCollapseItemOfStep((MyQStandardItem*)root->child(i), true);
        }
        else if(item->result() == NULL)
        {
            QStandardItem *root = _model.invisibleRootItem();
            int size = root->rowCount();

            for(int i=0; i<size; ++i)
                recursiveExpandCollapseItemOfResultsGroup((MyQStandardItem*)root->child(i), true);
        }
    }
}

void GStepManager::collapseSelected()
{
    QModelIndexList list = _view.selectionModel()->selectedRows();

    if(!list.isEmpty())
        _view.collapse(list.first());
}

void GStepManager::collapseAllTypeOfSelected()
{
    MyQStandardItem *item = getSelectedItem();

    if(item != NULL)
    {
        if(item->step() != NULL)
        {
            QStandardItem *root = _model.invisibleRootItem();
            int size = root->rowCount();

            for(int i=0; i<size; ++i)
                recursiveExpandCollapseItemOfStep((MyQStandardItem*)root->child(i), false);
        }
        else if(item->result() == NULL)
        {
            QStandardItem *root = _model.invisibleRootItem();
            int size = root->rowCount();

            for(int i=0; i<size; ++i)
                recursiveExpandCollapseItemOfResultsGroup((MyQStandardItem*)root->child(i), false);
        }
    }
}

void GStepManager::showViewContextMenu(const QPoint &point)
{
    MyQStandardItem *item = getSelectedItem();

    if(item != NULL)
    {
        if(item->step() != NULL)
        {
            _contextMenuStep->setSelectedStep(item->step());
            _contextMenuStep->exec(_view.viewport()->mapToGlobal(point));
        }
        else if(item->result() == NULL)
        {
            m_contextMenuGroupResult->exec(_view.viewport()->mapToGlobal(point));
        }
    }
}
