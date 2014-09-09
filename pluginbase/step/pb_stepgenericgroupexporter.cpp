#include "pb_stepgenericgroupexporter.h"

#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/ct_resultgroup.h"

#include "ct_step/ct_stepinitializedata.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include <QFileDialog>

#define DEF_SearchInGroup    "g"
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
    resultModel->addGroupModel("", DEF_SearchInGroup, CT_AbstractItemGroup::staticGetType(), "Groupe à exporter");
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
    
    QList<CT_AbstractItemDrawable*> itemsToExport;


    // create a iterator to find groups that user selected with the IN model named DEF_SearchInGroup
    CT_ResultGroupIterator it(outRes, this, DEF_SearchInGroup);

    // iterate over all groups
    while(!isStopped() && it.hasNext())
    {
        CT_AbstractItemGroup *group = (CT_AbstractItemGroup*)it.next();
        itemsToExport.append(group);
    }
    
    if (!_exportFilename.isEmpty())
    {
        // une fois la liste constituée
        if(!itemsToExport.isEmpty() && _exportFilename.first()!="")
        {
            _exporter->setExportFilePath(_exportFilename.first());
            
            // on la donne à l'exportateur
            if(!_exporter->setItemDrawableToExport(itemsToExport))
            {
                PS_LOG->addErrorMessage(this, _exporter->errorMessage());
                setErrorMessage(1, _exporter->errorMessage());
                setErrorCode(1);
            }
            else
            {
                // et on exporte....
                _exporter->exportToFile();
            }
        }
    }
}

