#include "tufr_templatestep.h"

// Inclusion des modèles de résultats in et out
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

// Inclusion des modèles de groupes in et out
#include "ct_itemdrawable/model/inModel/ct_inoneormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instandardgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"

// Inclusion des modèles d'items in et out
#include "ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h"

// Inclusion de la classe de résultat standard
#include "ct_result/ct_resultgroup.h"

#include "qdebug.h" // pour afficher des messages à la console
#include "math.h"

// Alias des chaines de caractères pour l'indexation des modèles in et out
#define DEF_SearchInResult  "r"
#define DEF_SearchOutResult "r"

// Constructeur : initialisation des paramètres
TUFR_TemplateStep::TUFR_TemplateStep(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _parametre = 0;
}

// Description de l'étape (tooltip du menu contextuel)
QString TUFR_TemplateStep::getStepDescription() const
{
    return "Description";
}

// Méthode de recopie de l'étape
CT_VirtualAbstractStep* TUFR_TemplateStep::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette étape
    return new TUFR_TemplateStep(dataInit);
}

//////////////////// PROTECTED //////////////////

// Création et affiliation des modèles IN
void TUFR_TemplateStep::createInResultModelListProtected()
{
}

// Création et affiliation des modèles OUT
void TUFR_TemplateStep::createOutResultModelListProtected()
{

}

// Création semi-automatique de la boite de dialogue de paramétrage de l'étape
void TUFR_TemplateStep::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble("Valeur du paramètre", "unité", 0, 1000, 2, _parametre);
}

void TUFR_TemplateStep::compute()
{


    // PENSER A AJOUTER CETTE ETAPE AU STEPPLUGINMANAGER !!!!!

}
