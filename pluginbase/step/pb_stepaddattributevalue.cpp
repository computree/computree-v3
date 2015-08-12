#include "pb_stepaddattributevalue.h"

#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

#include "ct_view/ct_stepconfigurabledialog.h"

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_item "profile"



// Constructor : initialization of parameters
PB_StepAddAttributeValue::PB_StepAddAttributeValue(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _name = "Hmin";
    _value = 0.0;
}

// Step description (tooltip of contextual menu)
QString PB_StepAddAttributeValue::getStepDescription() const
{
    return tr("Corriger le profil de densité de points ALS");
}

// Step detailled description
QString PB_StepAddAttributeValue::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepAddAttributeValue::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepAddAttributeValue::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepAddAttributeValue(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepAddAttributeValue::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Profile"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grp, DEFin_item, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"));

}

// Creation and affiliation of OUT models
void PB_StepAddAttributeValue::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);
    resCpy_res->addItemAttributeModel(DEFin_item, _Att_ModelName, new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_VALUE), _name);
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepAddAttributeValue::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addString(tr("Nom de l'attribut"), "", _name);
    configDialog->addDouble(tr("Valeur de l'attribut"), "", -1e+06, 1e+06, 4, _value);
}

void PB_StepAddAttributeValue::compute()
{

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res = outResultList.at(0);

    // COPIED results browsing
    CT_ResultGroupIterator itCpy_grp(res, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itCpy_grp.next();
        
        CT_AbstractSingularItemDrawable* inItem = grp->firstItemByINModelName(this, DEFin_item);
        if (inItem != NULL)
        {
            inItem->addItemAttribute(new CT_StdItemAttributeT<double>(_Att_ModelName.completeName(),
                                                                      CT_AbstractCategory::DATA_VALUE,
                                                                      res,
                                                                      _value));
        }
    }


}

