// Inclusion du fichier d'entête
#include "tufr_steptutorial05.h"

// Inclusion des modèles de résultats in et out, PERMETTANT LA COPIE
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"

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

// Inclusion des item utiles
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_circle.h"

// Alias des chaines de caractères pour l'indexation des modèles in et out
#define DEF_SearchInResult  "r"
#define DEF_SearchInPointCloud   "pc"

#define DEF_SearchOutResult  "r"

TUFR_StepTutorial05::TUFR_StepTutorial05(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    // pas de paramètres
}

// Description de l'étape (tooltip du menu contextuel)
QString TUFR_StepTutorial05::getStepDescription() const
{
    return "Ajuste des cercles sur des groupes de points";
}

// Méthode de recopie de l'étape
CT_VirtualAbstractStep* TUFR_StepTutorial05::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette étape
    return new TUFR_StepTutorial05(dataInit);
}

//////////////////// PROTECTED //////////////////

// Création et affiliation des modèles IN
void TUFR_StepTutorial05::createInResultModelListProtected()
{
    // Déclaration et création du modèle de groupe racine
    CT_InOneOrMoreGroupModel *groupModel;
    groupModel = new CT_InOneOrMoreGroupModel();

    // Déclaration et création du modèle d'item groupe de points
    CT_InStandardItemDrawableModel *pointCloudItemModel;
    pointCloudItemModel = new CT_InStandardItemDrawableModel(CT_AbstractItemDrawableWithPointCloud::staticGetType(),
                             CT_InStandardItemDrawableModel::C_ChooseOneIfMultiple,
                             DEF_SearchInPointCloud,
                             CT_InStandardItemDrawableModel::F_IsObligatory,
                             "Groupe de points",
                             "Objet héritant de CT_AbstractItemDrawableWithPointCloud");

    // On ajoute le modèle d'item (nuage de points) au modèle du groupe racine
    groupModel->addItem(pointCloudItemModel);

    // Déclaration et création du modèle de résultat
    CT_InResultModelGroup *resultModel;
    resultModel = new CT_InResultModelGroup(groupModel,
                                            DEF_SearchInResult,
                                            false,
                                            "Un résultat avec une/des groupes de points",
                                            "Résultat avec un/des groupes de points");

    // Ajout du modèle de résultat séquenciellement dans cette étape
    // En réalité cette méthode génère aussi un resulat auquel est attaché ce modèle
    // Les addInResultModel susccessifs, ajoutent des résultats dans l'ordre à la InResultList
    addInResultModel(resultModel);
}

// Création et affiliation des modèles OUT
void TUFR_StepTutorial05::createOutResultModelListProtected()
{
    // on récupère le résultat modèle d'entrée à copier défini dans "createInResultModelListProtected()"
    CT_InResultModelGroupToCopy *intResModelToCopy = (CT_InResultModelGroupToCopy*)getInResultModel(DEF_SearchInResult);

    // on récupère toutes les possibilités de RESULTATS que l'utilisateur a défini
    // ici il n'y en aobligatoirement une seule
    QList<CT_OutResultModelGroupToCopyPossibility*> copyList = intResModelToCopy->getOutResultModelGroupsSelectedToCopy();

    // On vérifie qu'il y a au moins une possibilité (on sait jamais ...)
    if (!copyList.isEmpty())
    {
        // On récupère la possibilité choisie
        CT_OutResultModelGroupToCopyPossibility *outResModelToCopy = copyList.first();

        // on récupère le résultat modèle de sortie à copier/modifier
        CT_OutResultModelGroup *outResModelModif = outResModelToCopy->outModelForModification();
        const CT_OutResultModelGroup *outResModelSearch = outResModelToCopy->outModelForSearch();

        // on récupère le modèle du nuage de point défini dans "createInResultModelListProtected()"
        // pour la recherche
        CT_InAbstractItemDrawableModel *inPointCloudModel = (CT_InAbstractItemDrawableModel*)getInModel(*outResModelSearch, DEF_SearchInPointCloud);

        // on recherche tous les modèles d'items du type nuage de points qui ont été sélectionnés
        // par l'utilisateur
        if(outResModelModif->recursiveBeginIterateItemsModel(*inPointCloudModel))
        {
            DEF_CT_AbstractGroupModelOut *outModel;

            // un if et non un while car on a défini C_ChooseOneIfMultiple
            // il ne peut donc y avoir qu'un seul modèle choisi pour le nuage de point
            if((outModel = outResModelModif->recursiveNextItemModel()) != NULL)
            {
                // un nouveau modèle pour les cercles SANS NOM puisqu'il sera défini automatiquement
                CT_OutStandardItemDrawableModel *newItemModel = new CT_OutStandardItemDrawableModel(new CT_Circle(), "", "Cercle", "Cercle ajusté");

                // On passe le modèle de cercle au modèle du groupe contenant le modèle du nuage de points
                // on passe aussi un objet de type CT_AutoRenameModels permettant de renommer l'item au cas où le nom
                // du modèle existe déjà (obligatoire dans ce cas puisqu'on ne connait pas les noms
                // des modèles du résultat que l'on copie et il risque d'y avoir conflit)
                outModel->addItem(newItemModel, _outCircleModelName);
            }
        }

        // on crée un nouveau résultat de sortie contenant le résultat modèle modifié
        addOutResultModel(new CT_OutResultModelGroupCopy(DEF_SearchOutResult, outResModelToCopy));
    }
}

// Création semi-automatique de la boite de dialogue de paramétrage de l'étape
void TUFR_StepTutorial05::createPostConfigurationDialog()
{
    // Pas de boite de dialogue
}

void TUFR_StepTutorial05::compute()
{
    // on récupère le résultat copié (résultat de sortie donc)
    CT_ResultGroup *outRes = getOutResultList().first();

    // ainsi que son modèle
    CT_OutAbstractResultModel *outResModel = outRes->model();

    // on récupère le modèle d'entrée qu'on avait ajouté
    CT_InResultModelGroupToCopy *intResModel = (CT_InResultModelGroupToCopy*)getInResultModel(DEF_SearchInResult);

    // Afin de récupérer le modèle de sortie correspondant
    // Ici il n'y a qu'une possiblité, donc un seul modèle, d'où le .first()
    const CT_OutResultModelGroup *outModelForSearchInModel = intResModel->getOutResultModelForSearchInModel().first();

    // et qu'on utilise ici pour rechercher le modèle d'entrée du nuage de point
    CT_InAbstractItemDrawableModel *inPointCloudModel = (CT_InAbstractItemDrawableModel*)getInModel(*outModelForSearchInModel, DEF_SearchInPointCloud);

    // on utilise le modèle du résultat de sortie pour rechercher le modèle de sortie du Cercle.
    // En utilisant le nom automatique généré lors de l'ajout (grace au CT_AutoRenameModels)
    CT_OutAbstractModel *outCircleModel = getOutModel(*outResModel, _outCircleModelName.completeName());

    /**************************************************/

    // on va rechercher tous les groupes contenant des nuages de points (qui ont été choisi par l'utilisateur)
    if(outRes->recursiveBeginIterateItems(*inPointCloudModel))
    {
        CT_AbstractItemGroup *group;

        // pour chaque groupe contenant un nuage de point
        while((group = outRes->recursiveNextItem()) != NULL && (!isStopped()))
        {
            group->beginIterateItems(*inPointCloudModel);

            // on récupère le groupe de point
            const CT_AbstractItemDrawableWithPointCloud *item = (const CT_AbstractItemDrawableWithPointCloud*)group->nextItem();

            // on créé le cercle mathématique
            CT_CircleData *cData = CT_CircleData::staticCreateZAxisAlignedCircleDataFromPointCloud(*item->getPointCloud(),
                                                                                                   *item->getPointCloudIndex(),                                                                                                   item->getCenterZ());

            // et on ajoute un itemDrawable cercle
            if(cData != NULL)
            {
                group->addItemDrawable(new CT_Circle(outCircleModel, item->id(), outRes, cData));
            }
        }
    }
}
