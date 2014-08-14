#ifndef CT_INRESULTMODELCONFIGURATIONMANAGER_H
#define CT_INRESULTMODELCONFIGURATIONMANAGER_H

#include "pluginShared_global.h"

#include <QList>

class SettingsNodeGroup;

class CT_InManager;
class CTG_InResultModelConfiguration;

class PLUGINSHAREDSHARED_EXPORT CT_InResultModelConfigurationManager
{
public:

    enum CreateDialogReturn
    {
        CreateError,
        CreateSuccess,
        CreateNotNecessary
    };

    enum ConfigureReturn
    {
        ConfigureError,
        ConfigureSuccess,
        ConfigureNotChanged,
        ConfigureCanceled
    };

    CT_InResultModelConfigurationManager(CT_InManager &inManager);
    ~CT_InResultModelConfigurationManager();

    /*!
     *  \brief Crée la fenêtre de configuration des résultats modèles d'entrées SI nécessaire
     *
     *  INFO : Si chaque modèle n'a qu'une seule possibilité aucune fenêtre de configuration ne sera créé
     *
     *  \return une valeur indiquant si la création est un succès ou autre
     */
    CT_InResultModelConfigurationManager::CreateDialogReturn createInResultModelConfigurationDialog();

    /*!
     *  \brief Affiche la fenêtre de configuration des résultats modèles d'entrées SI nécessaire et configure les modèles d'entrée
     *
     *  INFO : Si chaque modèle n'a qu'une seule possibilité il seront tous cochés par défaut et aucune fenêtre ne sera affiché
     *
     *  \return une valeur indiquant si la configuration est un succès ou autre
     */
    CT_InResultModelConfigurationManager::ConfigureReturn configureInResultModel();

    /*!
     *  \brief Affiche la fenêtre de configuration des résultats modèles d'entrées SI nécessaire en mode lecture seul
     */
    void showReadOnlyInResultModel();

    /*!
     *  \brief Retourne tous les éléments nécessaire à la sauvegarde des modèles dans un fichier
     */
    QList<SettingsNodeGroup*> getAllValues() const;

    /*!
     *  \brief Initialise la fenêtre en fonction des éléments passé en paramètres
     */
    bool setAllValues(const QList<SettingsNodeGroup*> &list);

    /*!
     *  \brief Retourne true si il faut créer au afficher la fenêtre de configuration (test si les
     *         modèles ont plusieurs possibilités)
     */
    bool checkIfMustCreateOrShowConfigurationDialog() const;

    /**
     * @brief Called to inform that it will be deleted
     */
    void aboutToBeDeleted();

private:

    CT_InManager                    *_inManager;

    CTG_InResultModelConfiguration  *_configDialog;
};

#endif // CT_INRESULTMODELCONFIGURATIONMANAGER_H
