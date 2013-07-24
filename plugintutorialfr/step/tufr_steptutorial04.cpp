// Inclusion du fichier d'entête
#include "tufr_steptutorial04.h"

// Inclusion des modèles de résultats in et out, PERMETTANT LA COPIE
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"

// Inclusion des modèles de groupes in et out
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
#include "ct_itemdrawable/ct_pointcluster.h"
#include "ct_itemdrawable/ct_referencepoint.h"

// Inclusion du système d'action
#include "ct_tools/model/ct_outmodelcopyactionaddmodelitemingroup.h"

#include "math.h"

// Alias des chaines de caractères pour l'indexation des modèles in et out
#define DEF_SearchInGroup           "g"
#define DEF_SearchInPointCluster    "p"
#define DEF_SearchInResult          "r"
#define DEF_SearchOutResult         "r"


TUFR_StepTutorial04::TUFR_StepTutorial04(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    // pas de paramètres
}

// Description de l'étape (tooltip du menu contextuel)
QString TUFR_StepTutorial04::getStepDescription() const
{
    return tr("Générer des ReferencePoints à partir des barycentres de PointClusters");
}

// Méthode de recopie de l'étape
CT_VirtualAbstractStep* TUFR_StepTutorial04::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette étape
    return new TUFR_StepTutorial04(dataInit);
}

//////////////////// PROTECTED //////////////////

// Création et affiliation des modèles IN
void TUFR_StepTutorial04::createInResultModelListProtected()
{
    // Déclaration et création du modèle de groupe racine
    // Groupe optionnel, pouvant se trouver à tout endroit de la hiérachie
    CT_InZeroOrMoreGroupModel *groupR = new CT_InZeroOrMoreGroupModel();

    // Groupe contenant le cluster (indexé car nous allons y ajouter quelque chose)
    CT_InStandardGroupModel *group = new CT_InStandardGroupModel(DEF_SearchInGroup);

    // le groupe doit contenir un ItemDrawable de type nuage de points
    CT_InStandardItemDrawableModel *item = new CT_InStandardItemDrawableModel(DEF_SearchInPointCluster,
                                                                              CT_PointCluster::staticGetType(),
                                                                              tr("Groupe de points"));

    group->addItem(item);
    groupR->addGroup(group);

    // le résultat est du type ResultGroupToCopy pour que le système face une copie du
    // résultat d'entrée, que nous complèterons
    addInResultModel(new CT_InResultModelGroupToCopy(DEF_SearchInResult,
                                                     groupR,
                                                     tr("Groupe(s) de points")));
}

// Création et affiliation des modèles OUT
void TUFR_StepTutorial04::createOutResultModelListProtected()
{
    // on récupère le résultat modèle d'entrée à copier défini dans "createInResultModelListProtected()"
    CT_InResultModelGroupToCopy *intResModelToCopy = (CT_InResultModelGroupToCopy*)getInResultModel(DEF_SearchInResult);

    // On créée le modèle d'item (CT_ReferencePoint) à ajouter
    CT_OutStandardItemDrawableModel *refPointModel = new CT_OutStandardItemDrawableModel("", new CT_ReferencePoint(), tr("Barycentre"));

    // On créée une liste d'action à executer sur la copie du modèle
    QList<CT_AbstractOutModelCopyAction*> actions;


    // On ajoute une action permettant d'ajouter le modèle d'item créé
    // Cette action prend en paramètre à générateur de nom automatique : _outRefPointModelName
    actions << new CT_OutModelCopyActionAddModelItemInGroup(DEF_SearchInGroup,
                                                            refPointModel,
                                                            _outRefPointModelName);

    // On ajoute le modèle de sortie modifier
    // En réalité cette méthode va faire effectivement la copie du résultat d'entrée
    addOutResultModelCopy(intResModelToCopy, actions);

}

// Création semi-automatique de la boite de dialogue de paramétrage de l'étape
void TUFR_StepTutorial04::createPostConfigurationDialog()
{
    // Pas de boite de dialogue
}

void TUFR_StepTutorial04::compute()
{
    // on récupère le résultat copié (résultat de sortie)
    CT_ResultGroup *outRes = getOutResultList().first();

    /********* RECHERCHE DES MODELES DE SORTIE ************/

    // on utilise le modèle du résultat de sortie pour rechercher le modèle de sortie du CT_ReferencePoint.
    // En utilisant bien sur le nom automatique généré lors de l'ajout
    CT_OutAbstractItemDrawableModel *outRefPointModel = (CT_OutAbstractItemDrawableModel*) getOutModelForCreation(outRes, _outRefPointModelName.completeName());

    /********* RECHERCHE DES MODELES D'ENTREE ************/
    // On récupère les modèles d'entrée du groupe et du groupe de points
    CT_InAbstractGroupModel *inGroupModel = (CT_InAbstractGroupModel*)getInModelForResearchIfUseCopy(DEF_SearchInResult, DEF_SearchInGroup);
    CT_InAbstractItemDrawableModel *inPointClusterModel = (CT_InAbstractItemDrawableModel*)getInModelForResearchIfUseCopy(DEF_SearchInResult, DEF_SearchInPointCluster);

    /**************************************************/
    // on va rechercher tous les groupes contenant des nuages de points (qui ont été choisi par l'utilisateur)
    for ( CT_AbstractItemGroup *group = outRes->beginGroup(inGroupModel)
        ; group != NULL  && !isStopped()
        ; group = outRes->nextGroup() )
    {
        // On récupère le groupe de points à l'aide du modèle
        const CT_PointCluster *item = (const CT_PointCluster*) group->findFirstItem(inPointClusterModel);

        // On vérifie qu'il existe
        if(item != NULL)
        {
            // On récupère le barycentre auto-calculé du groupe de points
            const CT_PointClusterBarycenter barycentre = item->getBarycenter();

            // coordonnées du barycentre
            float xref = barycentre.x();
            float yref = barycentre.y();
            float zref = barycentre.z();

            // calcul du bufferXY
            // Maximum de la distance point/refPoint pour chaque segment
            float buffer = 0;
            const CT_AbstractPointCloud *cloud = item->getPointCloud();
            const CT_AbstractPointCloudIndex *cloudIndex = item->getPointCloudIndex();

            int indexsize = cloudIndex->indexSize();
            for (int i = 0 ; i < indexsize ; i++)
            {
                int index = cloudIndex->operator [](i);
                const CT_Point point = cloud->operator [](index);
                float distance = pow(xref-point.x, 2) + pow(yref-point.y, 2);
                if (distance > buffer) {buffer = distance;}
            }
            if (buffer > 0) {buffer = sqrt(buffer);}

            // et on ajoute un referencePoint
            CT_ReferencePoint *refPoint = new CT_ReferencePoint(outRefPointModel, item->id(), outRes, xref, yref, zref, buffer);
            group->addItemDrawable(refPoint);

        }
    }

}
