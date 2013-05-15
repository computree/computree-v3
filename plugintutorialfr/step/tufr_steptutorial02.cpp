// Inclusion du fichier d'entête
#include "tufr_steptutorial02.h"

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

// Inclusion des CT_ItemDrawables utiles dans l'étape
#include "ct_itemdrawable/ct_scene.h"

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
#define DEF_SearchOutScene  "sc"

// Constructeur : appel du constructeur de la classe mère
//                Initialisation des paramètres (valeurs par défaut)
TUFR_StepTutorial02::TUFR_StepTutorial02(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _zmin = 0;
    _zmax = 1;
}

// Description de l'étape (tooltip du menu contextuel)
QString TUFR_StepTutorial02::getStepDescription() const
{
    return "Extrait une placette entre zmin et zmax";
}

// Méthode de recopie de l'étape
CT_VirtualAbstractStep* TUFR_StepTutorial02::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette étape
    return new TUFR_StepTutorial02(dataInit);
}

//////////////////// PROTECTED //////////////////

// Création et affiliation des modèles IN
void TUFR_StepTutorial02::createInResultModelListProtected()
{
    // Déclaration et création du modèle de groupe racine
    // Ici on utilise un CT_InOneOrMoreGroupModel, pour permettre
    // d'utiliser une scène n'importe où dans une hiérachie des groupes
    CT_InOneOrMoreGroupModel *groupModel;
    groupModel = new CT_InOneOrMoreGroupModel();

    // Déclaration et création du modèle d'item scène
    // Le 1er paramètre spécifie la classe souhaitée pour l'Item NomDeClasse::staticGetType()
    // Le 2ieme paramètre indique si plusieurs candidats peuvent être séléctionnés dans le résultat d'entrée
    // Ici on oblige à n'en choisir qu'un seul
    // Le 3ieme paramètre est l'alias d'indexation de ce modèle
    // Le 4ieme paramètre indique si cet item est obligatoire (c'est le cas ici) ou facultatif
    // N.B. : il faut au moins un item obligatoire dans le cas général
    // Le 5ieme paramètre donne un nom à ce modèle d'item affichable par l'interface
    // Le 6ieme paramètre donne une description à ce modèle d'item affichable par l'interface
    CT_InStandardItemDrawableModel *sceneItemModel;
    sceneItemModel = new CT_InStandardItemDrawableModel(CT_Scene::staticGetType(),
                             CT_InStandardItemDrawableModel::C_ChooseOneIfMultiple,
                             DEF_SearchInScene,
                             CT_InStandardItemDrawableModel::F_IsObligatory,
                             "Scène",
                             "Objet CT_Scene");

    // On ajoute le modèle d'item (scène) au modèle du groupe racine
    groupModel->addItem(sceneItemModel);

    // Déclaration et création du modèle de résultat
    CT_InResultModelGroup *resultModel;
    // Le 1er paramètre est le modèle du groupe racine
    // Le 2ieme paramètre est l'alias d'indexation du modèle de résultat
    // Le 3ieme paramètre indique si on doit chercher le résultat d'entrée
    // récurssivement en remontant l'arbre des étapes (ici non : false)
    // Le 4ieme paramètre donne une description à ce modèle de résultat, affichable par l'interface
    // Le 5ieme paramètre donne un nom à ce modèle de résultat, affichable par l'interface
    resultModel = new CT_InResultModelGroup(groupModel,
                                            DEF_SearchInResult,
                                            false,
                                            "Un résultat avec une/des scènes",
                                            "Résultat avec une Scène");

    // Ajout du modèle de résultat séquenciellement dans cette étape
    // En réalité cette méthode génère aussi un resulat auquel est attaché ce modèle
    // Les addInResultModel susccessifs, ajoutent des résultats dans l'ordre à la InResultList
    addInResultModel(resultModel);
}

// Création et affiliation des modèles OUT
void TUFR_StepTutorial02::createOutResultModelListProtected()
{
    // Déclaration et création du modèle de groupe racine
    CT_OutStandardGroupModel *groupModel;
    groupModel = new CT_OutStandardGroupModel(DEF_SearchOutGroup);

    // Déclaration et création du modèle d'item scène
    // Le 1er paramètre spécifie la classe qui sera crée, en fournissant une instance de cette classe
    // Le 2ieme paramètre est l'alias d'indexation de ce modèle
    // Le 3ieme paramètre donne un nom à ce modèle d'item affichable par l'interface
    // Le 4ieme paramètre donne une description à ce modèle d'item affichable par l'interface
    CT_OutStandardItemDrawableModel *sceneItemModel;
    sceneItemModel = new CT_OutStandardItemDrawableModel(new CT_Scene(),
                             DEF_SearchOutScene,
                             "Scène extraite",
                             "Scène extraite");

    // On ajoute le modèle d'item (scène) au modèle du groupe racine
    groupModel->addItem(sceneItemModel);

    // Déclaration et création du modèle de résultat
    CT_OutResultModelGroup *resultModel;
    // Le 1er paramètre est le modèle du groupe racine
    // Le 2ieme paramètre est l'alias d'indexation du modèle de résultat
    // Le 3ieme paramètre donne un nom au résultat
    // Le 4ieme paramètre donne un nom à ce modèle de résultat affichable par l'interface
    // Le 5ieme paramètre donne une decription à ce modèle de résultat affichable par l'interface
    resultModel = new CT_OutResultModelGroup(groupModel,
                                             DEF_SearchOutResult,
                                             "ExtractedPlot",
                                             "Scène extraite",
                                             "Scène extraite");

    // Ajout du modèle de résultat séquenciellement dans cette étape
    // En réalité cette méthode génère aussi un resulat auquel est attaché ce modèle
    // Les addOutResultModel susccessifs, ajoutent des résultats dans l'ordre à la OutResultList
    addOutResultModel(resultModel);
}

// Création semi-automatique de la boite de dialogue de paramétrage de l'étape
void TUFR_StepTutorial02::createPostConfigurationDialog()
{
    // Création de la boite de dialog en elle-même
    // La méthode newStandardPostConfigurationDialog crée la boite et l'affecte à l'étape en cours
    // Ainsi la gestion et la suppression de cette boite sont gérés automatiquement
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    // Ajout de contrôles unitaires de paramètrages à la boite (un par paramètre en général)
    configDialog->addDouble("Z minimum :", "m", -10000, 10000, 2, _zmin);
    configDialog->addDouble("Z maximum :", "m", -10000, 10000, 2, _zmax);
}

void TUFR_StepTutorial02::compute()
{
    // RESULTATS IN
    // Récupération de la liste des résultats d'entrée séléctionné par createInResultModelListProtected()
    // L'ordre dans cette liste est celui des addInResultModel() successifs
    QList<CT_ResultGroup*> inResultList = getInputResults();

    // Ici on a fait un seul addInResultModel(), donc on a un seul résultat à récupérer : le premier
    CT_ResultGroup* inResult;
    inResult = inResultList.at(0);

    // On va récupérer le modèle OUT de ce résultat d'entrée
    CT_OutAbstractResultModel* inResultOutModel = inResult->model();

    // On va récupérer les modèles IN des groupes et items que l'on souhaite indexer
    CT_InStandardItemDrawableModel *inSceneModel = (CT_InStandardItemDrawableModel*) getInModel(*inResultOutModel, DEF_SearchInScene);


    // RESULTATS OUT
    // Récupération de la liste des résultats de sortie créés par createOutResultModelListProtected()
    // L'ordre dans cette liste est celui des addOutResultModel() successifs
    QList<CT_ResultGroup*> outResultList = getOutResultList();

    // Ici on a fait un seul addOutResultModel(), donc on a un seul résultat à récupérer : le premier
    CT_ResultGroup* outResult;
    outResult = outResultList.at(0);

    // On va récupérer le modèle OUT de ce résultat de sortie
    CT_OutAbstractResultModel* outResultOutModel = outResult->model();

    // On va récupérer les modèles OUT des groupes et items que l'on souhaite créer
    CT_OutStandardGroupModel* groupModel = (CT_OutStandardGroupModel*) getOutModel(*outResultOutModel, DEF_SearchOutGroup);
    CT_OutStandardItemDrawableModel* sceneItemModel = (CT_OutStandardItemDrawableModel*) getOutModel(*outResultOutModel, DEF_SearchOutScene);


    // ALGORITHME
    // Création d'un itérateur sur les items ayant pour modèle : inSceneModel
    if (inResult->recursiveBeginIterateItems(*inSceneModel))
    {
        // récupération du premier groupe contenant un item de ce type
        CT_AbstractItemGroup *itemGroup = inResult->recursiveNextItem();

        // on s'assure qu'il exite
        if (itemGroup!=NULL)
        {
            // On récupère la première (la seule ici) scène correspondant au modèle dans ce groupe
            const CT_Scene *scene = (CT_Scene*) itemGroup->findFirstItem(*inSceneModel);

            // On s'assure qu'elle existe
            if (scene != NULL)
            {
                const CT_AbstractPointCloud *pointcloud = scene->getPointCloud();
                const CT_AbstractPointCloudIndex *pointcloudIndex = scene->getPointCloudIndex();

                CT_PointCloudIndexVector *newIndex = new CT_PointCloudIndexVector();

                double xmin = LONG_MAX;
                double xmax = LONG_MIN;
                double ymin = LONG_MAX;
                double ymax = LONG_MIN;
                double zmin = LONG_MAX;
                double zmax = LONG_MIN;

                int cloudSize = pointcloudIndex->indexSize();

                for (int i = 0 ; i < cloudSize ; ++i)
                {
                    const CT_Point &point = (*pointcloud)[(*pointcloudIndex)[i]];

                    if ((point.z >= _zmin) && (point.z <= _zmax))
                    {
                        newIndex->addIndex((*pointcloudIndex)[i]);

                        if (point.x < xmin) {xmin = point.x;}
                        if (point.x > xmax) {xmax = point.x;}
                        if (point.y < ymin) {ymin = point.y;}
                        if (point.y > ymax) {ymax = point.y;}
                        if (point.z < zmin) {zmin = point.z;}
                        if (point.z > zmax) {zmax = point.z;}
                     }
                }

                CT_StandardItemGroup* newGroup = new CT_StandardItemGroup(*groupModel, 0, outResult);
                CT_Scene *newScene = new CT_Scene(sceneItemModel, 0, outResult, (CT_AbstractPointCloud*) pointcloud, newIndex, new CT_AxisAlignedBoundingBox(QVector3D(xmin, ymin, zmin), QVector3D(xmax, ymax, zmax)));
                newScene->setAutoDeletePointCloud(false);
                newScene->setAutoDeletePointCloudIndex(true);

                newGroup->addItemDrawable(newScene);
                outResult->addGroup(newGroup);
            }
        }
    } else {
        qDebug() << "Problème d'itérateur";
    }





}
