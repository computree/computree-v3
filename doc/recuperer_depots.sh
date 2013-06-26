#!/bin/bash
sudo apt-get install subversion

mkdir exportershared
mkdir pluginsharedv2
mkdir computree
mkdir pluginonfensam
mkdir pluginonflsis
mkdir ressources

svn checkout https://computree.unfuddle.com/svn/computree_exportershared/ exportershared
svn checkout https://computree.unfuddle.com/svn/computree_pluginsharedv2/ pluginsharedv2
svn checkout https://computree.unfuddle.com/svn/computree_computreev2/ computree
svn checkout https://computree.unfuddle.com/svn/computree_pluginonfensamv2/ pluginonfensam
svn checkout https://computree.unfuddle.com/svn/computree_pluginonflsisv2/ pluginonflsis
svn checkout https://computree.unfuddle.com/svn/computree_ressources/ ressources

echo
echo
echo Si vous avez eu une erreur de proxy dans les commandes précédentes
echo Vous devez paramétrer le fichier suivant \(section [global]\) \:
echo .subversion\/servers dans dossier utilisateur
echo \(attention, le répertoire .subversion est caché\)
echo Ensuite, relancer ce script
echo
echo
read -p'Voulez-vous utiliser pcl (o/n) ?' -n 1 usepcl
echo
echo

if [ "${usepcl}" == "o" ]
then
	echo "" > use_pcl.ini
	echo Le fichier use_pcl.ini a été créé
	echo La librairie PCL sera accessible pour les plugins
else
	echo Attention, pcl non activé pour computree
	echo Certains plugins ne fonctionneront pas
fi

echo
echo
echo Si vous utilisez RapidSVN, pour chacun des répertoires suivants :
echo 								exportershared
echo 								pluginsharedv2
echo 								computree
echo 								pluginonfensam
echo 								pluginonflsis
echo 								ressources
echo
echo Faire Signet\/Ajouter une copie existante
echo Cela vous permettra de faire aisément les mises à jour
echo
echo
