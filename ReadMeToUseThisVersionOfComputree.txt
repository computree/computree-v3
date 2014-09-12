Passer de l'ancienne version à la version 3 de Computree :

- Créer votre dossier de plugin à la racine du dossier de computreev3 (celui contenant ComputreeCore, ComputreGui, pluginshared, etc...)

- Faire un checkout sur ce dossier pour récupérer votre plugin

Fichier .pro :
--------------

- Ouvrir le fichier .pro de votre plugin
- Rajouter avant la commande "include(shared.pri)" la commande suivante :
	CT_PREFIX = ../..
- Le nombre de ".." doit correspondre au nombre de niveau à remonter à partir de votre .pro jusqu'au dossier racine contenant ComputreeCore, ComputreGui, pluginshared, etc...
- Modifier la commande "include(shared.pri)" par la commande suivante :
	include($${CT_PREFIX}/shared.pri)
	
Fichier XXX_PluginEntry.h et XXX_PluginEntry.cpp :
--------------------------------------------------

- Modifier la méthode "PluginInterface* getPlugin()" par :
	CT_AbstractStepPlugin* getPlugin() const
- ATTENTION veuillez ne pas oublier le mot clé "const" à la fin de la méthode

Fichier XXX_StepPluginManager.h et XXX_StepPluginManager.cpp:
-------------------------------------------------------------

- Ajouter le mot clé "const" à la fin de la méthode "getPluginURL()" :
	QString getPluginURL() const;
	
Fichier XXX_StepXXX :
---------------------

- Si vous utilisez des modèles il se peut que des include ne fonctionne plus. En effet les modèles "...Standard..." ont été renommés en "...Std..."

- Si vous avez une erreur du type : "invalid use of incomplete type 'class CT_StepConfigurableDialog'" vous devez include le fichier ".h" :
	#include "ct_view/ct_stepconfigurabledialog.h"
	
- Si vous avez une erreur du type : "'CT_GroupIterator' was not declared in this scope" vous devez include le fichier ".h" :
	#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
	
- Si vous avez une erreur du type : "'const class CT_AbstractPointCloudIndex' has no member named 'indexSize'" vous devez changer la méthode "indexSize" par "size"

- Si vous avez une erreur dans votre propre ItemDrawable du type : "'Result' has not been declared" vous devez changer "Result" par "const CT_AbstractResult"

- Si vous avez une erreur dans votre propre ItemDrawable du type : "'ItemDrawableGenericType' does not name a type" vous devez supprimer la méthode "rtti", celle ci ne sert plus.

- Si vous avez une erreur dans votre propre ItemDrawable du type : "'ItemDrawableConfiguration' does not name a type" vous devez remplacer "ItemDrawableConfiguration" par "CT_ItemDrawableConfiguration"