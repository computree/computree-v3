#include "pb_stepcreatedatasource.h"

#include "ct_itemdrawable/ct_datasourcegeo.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_abstractstepplugin.h"
#include "ct_reader/ct_standardreaderseparator.h"

#include "ct_reader/ct_reader_xyb.h"
#include "ct_view/ct_combobox.h"

// Alias for indexing models
#define DEFout_res "res"
#define DEFout_grp "grp"
#define DEFout_datasource "datasource"


// Constructor : initialization of parameters
PB_StepCreateDataSource::PB_StepCreateDataSource(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
    _readersListValue = "xyb";
    _isGeoReader = false;
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
    CT_OutResultModelGroup *res_res = createNewOutResultModel(DEFout_res, tr("Source de données géographique"));
    res_res->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));

    if (_isGeoReader)
    {
        res_res->addItemModel(DEFout_grp, DEFout_datasource, new CT_DataSourceGeo(), tr("Source de données géographique"));

    } else {
        res_res->addItemModel(DEFout_grp, DEFout_datasource, new CT_DataSource(), tr("Source de données"));
    }

}


void PB_StepCreateDataSource::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resultOut = outResultList.at(0);


    CT_StandardItemGroup* grp= new CT_StandardItemGroup(DEFout_grp, resultOut);
    resultOut->addGroup(grp);
    
    if (_isGeoReader)
    {
        CT_DataSourceGeo* item_datasource = new CT_DataSourceGeo(DEFout_datasource, resultOut, new CT_Reader_XYB());
        grp->addItemDrawable(item_datasource);
    } else {
        CT_DataSource* item_datasource = new CT_DataSource(DEFout_datasource, resultOut, new CT_Reader_XYB());
        grp->addItemDrawable(item_datasource);
    }





}
