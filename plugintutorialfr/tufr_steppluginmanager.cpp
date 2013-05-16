/*
    Implémentation du gestionnaire d'étapes
*/

// Inclusion des classes génériques de PluginSharedV2 nécessaires
#include "tufr_steppluginmanager.h"
#include "ct_stepseparator.h"
#include "ct_steploadfileseparator.h"
#include "ct_stepcanbeaddedfirstseparator.h"
#include "step/tufr_steptutorial01.h"
#include "step/tufr_steptutorial02.h"
#include "step/tufr_steptutorial03.h"

// Inclure ici toutes les entêtes des classes définissant des étapes dans le plugin
//#include "step/TUFR_stepcalculateplotindicator.h"

// Constructeur (appelle le constructeur générique)
TUFR_StepPluginManager::TUFR_StepPluginManager() : CT_AbstractStepPlugin()
{
}

// Destructeur (ne fait rien de particulier)
TUFR_StepPluginManager::~TUFR_StepPluginManager()
{
}

// Cette méthode permet de construire le menu contextuel des étapes normales du plugin
bool TUFR_StepPluginManager::loadGenericsStep()
{
    // Ajoute une nouvelle section d'étapes (séparateur dans le menu contextuel)
    CT_StepSeparator *sep = addNewSeparator(new CT_StepSeparator());

    // Ajout d'un étape dans le menu (en fait l'étape est ajoutée au séparateur)
    sep->addStep(new TUFR_StepTutorial02(*createNewStepInitializeData(NULL)));
    sep->addStep(new TUFR_StepTutorial03(*createNewStepInitializeData(NULL)));
    //    sep->addStep(new TUFR_StepCalculatePlotIndicator(*createNewStepInitializeData(NULL)));

    // Si toutes les étapes ont pu être ajoutées, la méthode renvoie true (on peut continuer)
    return true;
}

// Cette méthode permet de construire la liste des étapes de chargement de fichier du plugin
// Si le plugin n'en contient pas, cette méthode se contente de renvoyer true
bool TUFR_StepPluginManager::loadOpenFileStep()
{
    // Si toutes les étapes ont pu être ajoutées, la méthode renvoie true (on peut continuer)
    return true;
}

// Cette méthode permet de construire la liste des étapes indépendantes
// Les étapes indépendantes, ne nécessitent pas d'étape mère pour être ajoutée
// Pour autant ce ne sont pas des étapes standard de chargement de fichier
bool TUFR_StepPluginManager::loadCanBeAddedFirstStep()
{
    // Ajoute une nouvelle section d'étapes pouvant être ajouter en tête de script
    CT_StepCanBeAddedFirstSeparator *sep = addNewSeparator(new CT_StepCanBeAddedFirstSeparator());

    // Ajout d'une étape
    sep->addStep(new TUFR_StepTutorial01(*createNewStepInitializeData(NULL)));

    // Si toutes les étapes ont pu être ajoutées, la méthode renvoie true (on peut continuer)
    return true;
}
