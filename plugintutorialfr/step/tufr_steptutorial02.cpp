// Inclusion du fichier d'entête
#include "tufr_steptutorial02.h"

// Inclusion des modèles de résultats in et out
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

// Inclusion des modèles de groupes in et out
#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"

// Inclusion des modèles d'items in et out
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
TUFR_StepTutorial02::TUFR_StepTutorial02(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
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
QString TUFR_StepTutorial02::getStepDescription() const
{
    return "Génère une scène";
}

// Méthode de recopie de l'étape
CT_VirtualAbstractStep* TUFR_StepTutorial02::createNewInstance(CT_StepInitializeData &dataInit)
{
    // créée une copie de cette étape
    return new TUFR_StepTutorial02(dataInit);
}

//////////////////// PROTECTED //////////////////

// Création et affiliation des modèles IN
void TUFR_StepTutorial02::createInResultModelListProtected()
{
    // Déclaration et création du modèle de résultat : Pas de résultat
    CT_InResultModelNotNeedInputResult *resultModel = new CT_InResultModelNotNeedInputResult();

    // Ajout du modèle de résultat dans cette étape
    addInResultModel(resultModel);
}

// Création et affiliation des modèles OUT
void TUFR_StepTutorial02::createOutResultModelListProtected()
{
    // Déclaration et création du modèle de groupe racine
    CT_OutStandardGroupModel *groupModel = new CT_OutStandardGroupModel(DEF_SearchOutGroup);

    // Déclaration et création du modèle d'item scène
    // Le 1ier paramètre est l'alias d'indexation de ce modèle
    // Le 2ieme paramètre spécifie la classe qui sera crée, en fournissant une instance de cette classe
    // Le 3ieme paramètre donne un nom à ce modèle d'item affichable par l'interface
    CT_OutStandardItemDrawableModel *sceneItemModel;
    sceneItemModel = new CT_OutStandardItemDrawableModel(DEF_SearchOutScene,
                                                         new CT_Scene(),
                                                         "Scène générée");

    // On ajoute le modèle d'item (scène) au modèle du groupe racine
    groupModel->addItem(sceneItemModel);

    // Déclaration et création du modèle de résultat
    // Le 1er paramètre est l'alias d'indexation du modèle de résultat
    // Le 2ieme paramètre est le modèle du groupe racine
    // Le 3ieme paramètre donne un nom au résultat
    CT_OutResultModelGroup *resultModel = new CT_OutResultModelGroup(DEF_SearchOutResult,
                                                                     groupModel,
                                                                     "Scène générée");

    // Ajout du modèle de résultat séquenciellement dans cette étape
    // En réalité cette méthode génère aussi un resulat auquel est attaché ce modèle
    // Les addOutResultModel susccessifs, ajoutent des résultats dans l'ordre à la OutResultList
    addOutResultModel(resultModel);
}

// Création semi-automatique de la boite de dialogue de paramétrage de l'étape
void TUFR_StepTutorial02::createPostConfigurationDialog()
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

void TUFR_StepTutorial02::compute()
{
    // RESULTATS OUT
    // Récupération de la liste des résultats de sortie créés par createOutResultModelListProtected()
    // L'ordre dans cette liste est celui des addOutResultModel() successifs
    QList<CT_ResultGroup*> outResultList = getOutResultList();

    // Ici on a fait un seul addOutResultModel(), donc on a un seul résultat à récupérer : le premier
    CT_ResultGroup* outResult = outResultList.at(0);

    // On va récupérer les modèles OUT des groupes et items que l'on souhaite créer
    CT_OutStandardGroupModel* groupModel = (CT_OutStandardGroupModel*) getOutModelForCreation(outResult, DEF_SearchOutGroup);
    CT_OutStandardItemDrawableModel* sceneItemModel = (CT_OutStandardItemDrawableModel*) getOutModelForCreation(outResult,
                                                                                                                DEF_SearchOutScene);
    // Création du nuage de points et du vecteur d'index pour la nouvelle scène
    CT_PointCloudStdVector* newCloud = new CT_PointCloudStdVector(_n);
    CT_PointCloudIndexVector *newIndex = new CT_PointCloudIndexVector(_n);

    // Vérification du bon ordonnencement des coordonnées min et max
    double tmp;
    if (_xmin > _xmax) {tmp = _xmin; _xmin = _xmax; _xmax = tmp;}
    if (_ymin > _ymax) {tmp = _ymin; _ymin = _ymax; _ymax = tmp;}
    if (_zmin > _zmax) {tmp = _zmin; _zmin = _zmax; _zmax = tmp;}

    // Génération aléatoire des points
    for (int i = 0 ; i < _n ; i++)
    {
        CT_Point& point = newCloud->operator [](i);
        newIndex->operator [](i) = i;
        point.x = (rand()/(double)RAND_MAX ) * (_xmax-_xmin) + _xmin;
        point.y = (rand()/(double)RAND_MAX ) * (_ymax-_ymin) + _ymin;
        point.z = (rand()/(double)RAND_MAX ) * (_zmax-_zmin) + _zmin;
        point.intensity = 50000;

    }

    // Création du groupe contenant la scène (avec le modèle correspondant
    CT_StandardItemGroup* newGroup = new CT_StandardItemGroup(groupModel, 0, outResult);

    // Création de la boite englobante avec les limites de la scène
    CT_AxisAlignedBoundingBox* boundingBox = new CT_AxisAlignedBoundingBox(QVector3D(_xmin, _ymin, _zmin), QVector3D(_xmax, _ymax, _zmax));

    // Création de la scène
    CT_Scene *newScene = new CT_Scene(sceneItemModel, 0, outResult, newCloud, newIndex, boundingBox);

    // C'est une nouvelle scène : elle devra se charger le l'effacement des points tout juste créés
    newScene->setAutoDeletePointCloud(true);
    newScene->setAutoDeletePointCloudIndex(true);

    // Ajout de la scène au groupe
    newGroup->addItemDrawable(newScene);

    // Ajout du groupe au résultat
    outResult->addGroup(newGroup);
}
