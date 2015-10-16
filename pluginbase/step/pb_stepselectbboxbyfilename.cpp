#include "pb_stepselectbboxbyfilename.h"

#include "ct_itemdrawable/ct_fileheader.h"
#include "ct_itemdrawable/ct_box2d.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"

#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

#include "ct_view/ct_asciifilechoicebutton.h"
#include "ct_view/ct_combobox.h"

// Alias for indexing models
#define DEFin_resHeader "resHeader"
#define DEFin_resBBox "resBBox"
#define DEFin_grpHeader "grpHeader"
#define DEFin_grpBBox "grpBBox"
#define DEFin_header "header"
#define DEFin_bbox "bbox"
#define DEF_inATT "fileNameAtt"

#include <QFile>
#include <QTextStream>


// Constructor : initialization of parameters
PB_StepSelectBBoxByFileName::PB_StepSelectBBoxByFileName(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

// Step description (tooltip of contextual menu)
QString PB_StepSelectBBoxByFileName::getStepDescription() const
{
    return tr("Charge l'emprise correspondant au nom de fichier");
}

// Step detailled description
QString PB_StepSelectBBoxByFileName::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepSelectBBoxByFileName::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepSelectBBoxByFileName::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepSelectBBoxByFileName(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepSelectBBoxByFileName::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn_res = createNewInResultModelForCopy(DEFin_resBBox, tr("Emprises"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grpBBox, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grpBBox, DEFin_header, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item Nom de fichier"));
    resIn_res->addItemAttributeModel(DEFin_header, DEF_inATT, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::STRING, tr("Nom de fichier"));
    resIn_res->addItemModel(DEFin_grpBBox, DEFin_bbox, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item Emprise"));


    CT_InResultModelGroupToCopy *resIn_resCpy = createNewInResultModelForCopy(DEFin_resHeader, tr("Fichier"), "", true);
    resIn_resCpy->setZeroOrMoreRootGroup();
    resIn_resCpy->addGroupModel("", DEFin_grpHeader, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_resCpy->addItemModel(DEFin_grpHeader, DEFin_header, CT_FileHeader::staticGetType(), tr("Entête de fichier"));
}

// Creation and affiliation of OUT models
void PB_StepSelectBBoxByFileName::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_resHeader);
    resCpy_res->addItemModel(DEFin_grpHeader, _outBBox_ModelName, new CT_Box2D(), tr("Emprise"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepSelectBBoxByFileName::createPostConfigurationDialog()
{
    //CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
}

void PB_StepSelectBBoxByFileName::compute()
{  

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* inRes = inResultList.at(0);

    QMap<QString, CT_AbstractSingularItemDrawable*> corresp;

    // Création de la liste des id recherchés
    CT_ResultGroupIterator itGrpIn(inRes, this, DEFin_grpBBox);
    while (itGrpIn.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* group = (CT_StandardItemGroup*) itGrpIn.next();

        if (group != NULL)
        {
            CT_AbstractSingularItemDrawable* fileHeader  = (CT_AbstractSingularItemDrawable*) group->firstItemByINModelName(this, DEFin_header);
            CT_AbstractSingularItemDrawable* bbox  = (CT_AbstractSingularItemDrawable*) group->firstItemByINModelName(this, DEFin_bbox);

            if (fileHeader != NULL && bbox != NULL)
            {
                CT_AbstractItemAttribute *attribute = fileHeader->firstItemAttributeByINModelName(inRes, this, DEF_inATT);
                if (attribute != NULL)
                {
                    bool ok;
                    QString filename = attribute->toString(fileHeader, &ok);

                    if (ok && !filename.isEmpty())
                    {
                        corresp.insert(filename.toLower(), bbox);
                    }
                }
            }
        }
    }


    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* outRes = outResultList.at(0);

    // Création de la liste des id recherchés
    CT_ResultGroupIterator itGrpOut(outRes, this, DEFin_grpHeader);
    while (itGrpOut.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* group = (CT_StandardItemGroup*) itGrpOut.next();

        if (group != NULL)
        {
            CT_FileHeader* fileHeader  = (CT_FileHeader*) group->firstItemByINModelName(this, DEFin_header);

            if (fileHeader != NULL)
            {
                QString filename = fileHeader->getFileInfo().fileName();
                CT_AbstractSingularItemDrawable* bbox = corresp.value(filename.toLower(), NULL);

                if (!filename.isEmpty() && bbox != NULL)
                {
                    Eigen::Vector3d min, max;
                    bbox->getBoundingBox(min, max);
                    Eigen::Vector2d min2d, max2d;

                    min2d(0) = min(0);
                    min2d(1) = min(1);
                    max2d(0) = max(0);
                    max2d(1) = max(1);

                    CT_Box2DData *data = new CT_Box2DData(min2d, max2d);
                    CT_Box2D *box = new CT_Box2D(_outBBox_ModelName.completeName(), outRes, data);
                    group->addItemDrawable(box);
                }
            }
        }
    }



}
