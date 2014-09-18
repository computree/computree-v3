#ifndef BATCHPLUGINMANAGER_H
#define BATCHPLUGINMANAGER_H

#include "interfacesbatch.h"

#include <QMap>
#include <QPluginLoader>

class BatchPluginManager
{
public:
    BatchPluginManager();
    ~BatchPluginManager();

    /*!
     *  \brief Charge/Recharge le plugin.
     *
     *  \return false si le chargement a échoué. Voir getError() pour plus d'informations.
     */
    bool load();

    /*!
     *  \brief Retourne un message expliquant la derniere erreur de chargement connu.
     */
    QString getError() const;

    /*!
     *  \brief Retourne le chemin de recherche des plugins.
     */
    QString getSearchPath() const;

    /*!
     *  \brief Defini le chemin de recherche des plugins.
     */
    void setSearchPath(QString path);

    /*!
     *  \brief Retourne l'entree du plugin chargé "i".
     */
    BatchPluginInterface* getPlugin(int i) const;

    /*!
     *  \brief Retourne le nom du plugin chargé "i".
     */
    QString getPluginName(int i) const;

    /*!
     *  \brief Retourne le nombre de plugins disponible.
     */
    int nPlugins() const;

    /*!
     *  \brief Retourne true si au moins un plugin a ete charge.
     */
    bool isAPluginLoaded() const;

private:

    QMap<QString, QPluginLoader*>       _plugins;

    QString                             _defaultPluginDirPath;
    QString                             _errorWhenLoad;

    void clearPlugins();
};

#endif // BATCHPLUGINMANAGER_H
