// Inclusion du fichier d'entête
#include "tufr_steptutorial01.h"

// Inclusion des modèles de résultats in et out
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"
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
#include "ct_pointcloudstdvector.h"
#include "ct_element/ct_axisalignedboundingbox.h"

#include "qdebug.h" // pour afficher des messages à la console

#include "math.h"

// Alias des chaines de caractères pour l'indexation du modèle out
#define DEF_SearchOutResult "r"
#define DEF_SearchOutGroup  "g"
#define DEF_SearchOutScene  "sc"

// Constructeur : appel du constructeur de la classe mère
//                Initialisation des paramètres (valeurs par défaut)
TUFR_StepTutorial01::TUFR_StepTutorial01(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
    _n = 10000;
    _xmin = -10;
    _xmax = 10;
    _ymin = -10;
    _ymax = 10;
    _zmin = -1;
    _zmax = 20;
}

// Description de l'étape (tooltip du menu contextuel)
QString TUFR_StepTutorial01::getStepDescription() const
{
    return "Génère une scène";
}

// Méthode de recopie de l'étape
CT_VirtualAbstractStep* TUFR_StepTutorial01::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette étape
    return new TUFR_StepTutorial01(dataInit);
}

//////////////////// PROTECTED //////////////////

// Création et affiliation des modèles IN
void TUFR_StepTutorial01::createInResultModelListProtected()
{
    // Déclaration et création du modèle de résultat : Pas de résultat
    CT_InResultModelNotNeedInputResult *resultModel;
    resultModel = new CT_InResultModelNotNeedInputResult();

    // Ajout du modèle de résultat dans cette étape
    addInResultModel(resultModel);
}

// Création et affiliation des modèles OUT
void TUFR_StepTutorial01::createOutResultModelListProtected()
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
                             "Scène générée",
                             "Scène générée");

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
                                             "GeneratedPlot",
                                             "Scène générée",
                                             "Scène générée");

    // Ajout du modèle de résultat séquenciellement dans cette étape
    // En réalité cette méthode génère aussi un resulat auquel est attaché ce modèle
    // Les addOutResultModel susccessifs, ajoutent des résultats dans l'ordre à la OutResultList
    addOutResultModel(resultModel);
}

// Création semi-automatique de la boite de dialogue de paramétrage de l'étape
void TUFR_StepTutorial01::createPostConfigurationDialog()
{
    // Pas de boite de dialogue
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    // Ajout de contrôles unitaires de paramètrages à la boite (un par paramètre en général)
    configDialog->addInt("Nombre de points aléatoires à générer", "", 1, 1000000, _n);
    configDialog->addDouble("X minimum :", "m", -10000, 10000, 2, _xmin);
    configDialog->addDouble("X maximum :", "m", -10000, 10000, 2, _xmax);
    configDialog->addDouble("Y minimum :", "m", -10000, 10000, 2, _ymin);
    configDialog->addDouble("Y maximum :", "m", -10000, 10000, 2, _ymax);
    configDialog->addDouble("Z minimum :", "m", -10000, 10000, 2, _zmin);
    configDialog->addDouble("Z maximum :", "m", -10000, 10000, 2, _zmax);
}

void TUFR_StepTutorial01::compute()
{
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

    CT_PointCloudStdVector* newCloud = new CT_PointCloudStdVector(_n);
    CT_PointCloudIndexVector *newIndex = new CT_PointCloudIndexVector(_n);

    double tmp;
    if (_xmin > _xmax) {tmp = _xmin; _xmin = _xmax; _xmax = tmp;}
    if (_ymin > _ymax) {tmp = _ymin; _ymin = _ymax; _ymax = tmp;}
    if (_zmin > _zmax) {tmp = _zmin; _zmin = _zmax; _zmax = tmp;}

    for (int i = 0 ; i < _n ; i++)
    {
        CT_Point& point = newCloud->operator [](i);
        newIndex->operator [](i) = i;
        point.x = (rand()/(double)RAND_MAX ) * (_xmax-_xmin) + _xmin;
        point.y = (rand()/(double)RAND_MAX ) * (_ymax-_ymin) + _ymin;
        point.z = (rand()/(double)RAND_MAX ) * (_zmax-_zmin) + _zmin;
        point.intensity = 50000;

    }


    CT_StandardItemGroup* newGroup = new CT_StandardItemGroup(*groupModel, 0, outResult);
    CT_AxisAlignedBoundingBox* boundingBox = new CT_AxisAlignedBoundingBox(QVector3D(_xmin, _ymin, _zmin), QVector3D(_xmax, _ymax, _zmax));
    CT_Scene *newScene = new CT_Scene(sceneItemModel, 0, outResult, newCloud, newIndex, boundingBox);
    newScene->setAutoDeletePointCloud(true);
    newScene->setAutoDeletePointCloudIndex(true);

    newGroup->addItemDrawable(newScene);
    outResult->addGroup(newGroup);

}
