#include "pb_stepcorrectalsprofile.h"

#include "ct_itemdrawable/ct_profile.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

#include "ct_view/ct_stepconfigurabledialog.h"

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_profile "profile"



// Constructor : initialization of parameters
PB_StepCorrectALSProfile::PB_StepCorrectALSProfile(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _threshold = 3.0;
}

// Step description (tooltip of contextual menu)
QString PB_StepCorrectALSProfile::getStepDescription() const
{
    return tr("Corrigé le profil de densité de points ALS");
}

// Step detailled description
QString PB_StepCorrectALSProfile::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepCorrectALSProfile::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepCorrectALSProfile::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepCorrectALSProfile(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepCorrectALSProfile::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Profile"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grp, DEFin_profile, CT_Profile<int>::staticGetType(), tr("Profil"));

}

// Creation and affiliation of OUT models
void PB_StepCorrectALSProfile::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);

    resCpy_res->addItemModel(DEFin_grp, _profile_ModelName, new CT_Profile<double>(), tr("Profil corrigé"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepCorrectALSProfile::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble(tr("Supprimer les données en dessous de"), "m", 0, 10000, 2, _threshold);
}

void PB_StepCorrectALSProfile::compute()
{

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res = outResultList.at(0);

    // COPIED results browsing
    CT_ResultGroupIterator itCpy_grp(res, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itCpy_grp.next();
        
        const CT_Profile<int>* inProfile = (CT_Profile<int>*)grp->firstItemByINModelName(this, DEFin_profile);
        if (inProfile != NULL)
        {
            const Eigen::Vector3d &dir = inProfile->getDirection();
            CT_Profile<double>* outProfile = new CT_Profile<double>(_profile_ModelName.completeName(), res,
                                                                    inProfile->minX(), inProfile->minY(), inProfile->minZ(),
                                                                    dir(0), dir(1), dir(2), inProfile->getDim(), inProfile->resolution(), inProfile->NA(), 0);

            for (size_t index = 0 ; index < inProfile->getDim() ; index++)
            {
                int inVal = inProfile->valueAtIndex(index);
                if (inVal == inProfile->NA() || (inProfile->lengthForIndex(index) < _threshold))
                {
                    outProfile->setValueAtIndex(index, outProfile->NA());
                } else {
                    double sum = 0;
                    for (size_t i = 0 ; i <= index ; i++)
                    {
                        int val = inProfile->valueAtIndex(i);
                        if (val != inProfile->NA() && inProfile->lengthForIndex(i) >= _threshold)
                        {
                            sum += (double)val;
                        }
                    }
                    outProfile->setValueAtIndex(index, (double)inVal / sum) ;
                }
            }
            outProfile->computeMinMax();

            grp->addItemDrawable(outProfile);
        }
    }


}
