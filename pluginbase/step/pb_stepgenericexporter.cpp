#include "pb_stepgenericexporter.h"

#include "ct_exporter/abstract/ct_abstractexporterattributesselection.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

#include "ct_itemdrawable/model/inModel/ct_instdsingularitemmodel.h"

#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/ct_loopcounter.h"

#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"

#include "ct_turn/inTurn/tools/ct_inturnmanager.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"


#include "ct_step/ct_stepinitializedata.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include <QFileDialog>

#define DEF_SearchInGroup           "g"
#define DEF_SearchInItemDrawable    "i"
#define DEF_SearchInResultToExport  "r"

#define DEFin_resCounter "resCounter"
#define DEF_inGroupCounter "GroupCounter"
#define DEF_inCounter "counter"

#define DEF_inresName "resName"
#define DEF_inGroupName "GroupName"
#define DEF_inName "Name"
#define DEF_inNameAtt "NameAtt"


PB_StepGenericExporter::PB_StepGenericExporter(CT_StepInitializeData &dataInit,
                                               const QString &pluginName,
                                               CT_AbstractExporter *exporter) : CT_AbstractStep(dataInit)
{
    _adaptative = false;
    _pluginName = pluginName;
    _exporter = exporter;
    _exporter->setMyStep(this);

    CT_AbstractExporterAttributesSelection *ex = dynamic_cast<CT_AbstractExporterAttributesSelection*>(_exporter);

    if(ex != NULL)
        ex->setSearchOnlyModels(true);

    _exportPath = ".";
    _exporterConfiguration = NULL;

    setStepCustomName(_exporter->getExporterCustomName());

    connect(_exporter, SIGNAL(exportInProgress(int)), this, SLOT(exportProgressChanged(int)));
    connect(this, SIGNAL(stopped()), _exporter, SLOT(cancel()), Qt::DirectConnection);
}

PB_StepGenericExporter::~PB_StepGenericExporter()
{
    clearExporterConfiguration();
    delete _exporter;
}

const CT_AbstractExporter *PB_StepGenericExporter::exporter() const
{
    return _exporter;
}

const QString& PB_StepGenericExporter::pluginName() const
{
    return _pluginName;
}

void PB_StepGenericExporter::init()
{
    _exporter->init();

    QSettings *settings = _data->getSettingsFile();

    if(settings != NULL)
    {
        settings->beginGroup(getStepName());

        _exportPath = settings->value("DefaultExportPath", ".").toString();

        settings->endGroup();
    }

    CT_AbstractStep::init();
}

QString PB_StepGenericExporter::getStepName() const
{
    return replaceBadCharacters(_exporter->getExporterName());
}

QString PB_StepGenericExporter::getStepDisplayableName() const
{
    return _exporter->getExporterName();
}

QString PB_StepGenericExporter::getStepDetailledDescription() const
{
    if(_exporter != NULL)
    {
        QString t = _exporter->toolTip();

        if(!t.isEmpty())
            return t;
    }

    QString tmp;

    const QList<FileFormat> &ext = _exporter->exportFormats();
    QListIterator<FileFormat> it(ext);

    while(it.hasNext())
    {
        FileFormat ff = it.next();
        QListIterator<QString> itS(ff.suffixes());
        while(itS.hasNext())
        {
            tmp += "*." + itS.next();
            if (itS.hasNext()) {tmp += " ";}
        }
    }

    return tr("Exporte un(des) fichier(s) de type :") + tmp;
}



SettingsNodeGroup* PB_StepGenericExporter::getAllSettings() const
{
    SettingsNodeGroup *root = CT_VirtualAbstractStep::getAllSettings();
    SettingsNodeGroup *group = new SettingsNodeGroup(getStepName());
    group->addValue(new SettingsNodeValue("Version", "1"));
    group->addValue(new SettingsNodeValue("ExportPath", _exportFilename));
    group->addValue(new SettingsNodeValue("Adaptative", _adaptative));

    SettingsNodeGroup *confGroup = new SettingsNodeGroup("ExporterConfiguration");

    if(_exporterConfiguration != NULL)
    {
        _exporter->loadExportConfiguration(_exporterConfiguration);

        SettingsNodeGroup *exporterConfiguration = exporter()->saveExportConfiguration();

        if(exporterConfiguration != NULL)
            confGroup->addGroup(exporterConfiguration);
        else
            confGroup->addValue(new SettingsNodeValue("NOTHING"));
    }

    group->addGroup(confGroup);
    root->addGroup(group);

    return root;
}

bool PB_StepGenericExporter::setAllSettings(const SettingsNodeGroup *settings)
{
    if(!CT_VirtualAbstractStep::setAllSettings(settings))
        return false;

    QList<SettingsNodeGroup*> groups = settings->groupsByTagName(getStepName());

    if(groups.isEmpty())
        return false;

    QList<SettingsNodeValue*> values = groups.first()->valuesByTagName("ExportPath");

    if(values.isEmpty())
        return false;

    _exportFilename = values.first()->value().toString();

    values.clear();
    values = groups.first()->valuesByTagName("Adaptative");

    if(values.isEmpty())
        return false;

    _adaptative = values.first()->value().toBool();


    groups = groups.first()->groupsByTagName("ExporterConfiguration");

    if(groups.isEmpty())
        return false;

    if(groups.first()->groups().isEmpty())
        return !groups.first()->valuesByTagName("NOTHING").isEmpty();

    SettingsNodeGroup *exporterConfiguration = groups.first()->groups().first();

    configureExporterFromModel();

    if(_exporter->loadExportConfiguration(exporterConfiguration))
    {
        clearExporterConfiguration();
        _exporterConfiguration = _exporter->saveExportConfiguration();

        return true;
    }

    return false;
}

QString PB_StepGenericExporter::getStepDescription() const
{
    return ((CT_AbstractExporter*)_exporter)->getExporterCustomName();
}

CT_VirtualAbstractStep* PB_StepGenericExporter::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    CT_VirtualAbstractStep* stepCopy = new PB_StepGenericExporter(dataInit, pluginName(), exporter()->copy());
    stepCopy->setStepCustomName(getStepCustomName());
    return stepCopy;
}

void PB_StepGenericExporter::createPreConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPreConfigurationDialog();

    configDialog->addBool(tr("Nom adaptatif (dans une boucle)"), "", "", _adaptative);

}

//////////////////// PROTECTED //////////////////

void PB_StepGenericExporter::createInResultModelListProtected()
{
    CT_InResultModelGroup *resultModel = createNewInResultModel(DEF_SearchInResultToExport, tr("Résultat"), "", true);

    resultModel->setZeroOrMoreRootGroup();
    resultModel->addGroupModel("", DEF_SearchInGroup);

    if(!_exporter->exportOnlyGroup())
        resultModel->addItemModel(DEF_SearchInGroup, DEF_SearchInItemDrawable, CT_AbstractItemDrawable::staticGetType(), tr("Item"));


    if (_adaptative)
    {
        CT_InResultModelGroup* resCounter = createNewInResultModel(DEFin_resCounter, tr("Résultat compteur"), "", true);
        resCounter->setRootGroup(DEF_inGroupCounter);
        resCounter->addItemModel(DEF_inGroupCounter, DEF_inCounter, CT_LoopCounter::staticGetType(), tr("Compteur"));

        CT_InResultModelGroup* baseNameRes = createNewInResultModel(DEF_inresName, tr("Nom de base (optionnel)"), "", true);
        baseNameRes->setZeroOrMoreRootGroup();
        baseNameRes->addGroupModel("", DEF_inGroupName, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
        baseNameRes->addItemModel(DEF_inGroupName, DEF_inName, CT_AbstractSingularItemDrawable::staticGetType(), tr("Base Name"));
        baseNameRes->addItemAttributeModel(DEF_inName, DEF_inNameAtt, QList<QString>() << CT_AbstractCategory::DATA_FILE_NAME << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Name"));
        baseNameRes->setMinimumNumberOfPossibilityThatMustBeSelectedForOneTurn(0);
    }

    // un tour maximum !
    setMaximumTurn(1);
}

bool PB_StepGenericExporter::configureInputResult(bool forceReadOnly)
{
    if(forceReadOnly)
        return CT_VirtualAbstractStep::configureInputResult(forceReadOnly);

    if(CT_VirtualAbstractStep::configureInputResult(forceReadOnly))
        return configureExporter();

    return false;
}

bool PB_StepGenericExporter::postConfigure()
{
    // méthode appelée après avoir configuré les modèles d'entrées

    const QList<FileFormat> &list = ((CT_AbstractExporter*) _exporter)->exportFormats();

    QString extensions = "";
    QListIterator<FileFormat> it(list);
    while (it.hasNext())
    {
        FileFormat format = it.next();
        if (extensions.size() > 0) { extensions +=  ";;";}
        extensions += format.description();
        extensions += " (";
        QList<QString> suffixes = format.suffixes();
        QListIterator<QString> it2(suffixes);
        while (it2.hasNext())
        {
            extensions += "*.";
            extensions += it2.next();
        }
        extensions += ")";
    }

    QString exportFileName;
    if (_adaptative)
    {
        exportFileName = QFileDialog::getExistingDirectory(NULL, tr("Exporter dans..."), _exportFilename.isEmpty() ? _exportPath : _exportFilename);
    } else {
        exportFileName = QFileDialog::getSaveFileName(NULL, tr("Exporter sous..."), _exportFilename.isEmpty() ? _exportPath : _exportFilename, extensions);
    }

    if(exportFileName.isEmpty())
        return false;

    _exportFilename = exportFileName;

    QFileInfo info(exportFileName);
    setDefaultExportPath(info.absoluteDir().absolutePath());

    _exporter->setExportFilePath(_exportFilename);

    return true;
}

void PB_StepGenericExporter::createOutResultModelListProtected()
{
}

void PB_StepGenericExporter::compute()
{
    CT_ResultGroup* resIn_Counter = NULL;
    CT_ResultGroup* resIn_BaseName = NULL;
    QString rootBaseName = "";
    QString baseName = "";

    if (_adaptative)
    {
        QList<CT_ResultGroup*> inResultList = getInputResults();

        resIn_Counter = inResultList.at(1);
        if (inResultList.size() > 2)
        {
            resIn_BaseName = inResultList.at(2);

            CT_ResultItemIterator it0(resIn_BaseName, this, DEF_inName);
            if (it0.hasNext())
            {
                const CT_AbstractSingularItemDrawable* item = it0.next();
                CT_AbstractItemAttribute* att = item->firstItemAttributeByINModelName(resIn_BaseName, this, DEF_inNameAtt);
                QString tmp = att->toString(item, NULL);
                rootBaseName = QFileInfo(tmp).baseName();
            }
        }

        CT_ResultItemIterator it(resIn_Counter, this, DEF_inCounter);
        if (it.hasNext())
        {
            CT_LoopCounter* counter = (CT_LoopCounter*) it.next();
            baseName = counter->getTurnName();
            baseName = QFileInfo(baseName).baseName();
        }
    }


    // on récupère le modèle d'entrée à exporter
    CT_InAbstractModel *inItemModelToExport = NULL;

    // on récupère les résultats d'entrée classé par modèle d'entrée
    QList<CT_ResultGroup*> inResultByModels = getInputResultsForModel(DEF_SearchInResultToExport);
    QListIterator<CT_ResultGroup*> it(inResultByModels);

    QList<CT_AbstractItemDrawable*> itemsToExport;

    while(it.hasNext())
    {
        CT_ResultGroup *inRes = it.next();

        if(_exporter->exportOnlyGroup())
            inItemModelToExport = (CT_InAbstractModel*)PS_MODELS->searchModel(DEF_SearchInGroup, inRes, this);
        else
            inItemModelToExport = (CT_InAbstractModel*)PS_MODELS->searchModel(DEF_SearchInItemDrawable, inRes, this);

        // on recherche tous les ItemDrawable à exporter
        CT_ResultIterator it(inRes, inItemModelToExport);

        while(it.hasNext())
            itemsToExport.append((CT_AbstractItemDrawable*)it.next());
    }

    // une fois la liste constituée
    if(!itemsToExport.isEmpty())
    {
        _exporter->clearErrorMessage();

        if (_adaptative)
        {
            QString path = _exportFilename;
            path.append("/");
            path.append(rootBaseName);
            if (!rootBaseName.isEmpty())
            {
                path.append("_");
            }
            path.append(baseName);
            _exporter->setExportFilePath(path);

        } else {
            _exporter->setExportFilePath(_exportFilename);
        }

        // on la donne à l'exportateur
        if(!_exporter->setItemDrawableToExport(itemsToExport))
        {
            PS_LOG->addErrorMessage(this, _exporter->errorMessage());
            setErrorMessage(1, _exporter->errorMessage());
            setErrorCode(1);
        }
        else
        {
            //if(_exporterConfiguration != NULL)
                //_exporter->loadExportConfiguration(_exporterConfiguration);

            // et on exporte....
            _exporter->exportToFile();
        }
    }
}


bool PB_StepGenericExporter::configureExporter()
{
    configureExporterFromModel();

    if(_exporterConfiguration != NULL)
        _exporter->loadExportConfiguration(_exporterConfiguration);

    if(_exporter->configureExport())
    {
        clearExporterConfiguration();
        _exporterConfiguration = _exporter->saveExportConfiguration();

        setSettingsModified(true);

        return true;
    }

    return false;
}

void PB_StepGenericExporter::clearExporterConfiguration()
{
    delete _exporterConfiguration;
    _exporterConfiguration = NULL;
}

void PB_StepGenericExporter::configureExporterFromModel()
{
    // on se place au premier tour (il n'y en a qu'un de toute façon)
    getInTurnManager()->getTurnIndexManager()->resetTurnIndex();

    // on demande au gestionnaire de tour de créer la liste des modèles d'entrée pour le premier tour
    getInTurnManager()->createSearchModelListForCurrentTurn();

    // on récupère le modèle d'entrée qu'on avait défini
    CT_InResultModelGroup *resModel = (CT_InResultModelGroup*)getInResultModel(DEF_SearchInResultToExport);

    // on récupère les possibilités pour ce résultat (il n'y en a qu'une puisque un résultat d'entrée = un résultat de sortie)
    QList<CT_InStdModelPossibility*> possibilities = resModel->getPossibilitiesSavedSelected();

    QListIterator<CT_InStdModelPossibility*> it(possibilities);

    QList<CT_AbstractItemDrawable*> eItems;

    while(it.hasNext())
    {
        // on récupère le modèle d'entrée qu'on avait défini (celui à exporter)
        CT_InAbstractItemModel *inItemModelToExport = NULL;

        if(_exporter->exportOnlyGroup())
            inItemModelToExport = (CT_InAbstractItemModel*)PS_MODELS->searchModel(DEF_SearchInGroup, (CT_OutAbstractResultModel*)it.next()->outModel(), this);
        else
            inItemModelToExport = (CT_InAbstractItemModel*)PS_MODELS->searchModel(DEF_SearchInItemDrawable, (CT_OutAbstractResultModel*)it.next()->outModel(), this);

        // on récupère sa possibilité (il n'y en a qu'une puisqu'on a mis CT_InStandardItemDrawableModel::C_ChooseOneIfMultiple)
        CT_InStdModelPossibility *possibility = inItemModelToExport->getPossibilitiesSavedSelected().first();

        // on récupère l'ItemDrawable choisi
        eItems.append(((CT_OutAbstractItemModel*)possibility->outModel())->itemDrawable());
    }

    _exporter->clearErrorMessage();

    // on les donne à l'exporter
    _exporter->setItemDrawableToExport(eItems);
}

void PB_StepGenericExporter::setDefaultExportPath(const QString &path)
{
    _exportPath = path;

    QSettings *settings = _data->getSettingsFile();

    if(settings != NULL)
    {
        settings->beginGroup(getStepName());

        settings->setValue("DefaultExportPath", _exportPath);

        settings->endGroup();
    }
}

void PB_StepGenericExporter::exportProgressChanged(int progress)
{
    setProgress(progress);
}

QString PB_StepGenericExporter::replaceBadCharacters(const QString &name) const
{
    QString value = name;
    value.replace(QRegExp("[àáâãäå]"), "a");
    value.replace(QRegExp("[ÀÁÂÃÄÅ]"), "A");
    value.replace(QRegExp("[éèëê]"), "e");
    value.replace(QRegExp("[ÈÉÊË]"), "E");
    value.replace(QRegExp("[ìíîï]"), "i");
    value.replace(QRegExp("[ÌÍÎÏ]"), "I");
    value.replace(QRegExp("[òóôõöø]"), "o");
    value.replace(QRegExp("[ÒÓÔÕÖØ]"), "O");
    value.replace(QRegExp("[ùúûü]"), "u");
    value.replace(QRegExp("[ÙÚÛÜ]"), "U");
    value.replace(QRegExp("[ñ]"), "n");
    value.replace(QRegExp("[Ñ]"), "N");
    value.replace(QRegExp("[ç]"), "c");
    value.replace(QRegExp("[Ç]"), "C");
    value.replace(QRegExp("[\\W]"), "_");
    return value;
}

