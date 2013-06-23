// Inclusion du fichier d'entête
#include "tufr_steptutorial01.h"

// Inclusion des modèles de résultats in et out
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

// Inclusion des modèles de groupes in et out
#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"

// Inclusion de la classe de résultat standard
#include "ct_result/ct_resultgroup.h"

// Inclusion des CT_ItemDrawables utiles dans l'étape
#include "ct_itemdrawable/ct_scene.h"

#include "qdebug.h" // pour afficher des messages à la console

// Alias des chaines de caractères pour l'indexation du modèle out
#define DEF_SearchOutResult "r"
#define DEF_SearchOutGroup  "g"

// Constructeur : appel du constructeur de la classe mère
//                Initialisation des paramètres (valeurs par défaut)
TUFR_StepTutorial01::TUFR_StepTutorial01(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
    _intParam = 10;
    _doubleParam = 50.6;
    _boolParam = true;
    _listParam = "choix 2";
    _choiceParam = 1;
    _fileNameParam = "";
}

// Description de l'étape (tooltip du menu contextuel)
QString TUFR_StepTutorial01::getStepDescription() const
{
    return "Etape de démonstration des types de paramètres";
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
    CT_InResultModelNotNeedInputResult *resultModel = new CT_InResultModelNotNeedInputResult();

    // Ajout du modèle de résultat dans cette étape
    addInResultModel(resultModel);
}

// Création et affiliation des modèles OUT
void TUFR_StepTutorial01::createOutResultModelListProtected()
{

    // Déclaration et création du modèle de groupe racine
    CT_OutStandardGroupModel *groupModel = new CT_OutStandardGroupModel(DEF_SearchOutGroup);

    // Déclaration et création du modèle de résultat  
    // Le 1er paramètre est l'alias d'indexation du modèle de résultat
    // Le 2ieme paramètre est le modèle du groupe racine
    // Le 3ieme paramètre donne un nom au résultat
    CT_OutResultModelGroup *resultModel = new CT_OutResultModelGroup(DEF_SearchOutResult, groupModel, "Résultat vide");

    // Ajout du modèle de résultat séquenciellement dans cette étape
    addOutResultModel(resultModel);
}

// Création semi-automatique de la boite de dialogue de paramétrage de l'étape
void TUFR_StepTutorial01::createPostConfigurationDialog()
{
    // Pas de boite de dialogue
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    // Ajout de contrôles unitaires de paramètrages à la boite (un par paramètre en général)
    // Paramètre prenant une valeur entière
    configDialog->addInt("Paramètre entier", "unité", 0, 100, _intParam);

    // Paramètre prenant une valeur double
    configDialog->addDouble("Paramètre double", "unité", 0, 100, 2, _doubleParam);

    // Paramètre prenant une valeur booléenne
    configDialog->addBool("Paramètre booléen", "labelAfter", "labelCheckBox", _boolParam);

    // Paramètre prenant une valeur parmi une liste de choix
    // 1) création de la liste des choix possibles
    QStringList liste;
    liste.append("choix 1");
    liste.append("choix 2");
    liste.append("choix 3");
    // 2) ajout de la liste de choix à la boite de dialogue
    configDialog->addStringChoice("Paramètre choix dans une liste", "labelAfter", liste, _listParam);

    // Paramètre prenant une valeur de choix
    // Création du groupe de valeurs exclusives dans la boite de dialogue (non visible)
    CT_ButtonGroup &buttonGroup = configDialog->addButtonGroup(_choiceParam);

    // Création des items séléctionnables de façon exclusive (visibles)
    configDialog->addExcludeValue("Valeur exclusive 1", "labelAfter", "labelRadioButton", buttonGroup, 1);
    configDialog->addExcludeValue("Valeur exclusive 2", "labelAfter", "labelRadioButton", buttonGroup, 2);
    configDialog->addExcludeValue("Valeur exclusive 3", "labelAfter", "labelRadioButton", buttonGroup, 3);

    // Paramètre prenant une valeur "nom de fichier"
    configDialog->addFileChoice("Paramètre nom de Fichier", CT_FileChoiceButton::OneExistingFile, "Tout fichier (*.*)",_fileNameParam);
}

void TUFR_StepTutorial01::compute()
{
    // Aucune action, sauf affichage des valeurs de paramètres choisies à la console

    qDebug();
    qDebug() << "Paramètre entier = " << _intParam;
    qDebug() << "Paramètre double = " << _doubleParam;
    qDebug() << "Paramètre booléenne = " << _boolParam;
    qDebug() << "Paramètre choix dans une liste = " << _listParam;
    qDebug() << "Paramètre valeur exclusive = " << _choiceParam;
    qDebug() << "Paramètre Nom de fichier = " << _fileNameParam;

}
