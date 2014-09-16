/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "qglobal.h"

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtConcurrentMap>
#else
#include <QtConcurrent/QtConcurrentMap>
#endif

#include "ct_global/ct_context.h"

#include "ct_virtualabstractstep.h"
#include "ct_stepinitializedata.h"

#include "ct_abstractresult.h"
#include "ct_abstractitemdrawable.h"

#include "ct_stepconfigurabledialog.h"

#include "ct_resultgroup.h"

#include "ct_turnindexmanager.h"
#include "ct_inmanager.h"
#include "ct_outmanager.h"

#include "ct_inresultmodelgrouptocopy.h"
#include "ct_outresultmodelgroupcopy.h"
#include "ct_outresultmodelgroup.h"

#include "ct_tools/model/abstract/ct_abstractoutmodelcopyaction.h"
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

#include "ct_generateoutresultmodelname.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include <QMutex>
#include <QWaitCondition>
#include <QDialog>
#include <QMessageBox>

#include <assert.h>

int CT_VirtualAbstractStep::CURRENT_ID = 1;

CT_VirtualAbstractStep::CT_VirtualAbstractStep(CT_StepInitializeData &dataInit)
{
    _uniqueId = CURRENT_ID;
    ++CURRENT_ID;

    m_guiContext = NULL;
    _preConfigDialog = NULL;
    _postConfigDialog = NULL;

    _data = &dataInit;

    _defaultOutModelCreating = false;
    _canChangeSetOneCompute = false;

    _running = false;
    _stop = false;
    _paused = false;
    _error_code = 0;
    _elapsed = 0;
    _progress = 0;
    _progressClearMem = 0;
    _isSettingsModified = true;
    _debuggable = false;
    _debugMode = false;
    _ackOfDebugMode = false;
    m_manual = false;
    m_modifiable = false;
    m_firstCallToManualMode = true;
    m_mustQuitManualMode = true;
    _result_absolute_path_saved_dir = "";
    _result_saved_dir = "";
    _n_result_saved = 0;

    _step_by_step_size = 0;
    _index_step_step_to_go = 0;

    _serializedResultsRelativeFilename ="";
    _nResultSerialized = 0;

    _inManager = new CT_InManager(*this);

    _turnIndexManager = new CT_TurnIndexManager(*_inManager->getTurnManager());

    _inManager->setTurnIndexManager(_turnIndexManager);
    _outManager = new CT_OutManager(*this, *_inManager->getTurnManager());
    _outManager->setTurnIndexManager(_turnIndexManager);

    connect(_outManager->getResultManager(), SIGNAL(resultAdded(const CT_AbstractResult*)), this, SIGNAL(resultAdded(const CT_AbstractResult*)), Qt::DirectConnection);
}

CT_VirtualAbstractStep::~CT_VirtualAbstractStep()
{
    qDeleteAll(_stepChildList.begin(), _stepChildList.end());

    clearOutResult();

    delete _inManager;
    delete _data;
    delete _preConfigDialog;
    delete _postConfigDialog;
    delete _outManager;
    delete _turnIndexManager;
}

int CT_VirtualAbstractStep::uniqueID() const
{
    return _uniqueId;
}

void CT_VirtualAbstractStep::init()
{
}

bool CT_VirtualAbstractStep::initInResultModelList()
{
    if(_stepChildList.isEmpty())
    {
        _inManager->getTurnManager()->clearTurnList();
        _inManager->getResultModelManager()->clearAllResultModels();
        createInResultModelList();

        return true;
    }

    return false;
}

QList<const CT_InAbstractResultModel*> CT_VirtualAbstractStep::getInResultsModel() const
{
    QList<const CT_InAbstractResultModel*> list;

    QListIterator<CT_InAbstractResultModel*> it(_inManager->getResultModelManager()->models());

    while(it.hasNext())
        list.append(it.next());

    return list;
}

QList<const CT_OutAbstractResultModel*> CT_VirtualAbstractStep::getOutResultsModel() const
{
    QList<const CT_OutAbstractResultModel*> list;

    if(_outManager->getTurnManager()->nTurn() > 0)
    {
        QListIterator<CT_OutAbstractResultModel*> it(_outManager->getTurnManager()->turns().first()->models());

        while(it.hasNext())
            list.append(it.next());
    }
    else if(!_outManager->getResultModelManager()->isEmpty())
    {
        QListIterator<CT_OutAbstractResultModel*> it(_outManager->getResultModelManager()->models());

        while(it.hasNext())
            list.append(it.next());
    }

    return list;
}

bool CT_VirtualAbstractStep::initAfterConfiguration()
{
    if(protectedInitAfterConfiguration())
    {
        if(_stepChildList.isEmpty())
        {
            // les resultats modeles en sortie sont crees apres l'affichage de la fenetre
            // de configuration de l'etape.

            // creation des modeles de sortie en fonction du nombre de tour
            return _outManager->createAllTurn();
        }

        return true;
    }

    return false;
}

bool CT_VirtualAbstractStep::hasChoiceBetweenMultipleInputResults()
{
    return _inManager->getResultModelConfigurationManager()->checkIfMustCreateOrShowConfigurationDialog();
}

CT_AbstractStepPlugin* CT_VirtualAbstractStep::getPlugin() const
{
    return _data->getPlugin();
}

SettingsNodeGroup* CT_VirtualAbstractStep::getAllSettings() const
{
    SettingsNodeGroup *root = new SettingsNodeGroup("Settings");

    SettingsNodeGroup *rootStep = new SettingsNodeGroup("CT_VirtualAbstractStep");

    if(!_customName.isEmpty())
        rootStep->addValue(new SettingsNodeValue("CustomName", getStepCustomName()));

    if(_preConfigDialog != NULL)
    {
        SettingsNodeGroup *preC = new SettingsNodeGroup("PreConfigDialog");

        QList<SettingsNodeGroup*> groups = _preConfigDialog->getAllValues();

        while(!groups.isEmpty())
            preC->addGroup(groups.takeFirst());

        rootStep->addGroup(preC);
    }

    if(_postConfigDialog != NULL)
    {
        SettingsNodeGroup *postC = new SettingsNodeGroup("PostConfigDialog");

        QList<SettingsNodeGroup*> groups = _postConfigDialog->getAllValues();

        while(!groups.isEmpty())
            postC->addGroup(groups.takeFirst());

        rootStep->addGroup(postC);
    }

    // input result configuration
    SettingsNodeGroup *inputC = new SettingsNodeGroup("InputResultConfigDialog");

    QList<SettingsNodeGroup*> groups = _inManager->getResultModelConfigurationManager()->getAllValues();

    while(!groups.isEmpty())
        inputC->addGroup(groups.takeFirst());

    rootStep->addGroup(inputC);

    root->addGroup(rootStep);

    return root;
}

bool CT_VirtualAbstractStep::setAllSettings(const SettingsNodeGroup *settings)
{
    if(settings == NULL)
        return false;

    QList<SettingsNodeGroup*> groups = settings->groupsByTagName("CT_VirtualAbstractStep");

    if(groups.isEmpty())
        return false;

    SettingsNodeGroup *rootStep = groups.first();

    QList<SettingsNodeValue*> values = rootStep->valuesByTagName("CustomName");

    if(!values.isEmpty())
        _customName = values.first()->value().toString();

    groups = rootStep->groupsByTagName("PreConfigDialog");

    //cree la fenetre de pre-configuration
    createPreConfigurationDialog();

    // pre-configuration
    if(_preConfigDialog != NULL)
    {
        if(groups.isEmpty())
            return false;

        SettingsNodeGroup *pG = groups.first();

        if(!_preConfigDialog->setAllValues(pG->groups()))
            return false;
    }

    // crée les résultats modèles d'entrée après avoir initialisé les paramètres des étapes
    // puisque certaines etapes modifie leur inResultModel après la pré-configuration !
    initInResultModelList();

    // input result configuration
    initInputResultConfiguration();

    groups = rootStep->groupsByTagName("InputResultConfigDialog");

    if(groups.isEmpty()
            || !_inManager->getResultModelConfigurationManager()->setAllValues(groups.first()->groups()))
        return false;

    groups = rootStep->groupsByTagName("PostConfigDialog");

    //cree la fenetre de pre-configuration
    createPostConfigurationDialog();

    // post-configuration
    if(_postConfigDialog != NULL)
    {
        if(groups.isEmpty())
            return false;

        SettingsNodeGroup *pG = groups.first();

        if(!_postConfigDialog->setAllValues(pG->groups()))
            return false;
    }

    return true;
}

bool CT_VirtualAbstractStep::appendStep(CT_VirtualAbstractStep *step)
{
    return insertStep(_stepChildList.size(), step);
}

bool CT_VirtualAbstractStep::insertStep(int n, CT_VirtualAbstractStep *step)
{
    if(step->acceptAddAfterThisStep(this))
    {
        if(n >= 0)
        {
            step->setParentStep(this);
            _stepChildList.insert(n, step);
            step->setParentStep(this);
        }
        else
        {
            bool ok = true;

            QListIterator<CT_VirtualAbstractStep*> it(_stepChildList);

            while(ok
                  && it.hasNext())
                ok = it.next()->acceptAddAfterThisStep(step);

            if(!ok)
                return false;

            it.toFront();

            while(it.hasNext())
                step->appendStep(it.next());

            _stepChildList.clear();
            _stepChildList.append(step);
        }

        return true;
    }

    return false;
}

bool CT_VirtualAbstractStep::removeStep(CT_VirtualAbstractStep *step)
{
    if(_stepChildList.removeOne(step))
    {
        delete step;
        return true;
    }

    return false;
}

QList<CT_VirtualAbstractStep *> CT_VirtualAbstractStep::getStepChildList() const
{
    return _stepChildList;
}

CT_VirtualAbstractStep* CT_VirtualAbstractStep::parentStep() const
{
    return _data->getParentStep();
}

CT_VirtualAbstractStep* CT_VirtualAbstractStep::rootStep() const
{
    CT_VirtualAbstractStep *parent = const_cast<CT_VirtualAbstractStep*>(this);

    while(parent->parentStep() != NULL)
        parent = parent->parentStep();

    return parent;
}

bool CT_VirtualAbstractStep::acceptAddAfterThisStep(const CT_VirtualAbstractStep *step)
{
    return _inManager->getResultModelManager()->isAllOutputModelFoundFromStep(step);
}

bool CT_VirtualAbstractStep::needInputResults() const
{
    return _inManager->getResultModelManager()->needInputResults();
}

bool CT_VirtualAbstractStep::isRunning() const
{
    return _running;
}

bool CT_VirtualAbstractStep::isStopped() const
{
    return _stop;
}

bool CT_VirtualAbstractStep::isPaused() const
{
    return _paused;
}

bool CT_VirtualAbstractStep::isInManualMode() const
{
    return !m_mustQuitManualMode;
}

bool CT_VirtualAbstractStep::mustBeRestarted() const
{
    return (isSettingsModified()
            || (nResult() == 0)
            || ((nResult() > 0)
                && (getResult(0)->isClearedFromMemory())));
}

bool CT_VirtualAbstractStep::mustRecheckTree() const
{
    return false;
}

void CT_VirtualAbstractStep::stop()
{
    _stop = true;
}

void CT_VirtualAbstractStep::quitManualMode()
{
    PS_LOG->addMessage(LogInterface::info, LogInterface::step, getStepName() + tr(" quit manual mode"));

    if(getGuiContext() != NULL)
        useManualMode(true);

    m_mustQuitManualMode = true;
}

void CT_VirtualAbstractStep::unPause()
{
    _paused = false;
}

void CT_VirtualAbstractStep::aboutToBeDeleted()
{
    delete _preConfigDialog;
    _preConfigDialog = NULL;

    delete _postConfigDialog;
    _postConfigDialog = NULL;

    _inManager->aboutToBeDeleted();
}

void CT_VirtualAbstractStep::setDebugModeOn(bool enable)
{
    if(!isRunning())
        _debugMode = enable;
    else if(!enable)
        _index_step_step_to_go = -1;
}

bool CT_VirtualAbstractStep::isDebuggable() const
{
    return _debuggable;
}

bool CT_VirtualAbstractStep::isManual() const
{
    return m_manual;
}

bool CT_VirtualAbstractStep::isModifiable() const
{
    return (m_manual && m_modifiable && (_inManager->getTurnManager()->nValidTurn() == 1));
}

int CT_VirtualAbstractStep::getExecuteTime() const
{
    return _elapsed;
}

int CT_VirtualAbstractStep::getProgress() const
{
    return _progress;
}

int CT_VirtualAbstractStep::getErrorCode() const
{
    return _error_code;
}

QString CT_VirtualAbstractStep::getErrorMessage(int error_code) const
{
    if(error_code != 0)
    {
        QString errMess = _errorMessageMap.value(error_code, "");

        if(!errMess.isEmpty())
            return errMess;

        return tr("Erreur %1").arg(error_code);
    }

    return tr("Aucune erreur detecte");
}

QString CT_VirtualAbstractStep::getToolTip() const
{
    // TODO
    /*QVariantList settings = getAllSettings();
    QListIterator<QVariant> it(settings);
    QListIterator<QList<Result*> > itRes(_inResultManager->getAllInputResults());

    QString str = getStepName();

    if(itRes.hasNext())
    {
        QListIterator<Result*> itRes2(itRes.next());

        if (itRes2.hasNext())
        {
            str += QString("\nInRes= %1").arg(itRes2.next()->getId());

            while(itRes2.hasNext())
            {
                str += QString(",%1").arg(itRes2.next()->getId());
            }

        }

        while(itRes.hasNext())
        {
            QListIterator<Result*> itRes2(itRes.next());

            if (itRes2.hasNext())
            {
                str += QString(" ; %1").arg(itRes2.next()->getId());

                while(itRes2.hasNext())
                {
                    str += QString(",%1").arg(itRes2.next()->getId());
                }

            }
        }
    }

    if(it.hasNext())
    {
        str += "\nParam: " + it.next().toString();

        while(it.hasNext())
        {
            str += "/" + it.next().toString();
        }
    }

    return str;*/

    return "TODO !";
}

QString CT_VirtualAbstractStep::getStepName() const
{
    return metaObject()->className();
}

QString CT_VirtualAbstractStep::getStepExtendedName() const
{
    return getStepName() + " (" + QString().setNum(uniqueID()) + ")";
}

QString CT_VirtualAbstractStep::getStepCustomName() const
{
    if(_customName.isEmpty())
        return getStepName();

    return _customName;
}

void CT_VirtualAbstractStep::setStepCustomName(QString customName)
{
    if(_customName != customName)
    {
        _customName = customName;
        emit nameChanged();
    }
}

QString CT_VirtualAbstractStep::getStepDescription() const
{
    return tr("No description for this step");
}

QString CT_VirtualAbstractStep::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

int CT_VirtualAbstractStep::nResult() const
{
    return _outManager->getResultManager()->nResults();
}

QList<CT_ResultGroup *> CT_VirtualAbstractStep::getResults() const
{
    QList<CT_ResultGroup*> ret;

    QList< QList<CT_AbstractResult*>* > list = _outManager->getResultManager()->getResultsForAllTurn();

    QListIterator< QList<CT_AbstractResult*>* > it(list);

    while(it.hasNext())
    {
        QListIterator<CT_AbstractResult*> it2(*it.next());

        while(it2.hasNext())
            ret.append((CT_ResultGroup*)it2.next());
    }

    return ret;
}

CT_ResultGroup* CT_VirtualAbstractStep::getResult(int n) const
{
    return (CT_ResultGroup*)_outManager->getResultManager()->getResult(n);
}

void CT_VirtualAbstractStep::clearOutResult()
{
    setClearMemoryProgress(0);

    int i = 0;
    int size = nResult();

    if(size > 0)
    {
        while(removeResult(0))
        {
            ++i;

            setClearMemoryProgress((i*100)/size);
        }
    }

    _outManager->resultCleared();

    setClearMemoryProgress(100);
}

void CT_VirtualAbstractStep::clearOutResultFromMemory()
{
    setClearMemoryProgress(0);

    int size = nResult();

    if(size > 0)
    {
        for(int i=0; i<size; ++i)
        {
            CT_ResultGroup *res = getResult(i);

            emit resultToBeClearedFromMemory(res);

            res->setBusy(true);
            res->clearFromMemory();
            res->setBusy(false);

            setClearMemoryProgress(((i+1)*100)/size);
        }
    }

    setClearMemoryProgress(100);
}

void CT_VirtualAbstractStep::recursiveClearOutResult()
{
    recursiveClearChildOutResult();
    clearOutResult();
}

void CT_VirtualAbstractStep::recursiveClearChildOutResult()
{
    QListIterator<CT_VirtualAbstractStep*> it(_stepChildList);

    while(it.hasNext())
        it.next()->recursiveClearOutResult();
}

void CT_VirtualAbstractStep::recursiveClearOutResultFromMemory()
{
    QListIterator<CT_VirtualAbstractStep*> it(_stepChildList);

    while(it.hasNext())
        it.next()->recursiveClearOutResultFromMemory();

    clearOutResultFromMemory();
}

bool CT_VirtualAbstractStep::isSettingsModified() const
{
    return _isSettingsModified;
}

bool CT_VirtualAbstractStep::isDebugModeOn() const
{
    return _debugMode;
}

SettingsNodeGroup* CT_VirtualAbstractStep::getSerializedInformation() const
{
    SettingsNodeGroup *root = new SettingsNodeGroup("SerializedInformation");

    root->addValue(new SettingsNodeValue("Version", "1"));
    root->addValue(new SettingsNodeValue("RelativeFileName", _serializedResultsRelativeFilename));
    root->addValue(new SettingsNodeValue("NSerialized", QString().setNum(_nResultSerialized)));

    return root;
}

bool CT_VirtualAbstractStep::setSerializedInformation(const SettingsNodeGroup *settings)
{
    if(settings == NULL)
        return false;

    QList<SettingsNodeValue*> values = settings->valuesByTagName("RelativeFileName");

    if(values.isEmpty())
        return false;

    QString tmp = values.first()->value().toString();

    values = settings->valuesByTagName("NSerialized");

    if(values.isEmpty())
        return false;

    bool ok;
    int tmpI = values.first()->value().toInt(&ok);

    if(!ok)
        return false;

    _serializedResultsRelativeFilename = tmp;
    _nResultSerialized = tmpI;

    return true;
}

int CT_VirtualAbstractStep::getNumberResultSerialized() const
{
    return _nResultSerialized;
}

QString CT_VirtualAbstractStep::getRelativeFilepathOfSerializedResults() const
{
    return _serializedResultsRelativeFilename;
}

void CT_VirtualAbstractStep::execute()
{
    runProcessing();
}

void CT_VirtualAbstractStep::executeModify()
{
    if(isModifiable())
        runProcessing(true);
}

bool CT_VirtualAbstractStep::showInputResultConfigurationDialog()
{
    return configureInputResult();
}

bool CT_VirtualAbstractStep::showPreConfigurationDialog()
{
    //cree la fenetre de pre-configuration
    createPreConfigurationDialog();

    return preConfigure();
}

bool CT_VirtualAbstractStep::showPostConfigurationDialog()
{
    // cree la fenetre de post-configuration
    createPostConfigurationDialog();

    return postConfigure();
}

void CT_VirtualAbstractStep::ackDebugMode(int jump_n_step)
{
    _index_step_step_to_go += jump_n_step;
    _ackOfDebugMode = true;
}

void CT_VirtualAbstractStep::setGuiContext(const GuiContextInterface *context)
{
    m_guiContext = (GuiContextInterface*)context;
}

void CT_VirtualAbstractStep::ackManualMode()
{
    if(m_firstCallToManualMode)
        PS_LOG->addMessage(LogInterface::info, LogInterface::step, getStepName() + tr(" enter manual mode"));

    if(m_firstCallToManualMode
            && getGuiContext() != NULL)
    {
        initManualMode();
        m_firstCallToManualMode = false;
    }

    if(getGuiContext() != NULL)
        useManualMode();
}

////////// PROTECTED ////////

// RESULTAT ET ITEMDRAWABLE EN ENTREE //

bool CT_VirtualAbstractStep::addInResultModel(CT_InAbstractResultModel *model)
{
    if(!_inManager->getResultModelManager()->addResultModel(model))
    {
        qFatal(QString("Erreur lors de l'ajout d'un CT_InAbstractResultModel dans l'étape %1").arg(getStepName()).toLatin1());
        return false;
    }

    model->setStep(this);

    return true;
}

CT_InResultModelGroup* CT_VirtualAbstractStep::createNewInResultModel(const QString &uniqueName,
                                                                      const QString &displayableName,
                                                                      const QString &description,
                                                                      bool recursive)
{
    if(uniqueName.isEmpty())
        return NULL;

    CT_InResultModelGroup *model = new CT_InResultModelGroup(uniqueName,
                                                             NULL,
                                                             displayableName,
                                                             description,
                                                             recursive);

    if(addInResultModel(model))
        return model;

    delete model;
    return NULL;
}

CT_InResultModelGroupToCopy *CT_VirtualAbstractStep::createNewInResultModelForCopy(const QString &uniqueName,
                                                                                   const QString &displayableName,
                                                                                   const QString &description,
                                                                                   bool recursive)
{
    if(uniqueName.isEmpty())
        return NULL;

    CT_InResultModelGroupToCopy *model = new CT_InResultModelGroupToCopy(uniqueName,
                                                                         NULL,
                                                                         displayableName,
                                                                         description,
                                                                         recursive);

    if(addInResultModel(model))
        return model;

    delete model;
    return NULL;
}

bool CT_VirtualAbstractStep::setNotNeedInputResult()
{
    CT_InResultModelNotNeedInputResult *model = new CT_InResultModelNotNeedInputResult();

    if(addInResultModel(model))
        return true;

    delete model;
    return false;
}

bool CT_VirtualAbstractStep::setMaximumTurn(int n)
{
    if(n <= 0)
        return false;

    _inManager->getTurnManager()->setMaximumTurn(n);
    return true;
}

int CT_VirtualAbstractStep::maximumTurn() const
{
    return _inManager->getTurnManager()->maximumTurn();
}

int CT_VirtualAbstractStep::currentTurnIndex() const
{
    return _inManager->getTurnManager()->getTurnIndexManager()->currentTurnIndex();
}

CT_InTurnManager* CT_VirtualAbstractStep::getInTurnManager() const
{
    return _inManager->getTurnManager();
}

CT_InAbstractResultModel* CT_VirtualAbstractStep::getInResultModel(const QString &uniqueName) const
{
    return _inManager->getTurnManager()->getInResultModel(uniqueName);
}

CT_InAbstractModel* CT_VirtualAbstractStep::getInModelForResearch(const CT_OutAbstractResultModel *inResultModel,
                                                                      const QString &uniqueName) const
{
    assert(inResultModel != NULL);

    return _inManager->getTurnManager()->getInModel(*inResultModel, uniqueName);
}

CT_InAbstractModel* CT_VirtualAbstractStep::getInModelForResearch(const CT_ResultGroup *inResult,
                                                                      const QString &uniqueName) const
{
    assert(inResult != NULL);

    return getInModelForResearch(inResult->model(), uniqueName);
}

CT_InAbstractModel* CT_VirtualAbstractStep::getInModelForResearch(const QString &inResultUniqueName,
                                                                  const int &possibilitySelectedIndex,
                                                                  const QString &uniqueName) const
{
    CT_InAbstractResultModel *inResModel = getInResultModel(inResultUniqueName);
    assert(inResModel != NULL);

    QList<CT_InStdModelPossibility*> p = inResModel->getPossibilitiesSavedSelected();
    assert((possibilitySelectedIndex > 0) && (possibilitySelectedIndex < p.size()));

    return getInModelForResearch((CT_OutAbstractResultModel*)p.at(possibilitySelectedIndex)->outModel(), uniqueName);
}

QList<CT_ResultGroup*> CT_VirtualAbstractStep::getInputResults() const
{
    QList<CT_ResultGroup*> ret;
    QList<CT_AbstractResult*> list = _inManager->getResultManager()->getResultsForCurrentTurn();

    while(!list.isEmpty())
        ret.append(dynamic_cast<CT_ResultGroup*>(list.takeFirst()));

    return ret;
}

QList<CT_InResultsByModel*> CT_VirtualAbstractStep::getInputResultsSortedByModels() const
{
    return _inManager->getResultManager()->getResultsSortedByModelsForCurrentTurn();
}

QList<CT_ResultGroup*> CT_VirtualAbstractStep::getInputResultsForModel(const CT_InAbstractResultModel *resultModel) const
{
    QList<CT_ResultGroup*> outList;

    QList<CT_InResultsByModel*> inResultByModels = getInputResultsSortedByModels();
    QListIterator<CT_InResultsByModel*> it(inResultByModels);

    while(it.hasNext())
    {
        CT_InResultsByModel *resM = it.next();

        // si ce modèle correspond à celui à exporter
        if(resM->getInModel() == resultModel)
        {
            outList.append((const QList<CT_ResultGroup*>&)resM->getResults());
            return outList;
        }
    }

    return outList;
}

QList<CT_ResultGroup*> CT_VirtualAbstractStep::getInputResultsForModel(const QString &inResultUniqueName) const
{
    return getInputResultsForModel(getInResultModel(inResultUniqueName));
}

bool CT_VirtualAbstractStep::isCreateDefaultOutModelActive() const
{
    return _defaultOutModelCreating;
}

// RESULTAT ET ITEMDRAWABLE EN SORTIE //

bool CT_VirtualAbstractStep::addOutResultModel(CT_OutAbstractResultModelGroup *model)
{
    if(!_outManager->getResultModelManager()->addResultModel(model))
    {
        qFatal(QString("Erreur lors de l'ajout d'un CT_OutResultModelGroup dans l'étape %1").arg(getStepName()).toLatin1());
        return false;
    }

    model->setStep(this);

    return true;
}

CT_OutResultModelGroup* CT_VirtualAbstractStep::createNewOutResultModel(const QString &uniqueName,
                                                                        const QString &resultName,
                                                                        const QString &displayableName,
                                                                        const QString &description)
{
    QString mn = uniqueName;

    if(mn.isEmpty())
        mn = CT_GenerateOutResultModelName().getNewResultModelNameThatDontExistIn(*_outManager->getResultModelManager());

    CT_OutResultModelGroup *model = new CT_OutResultModelGroup(mn,
                                                               NULL,
                                                               resultName,
                                                               displayableName,
                                                               description);

    if(addOutResultModel(model))
        return model;

    delete model;
    return NULL;
}

CT_OutResultModelGroupToCopyPossibilities* CT_VirtualAbstractStep::createNewOutResultModelToCopy(const QString &inUniqueName)
{
    if(inUniqueName.isEmpty())
        return NULL;

    CT_InResultModelGroupToCopy *inModel = dynamic_cast<CT_InResultModelGroupToCopy*>(getInResultModel(inUniqueName));

    if(inModel == NULL)
        return NULL;

    CT_GenerateOutResultModelName gen;

    CT_OutResultModelGroupToCopyPossibilities *outModel = new CT_OutResultModelGroupToCopyPossibilities();

    // on récupère toutes les possibilités que l'utilisateur à défini
    QList<CT_OutResultModelGroupToCopyPossibility*> copyList = inModel->getOutResultModelGroupsSelectedToCopy();
    QListIterator<CT_OutResultModelGroupToCopyPossibility*> it(copyList);

    // pour chaque possibilité à copier
    while(it.hasNext())
    {
        CT_OutResultModelGroupToCopyPossibility *outResModelToCopy = it.next();
        QString newResultModelName = gen.getNewResultModelNameThatDontExistIn(*_outManager->getResultModelManager());

        CT_OutResultModelGroupCopy *rModel = new CT_OutResultModelGroupCopy(newResultModelName,
                                                                            outResModelToCopy);


        // on crée un nouveau résultat de sortie contenant le résultat modèle modifié
        if(!addOutResultModel(rModel))
        {
            delete rModel;

            delete outModel;
            return NULL;
        }

        rModel->setInResultCopyModel(inModel);

        outModel->addResulModel(rModel);
    }

    return outModel;
}

CT_OutTurnManager* CT_VirtualAbstractStep::getOutTurnManager() const
{
    return _outManager->getTurnManager();
}

bool CT_VirtualAbstractStep::addOutResultModelCopy(CT_InResultModelGroupToCopy *inModel,
                                                   const QList<CT_AbstractOutModelCopyAction*> &actions)
{
    if(inModel == NULL)
        return false;

    CT_GenerateOutResultModelName gen;

    // on récupère toutes les possibilités que l'utilisateur à défini
    QList<CT_OutResultModelGroupToCopyPossibility*> copyList = inModel->getOutResultModelGroupsSelectedToCopy();
    QListIterator<CT_OutResultModelGroupToCopyPossibility*> it(copyList);

    // pour chaque possibilité à copier
    while(it.hasNext())
    {
        CT_OutResultModelGroupToCopyPossibility *outResModelToCopy = it.next();

        QListIterator<CT_AbstractOutModelCopyAction*> itA(actions);

        while(itA.hasNext())
        {
            if(!itA.next()->execute(this, outResModelToCopy))
                return false;
        }

        QString newResultModelName = gen.getNewResultModelNameThatDontExistIn(*_outManager->getResultModelManager());

        // on crée un nouveau résultat de sortie contenant le résultat modèle modifié
        if(!addOutResultModel(new CT_OutResultModelGroupCopy(newResultModelName,
                                                             outResModelToCopy)))
            return false;
    }

    return true;
}

CT_OutAbstractResultModel* CT_VirtualAbstractStep::getOutResultModel(const QString &uniqueName) const
{
    return _outManager->getTurnManager()->getOutResultModel(uniqueName);
}

CT_OutAbstractModel* CT_VirtualAbstractStep::getOutModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const
{
    if((outResult == NULL)
            ||(outResult->model() == NULL))
        return NULL;

    return _outManager->getTurnManager()->getOutModel(*outResult->model(), uniqueName);
}

CT_OutAbstractSingularItemModel *CT_VirtualAbstractStep::getOutSingularItemModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const
{
    return (CT_OutAbstractSingularItemModel*)getOutModelForCreation(outResult, uniqueName);
}

CT_OutAbstractGroupModel *CT_VirtualAbstractStep::getOutGroupModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const
{
    return (CT_OutAbstractGroupModel*)getOutModelForCreation(outResult, uniqueName);
}

CT_OutAbstractItemAttributeModel *CT_VirtualAbstractStep::getOutItemAttributeModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const
{
    return (CT_OutAbstractItemAttributeModel*)getOutModelForCreation(outResult, uniqueName);
}

QList<CT_ResultGroup*> CT_VirtualAbstractStep::getOutResultList() const
{
    QList<CT_ResultGroup*> ret;

    QList<CT_AbstractResult*> list = _outManager->getResultManager()->getResultsForCurrentTurn();
    QListIterator<CT_AbstractResult*> it(list);

    while(it.hasNext())
    {
        ret.append(dynamic_cast<CT_ResultGroup*>(it.next()));
    }

    return ret;
}

// CONFIGURATION //

CT_StepConfigurableDialog* CT_VirtualAbstractStep::newStandardPreConfigurationDialog()
{
    delete _preConfigDialog;
    _preConfigDialog = new CT_StepConfigurableDialog();
    _preConfigDialog->setWindowTitle(_preConfigDialog->windowTitle() + QString(" (%1)").arg(_customName.isEmpty() ? getStepExtendedName() : _customName));
    return _preConfigDialog;
}

bool CT_VirtualAbstractStep::preConfigure()
{
    if(_preConfigDialog != NULL)
    {
        if(_preConfigDialog->exec() == 1)
        {
            setSettingsModified(_preConfigDialog->isSettingsModified());

            return true;
        }

        return false;
    }

    return true;
}

bool CT_VirtualAbstractStep::configureInputResult()
{
    bool firstTime = (_inManager->getTurnManager()->nTurn() == 0);

    initInputResultConfiguration();

    CT_InResultModelConfigurationManager::CreateDialogReturn crReturn = CT_InResultModelConfigurationManager::CreateError;

    // on crée la fenêtre de configuration des résultats d'entrée si nécessaire
    if((crReturn = _inManager->getResultModelConfigurationManager()->createInResultModelConfigurationDialog()) == CT_InResultModelConfigurationManager::CreateError)
    {
        qFatal("Erreur lors de la creation de la fenetre de configuration des resultats d'entree");
    }

    if(!_stepChildList.isEmpty()
            || (!firstTime && crReturn == CT_InResultModelConfigurationManager::CreateNotNecessary))
    {
        /*QMessageBox::warning(NULL, "Impossible", "Vous ne pouvez pas re-configurer les résultats d'entrées "
                                                 "puisque vous possédez des étapes filles. La re-configuration peut "
                                                 "entrainer une modification potentielle des résultats de sortie qui pourrait ainsi créer "
                                                 "des erreurs de traitement pour les étapes filles. Veuillez supprimer ces dernières pour "
                                                 "re-configurer les résultats d'entrées.", QMessageBox::Ok);*/
        _inManager->getResultModelConfigurationManager()->showReadOnlyInResultModel();
    }
    else
    {
        // on affiche la fenêtre de configuration si nécessaire ou on configure par défaut les modèles d'entrées
        CT_InResultModelConfigurationManager::ConfigureReturn cReturn = _inManager->getResultModelConfigurationManager()->configureInResultModel();

        // si il y a eu des modifications
        if(cReturn == CT_InResultModelConfigurationManager::ConfigureSuccess)
        {
            setSettingsModified(true);
            return true;
        }
        // sinon si il n'y en a pas eu
        else if(cReturn == CT_InResultModelConfigurationManager::ConfigureNotChanged)
        {
            return true;
        }
        // sinon si il y a eu une erreur
        else if(cReturn == CT_InResultModelConfigurationManager::ConfigureError)
        {
            qFatal("Erreur lors de la creation des tours");
        }
    }

    return false;
}

CT_StepConfigurableDialog* CT_VirtualAbstractStep::newStandardPostConfigurationDialog()
{
    delete _postConfigDialog;
    _postConfigDialog = new CT_StepConfigurableDialog();
    _postConfigDialog->setWindowTitle(_postConfigDialog->windowTitle() + QString(" (%1)").arg(_customName.isEmpty() ? getStepExtendedName() : _customName));

    return _postConfigDialog;
}

bool CT_VirtualAbstractStep::postConfigure()
{
    if(_postConfigDialog != NULL)
    {
        if(_postConfigDialog->exec() == 1)
        {
            setSettingsModified(_postConfigDialog->isSettingsModified());

            return true;
        }

        return false;
    }

    return true;
}

void CT_VirtualAbstractStep::setSettingsModified(bool modified)
{
    if(_isSettingsModified != modified)
    {
        _isSettingsModified = modified;
    }

    emit settingsModified();
}

// TRAITEMENT ET MODE DEBUG //

void CT_VirtualAbstractStep::setProgress(float progress)
{
    if(((int)progress) != _progress)
    {
        _progress = (int)progress;
        _elapsed = _execute_time.elapsed();

        emit inProgress(_progress);
        emit elapsed(_elapsed);
    }
}

void CT_VirtualAbstractStep::setErrorCode(int errorCode)
{
    _error_code = errorCode;
}

void CT_VirtualAbstractStep::setErrorMessage(int errorCode, const QString &errorMessage)
{
    _errorMessageMap.insert(errorCode, errorMessage);
}

void CT_VirtualAbstractStep::clearErrorMessage()
{
    _errorMessageMap.clear();
}

void CT_VirtualAbstractStep::pauseProcessing()
{
    if(!_paused)
    {
        _paused = true;

        emit paused();

        QMutex mutex;

        while((!_paused)
                && (!_stop))
        {
            mutex.lock();
            QWaitCondition().wait(&mutex, 100);
            mutex.unlock();
        }
    }
}

void CT_VirtualAbstractStep::setDebuggable(bool debuggable)
{
    _debuggable = debuggable;
}

void CT_VirtualAbstractStep::waitForAckIfInDebugMode()
{
    if(_debugMode)
    {
        if(_step_by_step_size == _index_step_step_to_go)
        {
            _inManager->getResultManager()->setResultsOfCurrentTurnBusy(false);

            // preWaitForAckIfInDebugMode() et postWaitForAckIfInDebugMode() sont appelé
            // par le GUI

            _ackOfDebugMode = false;
            emit waitForAckInDebugMode();

            QMutex mutex;

            while((_index_step_step_to_go != -1)
                    && (!_ackOfDebugMode)
                    && (!_stop))
            {
                mutex.lock();
                QWaitCondition().wait(&mutex, 100);
                mutex.unlock();
            }

            _inManager->getResultManager()->setResultsOfCurrentTurnBusy(true);
        }

        ++_step_by_step_size;
    }
}

int CT_VirtualAbstractStep::stepByStepSize() const
{
    return currentDebugBreakPointNumber();
}

int CT_VirtualAbstractStep::currentDebugBreakPointNumber() const
{
    return _step_by_step_size;
}

void CT_VirtualAbstractStep::setModifiable(bool modifiableOn)
{
    m_modifiable = modifiableOn;
}

void CT_VirtualAbstractStep::setManual(bool manualModeOn)
{
    m_manual = manualModeOn;
}

void CT_VirtualAbstractStep::requestManualMode()
{    
    if(m_manual)
    {
        m_mustQuitManualMode = false;
        _inManager->getResultManager()->setResultsOfCurrentTurnBusy(false);

        emit manualModeRequired();

        QMutex mutex;

        while(!m_mustQuitManualMode
                && (!isStopped()))
        {
            mutex.lock();
            QWaitCondition().wait(&mutex, 100);
            mutex.unlock();
        }

        _inManager->getResultManager()->setResultsOfCurrentTurnBusy(true);

        emit manualModeCompleted();
    }
}

void CT_VirtualAbstractStep::initManualMode()
{
}

void CT_VirtualAbstractStep::useManualMode(bool quit)
{
    Q_UNUSED(quit)
}

GuiContextInterface* CT_VirtualAbstractStep::getGuiContext() const
{
    return m_guiContext;
}

int CT_VirtualAbstractStep::getNTurnToCompute() const
{
    return _inManager->getTurnManager()->nValidTurn();
}

int CT_VirtualAbstractStep::getCurrentTurn() const
{
    return _turnIndexManager->currentTurnIndex();
}

void CT_VirtualAbstractStep::setClearMemoryProgress(float progress)
{
    if(((int)progress) != _progressClearMem)
    {
        _progressClearMem = (int)progress;

        emit cleanMemoryInProgress(_progressClearMem);
    }
}

////////// PRIVATE ////////

void CT_VirtualAbstractStep::initInputResultConfiguration()
{
    // si on a pas encore configurer les tours d'entrées
    if(_inManager->getTurnManager()->nTurn() == 0)
    {
        // on recherche les possibilités de résultats à partir de l'étape parente
        if(!_inManager->getResultModelManager()->findAllOutputModelFromStepAndSavePossibilities(parentStep()))
        {
            qFatal("Erreur lors de la creation des possibilités");
        }
    }
}

void CT_VirtualAbstractStep::createInResultModelList()
{
    if(_inManager->getResultModelManager()->isEmpty())
    {
        PS_MODELS->clearCache();
        _canChangeSetOneCompute = true;
        createInResultModelListProtected();
        _canChangeSetOneCompute = false;
    }
}

void CT_VirtualAbstractStep::setParentStep(CT_VirtualAbstractStep *parent)
{
    _data->setParentStep(parent);
}

void CT_VirtualAbstractStep::setLaunchTime(QDateTime launchTime)
{
    _launch_time = launchTime;
}

void CT_VirtualAbstractStep::runProcessing(bool modificationMode)
{
    PS_LOG->addMessage(LogInterface::trace, LogInterface::step,  getStepName() + tr(" - Start computing"));

    _running = true;
    _stop = false;

    if(modificationMode) // supprime tous les anciens resultats des etapes enfants
        recursiveClearChildOutResult();
    else // supprime tous les anciens resultats ainsi que ceux des etapes enfants
        recursiveClearOutResult();

    // en fonction des resultats modeles configurer on cree les resultats à utiliser
    bool canCompute = _inManager->getResultManager()->initResultsFromStep(parentStep());

    // debut du lancement
    setLaunchTime(QDateTime::currentDateTime());
    _step_by_step_size = 0;
    _index_step_step_to_go = 0;
    setErrorCode(-2);

    _execute_time.start();
    setProgress(0);

    int nTurn = getNTurnToCompute();
    _turnIndexManager->resetTurnIndex();

    while((getCurrentTurn() < nTurn)
            && canCompute)
    {
        m_firstCallToManualMode = true;

        PS_MODELS->clearCache();

        // en fonction des resultats modeles de sortie on cree les resultats de sortie que l'etape
        // pourra utiliser dans sa methode compute.
        if(!modificationMode)
            canCompute = _outManager->getResultManager()->initResultsForCurrentTurn();

        if(canCompute)
        {
            // crée la liste de recherche des modèles d'entrée pour le tour courant
            canCompute = _inManager->getTurnManager()->createSearchModelListForCurrentTurn();

            if(canCompute)
            {
                // crée la liste de recherche des modèles de sortie pour le tour courant
                canCompute = _outManager->getTurnManager()->createSearchModelListForCurrentTurn();

                if(canCompute)
                {
                    if(_inManager->getResultModelManager()->needInputResults())
                        canCompute = _inManager->getResultManager()->existResultsForCurrentTurn();

                    if(canCompute)
                    {
                        setErrorCode(0);

                        _inManager->getResultManager()->setResultsOfCurrentTurnBusy(true);

                        // le traitement lui-meme
                        if(modificationMode)
                            modify();
                        else
                            compute();

                        _inManager->getResultManager()->setResultsOfCurrentTurnBusy(false);

                        // si l'utilisateur a stopper le traitement
                        if(isStopped())
                        {
                            // on supprime tous les resultats genere
                            setProgress(0);
                            clearOutResult();
                        }
                        else if(getErrorCode() == 0)
                        {
                            // on peut dire au ResultManager que les résultats du tour courant sont OK
                            if(!modificationMode)
                                _outManager->getResultManager()->completesResultsOfCurrentTurn();

                            setSettingsModified(false);
                            setProgress(100);
                        }
                    }
                }
            }
        }

        _turnIndexManager->incrementTurnIndex();
    }

    if((getCurrentTurn() > 0)
            && !canCompute)
    {
        setErrorCode(-3);
    }

    _turnIndexManager->resetTurnIndex();
    _running = false;

    PS_LOG->addMessage(LogInterface::trace, LogInterface::step,  getStepName() + tr(" - Computing completed"));

    emit isCompleted();
}


void CT_VirtualAbstractStep::setRelativeFilepathOfSerializedResults(QString path)
{
    _serializedResultsRelativeFilename = path;
}

void CT_VirtualAbstractStep::setNumberResultSerialized(int n)
{
    _nResultSerialized = n;
}

bool CT_VirtualAbstractStep::removeResult(int n)
{
    CT_ResultGroup *res = (CT_ResultGroup*)_outManager->getResultManager()->takeResult(n);

    if(res != NULL)
    {
        emit resultToBeClearedFromMemory(res);

        res->setBusy(true);
        res->clearFromMemory();
        res->setBusy(false);

        emit resultToBeRemoved(res);

        delete res;

        return true;
    }

    return false;
}

QList< QList<CT_OutAbstractResultModelGroup*> > CT_VirtualAbstractStep::getAllOutResultModels() const
{
    QList< QList<CT_OutAbstractResultModelGroup*> > ret;

    QListIterator<CT_OutTurn*> it(_outManager->getTurnManager()->turns());

    while(it.hasNext())
    {
        CT_OutTurn *turn = it.next();

        QList<CT_OutAbstractResultModelGroup*> ret2;
        QListIterator<CT_OutAbstractResultModel*> it2(turn->models());

        while(it2.hasNext())
        {
            CT_OutAbstractResultModelGroup *g = dynamic_cast<CT_OutAbstractResultModelGroup*>(it2.next());

            if(g == NULL)
            {
                ret.clear();
                return ret;
            }

            ret2.append(g);
        }

        ret.append(ret2);
    }

    return ret;
}

void CT_VirtualAbstractStep::setCreateDefaultOutModelActive(bool active)
{
    _defaultOutModelCreating = active;
}

CT_OutManager* CT_VirtualAbstractStep::getOutManager() const
{
    return _outManager;
}

////////// PROTECTED /////////

#ifdef USE_BOOST_OLD

#ifdef USE_BOOST_BINARY
bool CT_VirtualAbstractStep::recursiveSerializeResult(QString &dirPath, QString &relativeFilePath, boost::archive::binary_oarchive &ar, bool clearResultAfter)
#else
bool CT_VirtualAbstractStep::recursiveSerializeResult(QString &dirPath, QString &relativeFilePath, boost::archive::text_oarchive &ar, bool clearResultAfter)
#endif
{
    QListIterator<Result *> resultList = getResults();
    QListIterator<Result *> itResult(resultList);

    setNumberResultSerialized(0);
    setRelativeFilepathOfSerializedResults("");

    if(itResult.hasNext())
    {
        int nResultSaved = 0;

        while(!isStopped()
                && itResult.hasNext())
        {
            CT_AbstractResult *res = (CT_AbstractResult*)itResult.next();

            if(!res->isClearedFromMemory())
            {
                emit resultToBeSerialized(res);

                ar << res;

                ++nResultSaved;
            }
            else
            {
                return false;
            }
        }

        setNumberResultSerialized(nResultSaved);

        if(!isStopped())
        {
            // on defini le chemin vers les resultats et le nombre de resultats sauvegarder
            setRelativeFilepathOfSerializedResults(relativeFilePath);

            QList<Step*> stepChildList = getStepChildList();
            QListIterator<Step*> itStep(stepChildList);

            while(!isStopped()
                    && itStep.hasNext())
            {
                CT_VirtualAbstractStep *nextStep = dynamic_cast<CT_VirtualAbstractStep*>(itStep.next());

                if((nextStep == NULL) || !nextStep->recursiveSerializeResult(dirPath, relativeFilePath, ar, clearResultAfter))
                {
                    return false;
                }
            }

            if(!isStopped())
            {
                // si on doit supprimer les rsultats de la mmoire
                if(clearResultAfter)
                {
                    clearOutResultFromMemory();
                }

                return true;
            }
        }
    }

    return false;
}

#ifdef USE_BOOST_BINARY
bool CT_VirtualAbstractStep::recursiveLoadSerializedResult(boost::archive::binary_iarchive &ar)
#else
bool CT_VirtualAbstractStep::recursiveLoadSerializedResult(boost::archive::text_iarchive &ar)
#endif
{
    int n = getNumberResultSerialized();

    // si on a srialis aucun rsultat ce n'est pas possible et donc on arrete tout
    if(n == 0)
    {
        return false;
    }

    int i=0;

    while(!isStopped()
            && (i<n))
    {
        CT_AbstractResult *res;

        ar >> res;

        _outResultManager->addResult(res);

        ++i;
    }

    if(!isStopped())
    {
        setProgress(100);

        QList<Step *> stepChildList = getStepChildList();
        QListIterator<Step*> it(stepChildList);

        while(!isStopped()
                && it.hasNext())
        {
            CT_VirtualAbstractStep *nextStep = dynamic_cast<CT_VirtualAbstractStep*>(it.next());

            if(nextStep == NULL)
            {
                return false;
            }

            if(!nextStep->recursiveLoadSerializedResult(ar))
            {
                return false;
            }
        }
    }

    return !isStopped();
}

#endif

bool CT_VirtualAbstractStep::recursiveCanBeDeserialized(QString &dirPath)
{
#ifdef USE_BOOST_OLD

    int n = getNumberResultSerialized();

    if(n <= 0)
    {
        return false;
    }

    QList<Step *> stepChildList = getStepChildList();
    QListIterator<Step*> it(stepChildList);

    while(it.hasNext())
    {
        CT_VirtualAbstractStep *nextStep = dynamic_cast<CT_VirtualAbstractStep*>(it.next());

        if(nextStep == NULL)
        {
            return false;
        }

        if(!nextStep->recursiveCanBeDeserialized(dirPath))
        {
            return false;
        }
    }

    return true;
#else

    Q_UNUSED(dirPath)

    return false;
#endif
}
