#include "pb_stepcreatedatasource.h"

#include "ct_itemdrawable/ct_datasourcegeo.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_abstractstepplugin.h"
#include "ct_reader/ct_standardreaderseparator.h"

#include "ct_reader/ct_reader_xyb.h"
#include "ct_view/ct_combobox.h"
#include "ct_global/ct_context.h"
#include "ct_model/tools/ct_modelsearchhelper.h"


// Alias for indexing models
#define DEFout_res "res"
#define DEFout_grp "grp"
#define DEFout_datasource "datasource"
#define DEFout_grpHeader "grpHeader"
#define DEFout_header "header"


// Constructor : initialization of parameters
PB_StepCreateDataSource::PB_StepCreateDataSource(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
    _readersListValue = "xyb";
    _fileChoiceButton = NULL;

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
                    const FileFormat& format = formats.at(i);

                    QString key = QString("%2 - %1").arg(reader->GetReaderName()).arg(format.description());
                    _readersMap.insert(key, QPair<CT_AbstractReader*, int>(readerCpy, n));
                }

            }
        }
    }

}

PB_StepCreateDataSource::~PB_StepCreateDataSource()
{
    qDeleteAll(_readersInstancesList);
}

// Step description (tooltip of contextual menu)
QString PB_StepCreateDataSource::getStepDescription() const
{
    return tr("Créée une source de données");
}

// Step detailled description
QString PB_StepCreateDataSource::getStepDetailledDescription() const
{
    return tr("Une source de données est une liste de fichiers du même type.\nSeuls les headers des fichiers sont chargés à ce stade.");
}

// Step URL
QString PB_StepCreateDataSource::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStepCanBeAddedFirst::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepCreateDataSource::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepCreateDataSource(dataInit);
}

void PB_StepCreateDataSource::setFormat(QString formatName)
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

        _fileChoiceButton->setFormat(formatText);
    } else {
        _fileChoiceButton->setFormat(tr("Aucun reader disponible (*.error)"));
    }
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepCreateDataSource::createInResultModelListProtected()
{
    // No in result is needed
    setNotNeedInputResult();
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepCreateDataSource::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();


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

    CT_ComboBox* comboBox = configDialog->addStringChoice("Choix du type de fichier", "", list_readersList, _readersListValue);
    _fileChoiceButton = configDialog->addFileChoice("Choisir les fichiers", CT_FileChoiceButton::OneOrMoreExistingFiles, list_readersList.first(), _filesList);

    QObject::connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setFormat(QString)));

}

// Creation and affiliation of OUT models
void PB_StepCreateDataSource::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_res = createNewOutResultModel(DEFout_res, tr("Source de données"));
    res_res->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));

    const QPair<CT_AbstractReader*, int> &pair = _readersMap.value(_readersListValue);

    if (pair.first != NULL && _filesList.size() > 0)
    {

        CT_DataSource* dataSource = NULL;
        QString itemLabel = tr("Source de données");

        if (pair.first->hasBoundingBox())
        {
            dataSource = new CT_DataSourceGeo();
            itemLabel = tr("Source de données géographique");

        } else {
            dataSource = new CT_DataSource();
        }

        CT_AbstractReader* firstReader = NULL;

        for (int i = 0 ; i < _filesList.size() ; i++)
        {
            CT_AbstractReader* reader = pair.first->copy();

            if (reader->setFilePath(_filesList.at(i)))
            {
                if (dataSource->addReader(reader))
                {
                    reader->init();
                    if (firstReader == NULL) {firstReader = reader;}
                } else {
                    delete reader;
                }

            } else {
                delete reader;
            }
        }

        if (firstReader != NULL)
        {
            CT_FileHeader *header = (CT_FileHeader*) firstReader->getHeader();

            if (header != NULL)
            {
                CT_FileHeader *headerCpy = (CT_FileHeader*) header->copy(NULL, NULL, CT_ResultCopyModeList());

                res_res->addGroupModel(DEFout_grp, DEFout_grpHeader, new CT_StandardItemGroup(), tr("Fichier"));
                res_res->addItemModel(DEFout_grpHeader, DEFout_header, headerCpy, tr("Entête"));
            }

            res_res->addItemModel(DEFout_grp, DEFout_datasource, dataSource, itemLabel);
        } else {
            delete dataSource;
        }
    }
}


void PB_StepCreateDataSource::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resultOut = outResultList.at(0);

    CT_StandardItemGroup* grp= new CT_StandardItemGroup(DEFout_grp, resultOut);
    resultOut->addGroup(grp);

    CT_OutStdSingularItemModel* dataSourceModel = (CT_OutStdSingularItemModel*) PS_MODELS->searchModelForCreation(DEFout_datasource, resultOut);

    if (dataSourceModel != NULL)
    {
        CT_DataSource* item_datasourceBase = (CT_DataSource*) dataSourceModel->itemDrawable();
        CT_DataSource* item_datasource = (CT_DataSource*) item_datasourceBase->copy(dataSourceModel, resultOut, CT_ResultCopyModeList());
        grp->addItemDrawable(item_datasource);

        item_datasource->init();
        while (item_datasource->activateNextReader())
        {
            QSharedPointer<CT_AbstractReader> reader = item_datasource->getActiveReader();

            CT_StandardItemGroup* grpHeader = new CT_StandardItemGroup(DEFout_grpHeader, resultOut);
            grp->addGroup(grpHeader);

            CT_OutAbstractItemModel* headerModel = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(DEFout_header, resultOut);

            if (headerModel != NULL)
            {
                CT_FileHeader *header = reader->takeHeader(resultOut, headerModel);
                if (header != NULL) {grpHeader->addItemDrawable(header);}
            }
        }
    }

}
