// Inclusion du fichier d'entête
#include "tufr_steptutorial03.h"

// Inclusion des modèles de résultats in et out
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

// Inclusion des modèles de groupes in et out
#include "ct_itemdrawable/model/inModel/ct_inoneormoregroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"

// Inclusion des modèles d'items in et out
#include "ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h"

// Inclusion de la classe de résultat standard
#include "ct_result/ct_resultgroup.h"

// Inclusion des CT_ItemDrawables utiles dans l'étape
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointcluster.h"

// Inclusion des autres éléments nécessaires à la création d'une scène
#include "ct_pointcloudindexvector.h"
#include "ct_element/ct_axisalignedboundingbox.h"

#include "qdebug.h" // pour afficher des messages à la console

#include "math.h"

// Alias des chaines de caractères pour l'indexation des modèles in et out
#define DEF_SearchInResult  "r"
#define DEF_SearchInScene   "sc"

#define DEF_SearchOutResult "r"
#define DEF_SearchOutGroup  "g"
#define DEF_SearchOutCluster  "cl"

// Constructeur : appel du constructeur de la classe mère
//                Initialisation des paramètres (valeurs par défaut)
TUFR_StepTutorial03::TUFR_StepTutorial03(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _nx = 3;
    _ny = 3;
    _nz = 3;
}

// Description de l'étape (tooltip du menu contextuel)
QString TUFR_StepTutorial03::getStepDescription() const
{
    return "Extrait une placette entre zmin et zmax";
}

// Méthode de recopie de l'étape
CT_VirtualAbstractStep* TUFR_StepTutorial03::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette étape
    return new TUFR_StepTutorial03(dataInit);
}

//////////////////// PROTECTED //////////////////

// Création et affiliation des modèles IN
void TUFR_StepTutorial03::createInResultModelListProtected()
{
    // Déclaration et création du modèle de groupe racine
    // Ici on utilise un CT_InOneOrMoreGroupModel, pour permettre
    // d'utiliser une scène n'importe où dans une hiérachie des groupes
    CT_InOneOrMoreGroupModel *groupModel;
    groupModel = new CT_InOneOrMoreGroupModel();

    // Déclaration et création du modèle d'item scène
    // Le 1er paramètre est l'alias d'indexation de ce modèle
    // Le 2ieme paramètre spécifie la classe souhaitée pour l'Item NomDeClasse::staticGetType()
    // Le 3ieme paramètre donne un nom à ce modèle d'item affichable par l'interface
    CT_InStandardItemDrawableModel *sceneItemModel;
    sceneItemModel = new CT_InStandardItemDrawableModel(DEF_SearchInScene,
                                                        CT_Scene::staticGetType(),
                                                        "Scène");

    // On ajoute le modèle d'item (scène) au modèle du groupe racine
    groupModel->addItem(sceneItemModel);

    // Déclaration et création du modèle de résultat
    // Le 1er paramètre est l'alias d'indexation du modèle de résultat
    // Le 2ieme paramètre est le modèle du groupe racine
    // Le 3ieme paramètre donne un nom au résultat
    CT_InResultModelGroup *resultModel = new CT_InResultModelGroup(DEF_SearchInResult,
                                                                   groupModel,
                                                                   "Une/des scènes");

    // Ajout du modèle de résultat séquenciellement dans cette étape
    addInResultModel(resultModel);


  }

  // Création et affiliation des modèles OUT
  void TUFR_StepTutorial03::createOutResultModelListProtected()
  {
      // Déclaration et création du modèle de groupe racine
      CT_OutStandardGroupModel *groupModel = new CT_OutStandardGroupModel(DEF_SearchOutGroup);

      // Déclaration et création du modèle d'item scène
      // Le 1er paramètre est l'alias d'indexation de ce modèle
      // Le 2ieme paramètre spécifie la classe qui sera crée, en fournissant une instance de cette classe
      // Le 3ieme paramètre donne un nom à ce modèle d'item affichable par l'interface
      CT_OutStandardItemDrawableModel *clusterItemModel;
      clusterItemModel = new CT_OutStandardItemDrawableModel(DEF_SearchOutCluster,
                                                           new CT_PointCluster(),
                                                           "Groupe de points");

      // On ajoute le modèle d'item (scène) au modèle du groupe racine
      groupModel->addItem(clusterItemModel);

      // Déclaration et création du modèle de résultat
      // Le 1er paramètre est l'alias d'indexation du modèle de résultat
      // Le 2ieme paramètre est le modèle du groupe racine
      // Le 3ieme paramètre donne un nom au résultat
      CT_OutResultModelGroup *resultModel = new CT_OutResultModelGroup(DEF_SearchOutResult,
                                                                       groupModel,
                                                                       "Clusters");

      // Ajout du modèle de résultat séquenciellement dans cette étape
      addOutResultModel(resultModel);
  }

  // Création semi-automatique de la boite de dialogue de paramétrage de l'étape
  void TUFR_StepTutorial03::createPostConfigurationDialog()
  {
      // Création de la boite de dialog en elle-même
      // La méthode newStandardPostConfigurationDialog crée la boite et l'affecte à l'étape en cours
      // Ainsi la gestion et la suppression de cette boite sont gérés automatiquement
      CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

      // Ajout de contrôles unitaires de paramètrages à la boite (un par paramètre)
      configDialog->addInt("Nombre clusters selon x", "", 1, 100, _nx);
      configDialog->addInt("Nombre clusters selon x", "", 1, 100, _ny);
      configDialog->addInt("Nombre clusters selon x", "", 1, 100, _nz);
  }

  void TUFR_StepTutorial03::compute()
  {
      // RESULTATS IN
      // Récupération de la liste des résultats d'entrée séléctionné par createInResultModelListProtected()
      // L'ordre dans cette liste est celui des addInResultModel() successifs
      QList<CT_ResultGroup*> inResultList = getInputResults();

      // Ici on a fait un seul addInResultModel(), donc on a un seul résultat à récupérer : le premier
      CT_ResultGroup* inResult = inResultList.at(0);

      // On va récupérer les modèles IN des groupes et items que l'on souhaite indexer
      CT_InStandardItemDrawableModel *inSceneModel = (CT_InStandardItemDrawableModel*) getInModelForResearch(inResult, DEF_SearchInScene);


      // RESULTATS OUT
      // Récupération de la liste des résultats de sortie créés par createOutResultModelListProtected()
      // L'ordre dans cette liste est celui des addOutResultModel() successifs
      QList<CT_ResultGroup*> outResultList = getOutResultList();

      // Ici on a fait un seul addOutResultModel(), donc on a un seul résultat à récupérer : le premier
      CT_ResultGroup* outResult = outResultList.at(0);

      // On va récupérer les modèles OUT des groupes et items que l'on souhaite créer
      CT_OutStandardGroupModel* groupModel = (CT_OutStandardGroupModel*) getOutModelForCreation(outResult, DEF_SearchOutGroup);
      CT_OutStandardItemDrawableModel* clusterItemModel = (CT_OutStandardItemDrawableModel*) getOutModelForCreation(outResult, DEF_SearchOutCluster);

      // Création d'un tableau qui stockera les clusters créés
      QVector<CT_PointCluster*> clusterVector;
      clusterVector.resize(_nx*_ny*_nz);

      // ALGORITHME
      // Création d'un itérateur sur les items ayant pour modèle : inSceneModel
      if (inResult->recursiveBeginIterateItems(*inSceneModel))
      {
          // On récupère la première (la seule ici) scène correspondant au modèle dans ce groupe
          // HDE si on s'attend à plusieurs scènes, on peut mettre un for ou un while.
          //CT_Scene *scene;
          //while (scene = (CT_scene *scene) inResult->recursiveNextItem()) !=NULL)
          //{
          //}
          // ou for et meme on peut mettre le if dedans dans le for puisque c'est une init
          const CT_Scene *scene = (CT_Scene*) inResult->recursiveNextItem();

          // On s'assure qu'elle existe
          if (scene != NULL)
          {

              // Récupération des limites min et max en (x,y,z) de la scène d'entrée
              QVector3D min, max;
              scene->getBoundingShape()->getBoundingBox(min, max);

              // Calcul de la taille des clusters
              float xClusterSize = (max.x() - min.x())/_nx;
              float yClusterSize = (max.y() - min.y())/_ny;
              float zClusterSize = (max.z() - min.z())/_nz;

              // Recupération du nuage de points et des index de la scène
              const CT_AbstractPointCloud *pointcloud = scene->getPointCloud();
              const CT_AbstractPointCloudIndex *pointcloudIndex = scene->getPointCloudIndex();
              int cloudSize = pointcloudIndex->indexSize();

              // Parcours des points de la scène
              for (int i = 0 ; i < cloudSize ; ++i)
              {
                  const CT_Point &point = (*pointcloud)[(*pointcloudIndex)[i]];

                  // Calcul des indices x, y et z du cluster où affecter le point
                  int indiceX = (int) floor((point.x - min.x()) / xClusterSize);
                  int indiceY = (int) floor((point.y - min.y()) / yClusterSize);
                  int indiceZ = (int) floor((point.z - min.z()) / zClusterSize);

                  // Indice dans le tableau de clusters
                  int indice = indiceX + indiceY*_nx + indiceZ*_nx*_ny;

                  // Si le cluster n'existe pas on le créée
                  if (clusterVector[indice]==NULL)
                  {
                      clusterVector[indice] = new CT_PointCluster(clusterItemModel,
                                                                     indice,
                                                                     outResult,
                                                                     (CT_AbstractPointCloud*) pointcloud,
                                                                     new CT_AxisAlignedBoundingBox());

                      // En s'assurant que sa boite englobante se mettra à jour lors de l'ajout de points
                      clusterVector[indice]->setUpdateBoundingShapeAutomatically(true);
                  }

                  // Ajout du point au cluster (son index en fait)
                  clusterVector.at(indice)->addPoint((*pointcloudIndex)[i]);
              }

              // On parcours les clusters créés
              for (int i = 0 ; i < clusterVector.size() ; i++)
              {
                  CT_PointCluster* cluster = clusterVector[i];
                  if (cluster!=NULL)
                  {
                      // Pour chaque cluster, ajout à un groupe et ajout du groupe au résultat
                      // Avec les modèles OUT adéquats
                      CT_StandardItemGroup* group = new CT_StandardItemGroup(groupModel,
                                                                             cluster->id(),
                                                                             outResult);
                      group->addItemDrawable(cluster);
                      outResult->addGroup(group);
                  }
              }
          }
      }
  }
