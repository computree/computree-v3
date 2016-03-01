#include "pb_stepexportattributesinloop.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_itemdrawable/tools/iterator/ct_itemiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

#include "ct_abstractstepplugin.h"
#include "ct_exporter/ct_standardexporterseparator.h"

#include "ct_itemdrawable/ct_profile.h"
#include "exporters/profile/pb_profileexporter.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include <QFile>
#include <QTextStream>

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_itemWithAttribute "itemWithAttribute"
#define DEFin_attribute "attribute"

#define DEFout_res "res"
#define DEFout_grp "grp"


// Constructor : initialization of parameters
PB_StepExportAttributesInLoop::PB_StepExportAttributesInLoop(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

PB_StepExportAttributesInLoop::~PB_StepExportAttributesInLoop()
{
}


// Step description (tooltip of contextual menu)
QString PB_StepExportAttributesInLoop::getStepDescription() const
{
    return tr("Export d'attributs dans une boucle");
}

// Step detailled description
QString PB_StepExportAttributesInLoop::getStepDetailledDescription() const
{
    return tr("");
}

// Step URL
QString PB_StepExportAttributesInLoop::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepExportAttributesInLoop::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepExportAttributesInLoop(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepExportAttributesInLoop::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn = createNewInResultModel(DEFin_res, tr("Résultat"));
    resIn->setZeroOrMoreRootGroup();
    resIn->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn->addItemModel(DEFin_grp, DEFin_itemWithAttribute, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"), "", CT_InAbstractModel::C_ChooseMultipleIfMultiple);
    resIn->addItemAttributeModel(DEFin_itemWithAttribute, DEFin_attribute, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Attribut"), "", CT_InAbstractModel::C_ChooseMultipleIfMultiple);
}

// Creation and affiliation of OUT models
void PB_StepExportAttributesInLoop::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_res = createNewOutResultModel(DEFout_res, tr("Résultat vide"));
    res_res->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe vide"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepExportAttributesInLoop::createPostConfigurationDialog()
{
}

void PB_StepExportAttributesInLoop::compute()
{

    QFile f("./testExport.txt");
    f.open(QFile::WriteOnly);
    QTextStream stream (&f);

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn = inResultList.at(0);


    // IN results browsing
    CT_ResultGroupIterator itIn_grp(resIn, this, DEFin_grp);
    while (itIn_grp.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grp = (CT_AbstractItemGroup*) itIn_grp.next();

        CT_ItemIterator itItem(grp, this, DEFin_itemWithAttribute);

        while (itItem.hasNext())
        {
            CT_AbstractSingularItemDrawable* item = (CT_AbstractSingularItemDrawable*) itItem.next();

            stream << item->displayableName() << ";";

            if (item != NULL)
            {
                QList<CT_AbstractItemAttribute *> attributes = item->itemAttributes((CT_InAbstractItemAttributeModel*) PS_MODELS->searchModel(DEFin_attribute, resIn, this));

                for (int i = 0 ; i < attributes.size() ; i++)
                {
                    CT_AbstractItemAttribute* attribute = attributes.at(i);
                    stream << attribute->displayableName() << ";";
                }
            }

            stream << "\r\n";
        }
    }

    f.close();

}
