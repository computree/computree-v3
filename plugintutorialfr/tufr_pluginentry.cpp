/*
    Implémentation de la classe TUFR_PluginEntry
*/

// Inclusion du fichier d'entête des du pluginManager (gestionnaire d'étapes)
#include "tufr_pluginentry.h"
#include "tufr_steppluginmanager.h"

// Constructeur
TUFR_PluginEntry::TUFR_PluginEntry()
{
    // création effective et initialisation du gestionnaire d'étapes
    _stepPluginManager = new TUFR_StepPluginManager();
    _stepPluginManager->init();
}

// Desctructeur : suppression du gestionnaire d'étapes
TUFR_PluginEntry::~TUFR_PluginEntry()
{
    delete _stepPluginManager;
}

// Version du plugin
// Changer la chaîne de caractère lorsqu'une nouvelle version commence
// Attentions les scripts d'étapes tiennent compte de la version du plugin
QString TUFR_PluginEntry::getVersion() const
{
    return "1.0";
}

// Renvoie le gestionnaire d'étape créé
StepPluginInterface* TUFR_PluginEntry::getStepPluginManager()
{
    return _stepPluginManager;
}

// Macro QT permettant la création de la librairie (.dll ou .so)
// Le premier paramètre doit être identique à celui passé à la directive TARGET du fichier .pro
// Le second est le nom de la présente classe
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(plug_tutorialfr, TUFR_PluginEntry)
#endif
