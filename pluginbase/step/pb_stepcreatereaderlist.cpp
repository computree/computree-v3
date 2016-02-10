#include "pb_stepcreatereaderlist.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_abstractstepplugin.h"
#include "ct_reader/ct_standardreaderseparator.h"

#include "ct_view/ct_combobox.h"
#include "ct_global/ct_context.h"
#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_itemdrawable/ct_readeritem.h"


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

    // Create the available readers map
    PluginManagerInterface *pm = PS_CONTEXT->pluginManager();
    int s = pm->countPluginLoaded();

    for(int i=0; i<s; ++i)
    {
        CT_AbstractStepPlugin *p = pm->getPlugin(i);

        QList<CT_StandardReaderSeparator*> rsl = p->getReadersAvailable();
        QListIterator<CT_StandardReaderSeparator*> itR(rsl);

        while(itR.hasNext())
        {
            CT_StandardReaderSeparator *rs = itR.next();
            QListIterator<CT_AbstractReader*> itE(rs->readers());

            while(itE.hasNext())
            {
                CT_AbstractReader *reader = itE.next();
                CT_AbstractReader *readerCpy = reader->copy();
                readerCpy->init(false);
                _readersInstancesList.append(readerCpy);

                const QList<FileFormat>& formats = readerCpy->readableFormats();

                for (int n = 0 ; n < formats.size() ; n++)
                {
                    QString key = reader->GetReaderClassName();
                    _readersMap.insert(key, QPair<CT_AbstractReader*, int>(readerCpy, n));
                }
            }
        }
    }

}

PB_StepCreateReaderList::~PB_StepCreateReaderList()
{
    qDeleteAll(_readersInstancesList);
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

    QMapIterator<QString, QPair<CT_AbstractReader*, int> > it(_readersMap);
    while (it.hasNext())
    {
        it.next();
        list_readersList.append(it.key());
    }

    if (list_readersList.isEmpty())
    {
        list_readersList.append(tr("ERREUR : aucun reader disponible"));
    }

    configDialog->addStringChoice(tr("Choix du type de fichier"), "", list_readersList, _readersListValue);
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepCreateReaderList::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    QString fileFilter = getFormat(_readersListValue);
    configDialog->addFileChoice(tr("Choisir les fichiers"), CT_FileChoiceButton::OneOrMoreExistingFiles, fileFilter, _filesList);
}

// Creation and affiliation of OUT models
void PB_StepCreateReaderList::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *outRes = createNewOutResultModel(DEFout_res, tr("Liste de readers"));
    outRes->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));

    const QPair<CT_AbstractReader*, int> &pair = _readersMap.value(_readersListValue);

    CT_FileHeader *firstHeader = NULL;
    if (pair.first != NULL && _filesList.size() > 0)
    {
        bool found = false;
        CT_AbstractReader* reader = pair.first->copy();
        for (int i = 0 ; i < _filesList.size() && !found; i++)
        {
            if (reader->setFilePath(_filesList.at(i)))
            {
                firstHeader = (CT_FileHeader*) reader->getHeader();
                found = true;
            }
        }

        if (firstHeader != NULL)
        {
            CT_FileHeader *headerCpy = (CT_FileHeader*) firstHeader->copy(NULL, NULL, CT_ResultCopyModeList());

            outRes->addGroupModel(DEFout_grp, DEFout_grpHeader, new CT_StandardItemGroup(), tr("Fichier"));
            outRes->addItemModel(DEFout_grpHeader, DEFout_header, headerCpy, tr("Entête"));
            outRes->addItemModel(DEFout_grpHeader, DEFout_reader, new CT_ReaderItem(NULL, NULL, reader), tr("Reader"));
        } else {
            delete reader;
        }
    }
}


void PB_StepCreateReaderList::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resultOut = outResultList.at(0);

    CT_StandardItemGroup* grp = new CT_StandardItemGroup(DEFout_grp, resultOut);
    resultOut->addGroup(grp);

    const QPair<CT_AbstractReader*, int> &pair = _readersMap.value(_readersListValue);

    if (pair.first != NULL && _filesList.size() > 0)
    {
        for (int i = 0 ; i < _filesList.size() ; i++)
        {
            CT_AbstractReader* reader = pair.first->copy();

            if (reader->setFilePath(_filesList.at(i)))
            {
                reader->init();

                CT_StandardItemGroup* grpHeader = new CT_StandardItemGroup(DEFout_grpHeader, resultOut);
                grp->addGroup(grpHeader);

                CT_OutAbstractItemModel* headerModel = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(DEFout_header, resultOut);

                if (headerModel != NULL)
                {
                    CT_FileHeader *header = reader->takeHeaderCopy(resultOut, headerModel);
                    if (header != NULL) {grpHeader->addItemDrawable(header);}
                }

                CT_ReaderItem* readerItem = new CT_ReaderItem(DEFout_reader, resultOut, reader);
                grpHeader->addItemDrawable(readerItem);
            } else {
                PS_LOG->addMessage(LogInterface::warning, LogInterface::step, tr("Fichier %1 inexistant ou non valide").arg(_filesList.at(i)));
                delete reader;
            }
        }
    }
}

QString PB_StepCreateReaderList::getFormat(QString formatName)
{
    if (_readersMap.contains(formatName))
    {
        const QPair<CT_AbstractReader*, int> &pair = _readersMap.value(formatName);
        FileFormat fileFormat = pair.first->readableFormats().at(pair.second);

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
    } else {
        return tr("Aucun reader disponible (*.error)");
    }
}
