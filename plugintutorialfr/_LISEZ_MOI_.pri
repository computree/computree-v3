#################################################
# TUTORIAL Computree - FR - Version du 03/05/2013
# A. PIBOULE, ONF - alexandre.piboule@onf.fr
#
# Ce fichier (_LISEZ_MOI_.pri) est le point de départ de ce tutoriel.
# Ses lignes sont commentées (#) afin de ne pas perturber le projet Qt.
# Lors de la lecture il faut donc ignorer les signes #
#
# Ce projet est un plugin fonctionnel pour computree
# Il contient des étapes visant à expliquer les mécanismes de création d'un plugin
# Chaque étape est commentée
#
#########################
# Structure d'un plugin :
#
# Chaque plugin est par convention identifié par à un code de 2 à 4 lettres majuscules
# qui préfixent les noms de toutes les classes du plugin.
# Le présent plugin a pour code TUFR (comme TUtorial version FRançaise)
#
# Le fichier plugintutorialfr.pro est le fichier "projet Qt".
# Ainsi par convention le fichier "projet Qt" d'un plugin est nommé pluginnomduplugin.pro
# C'est lui qui permet à Qt-Creator de connaître les fichiers du projet.
# Pour plus d'information sur les fichiers .pro, voir la documentation de Qt
#
# Un plugin Computree doit contenir en début de son fichier .pro les deux lignes suivantes :
# (sans le signe #)
# include(../../pluginsharedv2/shared.pri)
# include($${PLUGIN_SHARED_DIR}/include.pri)
#
# Cela permet d'inclure dans le projet les éléments de Computree nécessaires
# La conséquence, est la disponibilité des fichiers suivants dans le projet Qt :
# - destdir.pri
# - include.pri
# - shared.pri
#
# ATTENTION : ces trois fichiers ne doivent en aucun cas être modifiés !!!
#
# Ensuite le fichier .pro doit contenir la directive suivante :
# (sans le signe #)
# TARGET = plug_nomDuPlugin
# Cela définira le nom de la librairie du plugin une fois compilé
# Ici le nom du plugin est tutorialfr, ce qui donne : TARGET = plug_tutorialfr
#
# Pour fonctionner un plugin Computree doit contenir (au moins) deux classes :
# - CODE_PluginEntry (définie dans tufr_pluginentry.h et tufr_pluginentry.cpp ici)
# - CODE_StepPluginManager (définie dans tufr_steppluginmanager.h et tufr_steppluginmanager.cpp ici)
#
# La classe CODE_PluginEntry permet à Computree d'utiliser le plugin
# La classe CODE_StepPluginManager définit les étapes du plugin utilisables
#
# Ensuite on ajoute une classe pour chaque étape ou item du plugin,
# respectivement dans des repertoires step et itemdrawable
# Dans le cas général la création d'item n'est pas utile étant donné
# qu'il est préférable de n'utiliser que des items standard définis dans PluginSharedV2
# Ainsi, la plupart du temps un plugin ne contient que des étapes.
#
##########################################################
# Ordre préconisé pour la lecture des fichiers de ce tutorial :
# 1) En_têtes : tufr_pluginentry.h
# 2) Sources  : tufr_pluginentry.cpp
#
# 3) En_têtes : tufr_steppluginmanager.h
# 4) Sources  : tufr_steppluginmanager.cpp
#
# 5) En_têtes : step/tufr_steptutorial01.h
# 8) Sources  : step/tufr_steptutorial01.cpp
