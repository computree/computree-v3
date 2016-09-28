#include "pb_stepcreatereaderlist.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_genericconfigurablewidget.h"
#include "ct_abstractstepplugin.h"
#include "ct_reader/ct_standardreaderseparator.h"

#include "ct_view/ct_combobox.h"
#include "ct_global/ct_context.h"
#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_itemdrawable/ct_readeritem.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_view/tools/ct_configurablewidgettodialog.h"

// Alias for indexing models
#define DEFout_res "res"
#define DEFout_grp "grp"
#define DEFout_grpHeader "grpHeader"
#define DEFout_header "header"
#define DEFout_reader "reader"

// Constructor : initialization of parameters
PB_StepCreateReaderList::PB_StepCreateReaderList(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
    _readersListValue = "";

    initListOfAvailableReaders();
}

PB_StepCreateReaderList::~PB_StepCreateReaderList()
{
    clear();
}

// Step description (tooltip of contextual menu)
QString PB_StepCreateReaderList::getStepDescription() const
{
    return tr("Créer une liste de readers");
}

// Step detailled description
QString PB_StepCreateReaderList::getStepDetailledDescription() const
{
    return tr("Cette étape créer une liste de reader (lecteur de fichier). Ces readers pourront par la suite être utilisés par d'autres étapes pour charger les fichiers.");
}

// Step URL
QString PB_StepCreateReaderList::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStepCanBeAddedFirst::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepCreateReaderList::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepCreateReaderList(dataInit);
}


//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepCreateReaderList::createInResultModelListProtected()
{
    // No in result is needed
    setNotNeedInputResult();
}

void PB_StepCreateReaderList::createPreConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPreConfigurationDialog();

    QStringList list_readersList;

    QListIterator<CT_AbstractReader*> it(_readersInstancesList);

    while (it.hasNext())
        list_readersList.append(it.next()->GetReaderClassName());

    if (list_readersList.isEmpty())
        list_readersList.append(tr("ERREUR : aucun reader disponible"));

    configDialog->addStringChoice(tr("Choix du type de fichier"), "", list_readersList, _readersListValue);
}

bool PB_StepCreateReaderList::postConfigure()
{
    QString fileFilter = getFormat(_readersListValue);

    QStringList fileList = _filesList;

    CT_GenericConfigurableWidget configDialog;
    configDialog.addFileChoice(tr("Choisir les fichiers"), CT_FileChoiceButton::OneOrMoreExistingFiles, fileFilter, fileList);

    if(CT_ConfigurableWidgetToDialog::exec(&configDialog) == QDialog::Accepted) {

        if(fileList.isEmpty())
            return false;

        CT_AbstractReader *reader = getReader(_readersListValue);

        if((reader != NULL) && reader->setFilePath(fileList.first())) {
            reader->setFilePathCanBeModified(false);
            bool ok = reader->configure();
            reader->setFilePathCanBeModified(true);

            if(ok) {
                _filesList = fileList;
                setSettingsModified(true);
            }

            return ok;
        }
    }

    return false;
}

// Creation and affiliation of OUT models
void PB_StepCreateReaderList::createOutResultModelListProtected()
{
    // create a new result
    CT_OutResultModelGroup *outRes = createNewOutResultModel(DEFout_res, tr("Liste de readers"));

    // add a root group
    outRes->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));

    // get the reader selected
    CT_AbstractReader *reader = getReader(_readersListValue);

    // if one reader was selected and at least one file is defined
    if (reader != NULL && _filesList.size() > 0)
    {
        // get the header
        CT_FileHeader *rHeader = reader->createHeaderPrototype();

        if(rHeader != NULL) {
            // copy the reader (copyFull = with configuration and models)
            CT_AbstractReader* readerCpy = reader->copyFull();

            outRes->addGroupModel(DEFout_grp, DEFout_grpHeader, new CT_StandardItemGroup(), tr("Fichier"));
            outRes->addItemModel(DEFout_grpHeader, DEFout_reader, new CT_ReaderItem(NULL, NULL, readerCpy), tr("Reader"));
            outRes->addItemModel(DEFout_grpHeader, DEFout_header, rHeader, tr("Entête"));
        }
    }
}

void PB_StepCreateReaderList::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();

    // get the out result
    CT_ResultGroup* resultOut = outResultList.at(0);

    // search the model for headers
    CT_OutAbstractItemModel* headerModel = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(DEFout_header, resultOut);

    // create the root group and add it to result
    CT_StandardItemGroup* grp = new CT_StandardItemGroup(DEFout_grp, resultOut);
    resultOut->addGroup(grp);

    CT_AbstractReader *reader = getReader(_readersListValue);

    if(reader != NULL) {
        // for each file in the list
        for (int i = 0 ; i < _filesList.size() ; i++)
        {
            // copy the reader (copyFull = with configuration and models)
            CT_AbstractReader* readerCpy = reader->copyFull();

            // set the new filepath and check if it is valid
            if (readerCpy->setFilePath(_filesList.at(i)))
            {
                // create models of this reader
                if(readerCpy->outItemDrawableModels().isEmpty() && reader->outGroupsModel().isEmpty())
                    readerCpy->createOutItemDrawableModelList();

                // create the group that will contains header and reader (represent a File)
                CT_StandardItemGroup* grpHeader = new CT_StandardItemGroup(DEFout_grpHeader, resultOut);

                CT_FileHeader *header = readerCpy->readHeader();
                header->changeResult(resultOut);
                header->setModel(headerModel);

                // add the header
                grpHeader->addItemDrawable(header);

                // add the reader
                grpHeader->addItemDrawable(new CT_ReaderItem(DEFout_reader, resultOut, readerCpy));

                // add the group to the root
                grp->addGroup(grpHeader);
            }
            else
            {
                PS_LOG->addMessage(LogInterface::warning, LogInterface::step, tr("Fichier %1 inexistant ou non valide").arg(_filesList.at(i)));
                delete readerCpy;
            }
        }
    }
}

SettingsNodeGroup *PB_StepCreateReaderList::getAllSettings() const
{
    SettingsNodeGroup *root = CT_AbstractStepCanBeAddedFirst::getAllSettings();
    SettingsNodeGroup *group = new SettingsNodeGroup("PB_StepCreateReaderList");
    group->addValue(new SettingsNodeValue("Version", "1"));

    SettingsNodeGroup *settings = new SettingsNodeGroup("Settings");
    settings->addValue(new SettingsNodeValue("ReaderSelected", _readersListValue));

    for (int i = 0 ; i < _filesList.size() ; i++) {
        SettingsNodeGroup *fileSettings = new SettingsNodeGroup("File");
        fileSettings->addValue(new SettingsNodeValue("path", _filesList.at(i)));
        settings->addGroup(fileSettings);
    }

    SettingsNodeGroup *readerSettings = new SettingsNodeGroup("ReaderSettings");
    settings->addGroup(readerSettings);

    CT_AbstractReader *reader = getReader(_readersListValue);

    if(reader != NULL)
        readerSettings->addGroup(reader->getAllSettings());

    group->addGroup(settings);
    root->addGroup(group);

    return root;
}

bool PB_StepCreateReaderList::setAllSettings(const SettingsNodeGroup *settings)
{
    bool ok = CT_AbstractStepCanBeAddedFirst::setAllSettings(settings);

    if(ok)
    {
        QList<SettingsNodeGroup*> groups = settings->groupsByTagName("PB_StepCreateReaderList");
        if(groups.isEmpty()) {return false;}

        groups = groups.first()->groupsByTagName("Settings");
        if(groups.isEmpty()) {return false;}

        SettingsNodeGroup *settings2 = groups.first();

        QList<SettingsNodeValue*> values = settings2->valuesByTagName("ReaderSelected");
        if(values.isEmpty()) {return false;}

        _readersListValue = values.first()->value().toString();

        _filesList.clear();

        groups = settings2->groupsByTagName("File");

        foreach (SettingsNodeGroup *gr, groups) {
            values = gr->valuesByTagName("path");

            if(values.isEmpty())
                return false;

            _filesList.append(values.first()->value().toString());
        }

        groups = settings2->groupsByTagName("ReaderSettings");
        if(groups.isEmpty()) {return false;}

        groups = groups.first()->groups();

        CT_AbstractReader *reader = getReader(_readersListValue);

        if(reader != NULL) {
            if(groups.isEmpty()) {return false;}

            return reader->setAllSettings(groups.first());
        }

        if(!groups.isEmpty())
            return false;
    }

    return ok;
}

void PB_StepCreateReaderList::initListOfAvailableReaders()
{
    clear();

    // get the plugin manager
    PluginManagerInterface *pm = PS_CONTEXT->pluginManager();
    int s = pm->countPluginLoaded();

    // for each plugin
    for(int i=0; i<s; ++i)
    {
        CT_AbstractStepPlugin *p = pm->getPlugin(i);

        // get readers
        QList<CT_StandardReaderSeparator*> rsl = p->getReadersAvailable();
        QListIterator<CT_StandardReaderSeparator*> itR(rsl);

        while(itR.hasNext())
        {
            CT_StandardReaderSeparator *rs = itR.next();
            QListIterator<CT_AbstractReader*> itE(rs->readers());

            while(itE.hasNext())
            {
                CT_AbstractReader *reader = itE.next();

                // copy the reader
                CT_AbstractReader *readerCpy = reader->copy();
                readerCpy->init(false);

                // and add it to the list
                _readersInstancesList.append(readerCpy);
            }
        }
    }
}

void PB_StepCreateReaderList::clear()
{
    qDeleteAll(_readersInstancesList.begin(), _readersInstancesList.end());
    _readersInstancesList.clear();
}

QString PB_StepCreateReaderList::getFormat(QString readerClassName) const
{
    CT_AbstractReader *reader = getReader(readerClassName);

    if(reader != NULL) {
        FileFormat fileFormat = reader->readableFormats().first();

        QString formatText = fileFormat.description();
        formatText.append(" (");
        const QList<QString> &suffixes = fileFormat.suffixes();
        for (int i = 0 ; i < suffixes.size() ; i++)
        {
            if (i > 0) {formatText.append(" ");}
            formatText.append("*.");
            formatText.append(suffixes.at(i));
        }
        formatText.append(")");

        return formatText;
    }

    return tr("Aucun reader disponible (*.error)");
}

CT_AbstractReader *PB_StepCreateReaderList::getReader(QString readerClassName) const
{
    QListIterator<CT_AbstractReader*> it(_readersInstancesList);

    while(it.hasNext()) {
        CT_AbstractReader *reader = it.next();

        if(reader->GetReaderClassName() == readerClassName) {
            return reader;
        }
    }

    return NULL;
}
