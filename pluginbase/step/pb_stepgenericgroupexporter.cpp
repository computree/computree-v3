#include "pb_stepgenericgroupexporter.h"

#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/ct_resultgroup.h"

#include "ct_step/ct_stepinitializedata.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"

#include <QFileDialog>

#define DEF_SearchInGroup    "g"
#define DEF_SearchInItem        "i"
#define DEF_SearchInResultToExport  "r"

PB_StepGenericGroupExporter::PB_StepGenericGroupExporter(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _exporter = new PB_GroupDataExporter();
    _exporter->init();
}

PB_StepGenericGroupExporter::~PB_StepGenericGroupExporter()
{
    delete _exporter;
}

QString PB_StepGenericGroupExporter::getStepDescription() const
{
    return tr("Exporte en txt");
}

CT_VirtualAbstractStep* PB_StepGenericGroupExporter::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    return new PB_StepGenericGroupExporter(dataInit);
}

//////////////////// PROTECTED //////////////////

void PB_StepGenericGroupExporter::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resultModel = createNewInResultModelForCopy(DEF_SearchInResultToExport, tr("Résultat"), "", true);

    resultModel->setZeroOrMoreRootGroup();
    resultModel->addGroupModel("", DEF_SearchInGroup, "Groupe à exporter");
    resultModel->addItemModel(DEF_SearchInGroup, DEF_SearchInItem, CT_AbstractItemDrawable::staticGetType(), tr("Item de séléction"), tr("Tous les items du groupe sont exportés SSI cet item existe"));
}

void PB_StepGenericGroupExporter::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
    configDialog->addFileChoice(tr("Fichier d'export :"), CT_FileChoiceButton::OneNewFile, "Fichier txt (*.txt)", _exportFilename);
}

void PB_StepGenericGroupExporter::createOutResultModelListProtected()
{
    createNewOutResultModelToCopy(DEF_SearchInResultToExport);
}

void PB_StepGenericGroupExporter::compute()
{
    CT_ResultGroup *outRes = getOutResultList().first();
    
    QList<ItemDrawable*> itemsToExport;


    // create a iterator to find groups that user selected with the IN model named DEF_SearchInGroup
    CT_ResultGroupIterator it(outRes, this, DEF_SearchInGroup);

    // iterate over all groups
    while(!isStopped() && it.hasNext())
    {
        CT_AbstractItemGroup *group = (CT_AbstractItemGroup*)it.next();

        CT_AbstractSingularItemDrawable* item = group->firstItemByINModelName(this, DEF_SearchInItem);
        
        if (item != NULL)
        {
            itemsToExport.append(group);
        }
    }
    
    if (!_exportFilename.isEmpty())
    {
        // une fois la liste constituée
        if(!itemsToExport.isEmpty() && _exportFilename.first()!="")
        {
            _exporter->setExportFilePath(_exportFilename.first());
            
            // on la donne à l'exportateur
            _exporter->setItemDrawableToExport(itemsToExport);
            
            // et on exporte....
            _exporter->exportToFile();
        }
    }
}

