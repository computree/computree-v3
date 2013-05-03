/*
  Classe définissant le gestionnaire d'étapes
  C'est elle qui rend effectivement disponible les étapes implémentées
*/

#ifndef TUFR_STEPPLUGINMANAGER_H
#define TUFR_STEPPLUGINMANAGER_H

// Inclusion de l'interface générique de gestionnaire d'étapes, issue de PluginSharedV2
#include "ct_abstractstepplugin.h"

class TUFR_StepPluginManager : public CT_AbstractStepPlugin
{
public:
    // Constructeur et destructeur
    TUFR_StepPluginManager();
    ~TUFR_StepPluginManager();

protected:

    // Déclaration des étapes normales
    bool loadGenericsStep();
    // Déclaration des étapes de chargement de fichier (début de script)
    bool loadOpenFileStep();
    // Déclaration des étapes pouvant être ajoutée indépendament d'étapes précédentes
    bool loadCanBeAddedFirstStep();

};

#endif // TUFR_STEPPLUGINMANAGER_H
