/*
    Cette classe est le point d'entrée du plugin. C'est elle qui initie la création d'une
    librairie (.dll ou .so) spécifique au plugin.
    Sa struture ne change pas d'un plugin à l'autre en dehors de sa dénomination
*/

// Directives classique c++, permettant d'éviter d'inclure plusieurs fois un même fichier
#ifndef TUFR_PLUGINENTRY_H
#define TUFR_PLUGINENTRY_H

// Inclusion du fichier interface.h
// Ce fichier est le point le liaison avec PluginSharedV2
// En effet, ce fichier regroupe toutes les déclarations de classes génériques de PluginSharedV2
#include "interfaces.h"

// Inlcusion de la classe de gestion d'étapes,
// On n'utilise pas de directive #include, à cause d'une inclusion réciproque des deux classes
class TUFR_StepPluginManager;

// Cette classe hétite de la classe générique PluginInterface de PluginSharedV2
// C'est le cas pour tout plugin d'étapes
class TUFR_PluginEntry : public PluginInterface
{

// Macros Qt. La première donne accès à certaines fonctionnalités Qt utiles
// La second permet de créer un plugin sous forme de librairie (.dll, .so)
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:

// Constructeur et destructeur
    TUFR_PluginEntry();
    ~TUFR_PluginEntry();

// Permet de versionner les plugins
    QString getVersion() const;
// Lien avec le gestionnaire d'étapes
    StepPluginInterface* getStepPluginManager();

private:
// Gestionnaire d'étapes du plugin
    TUFR_StepPluginManager *_stepPluginManager;
};

#endif // TUFR_PLUGINENTRY_H
