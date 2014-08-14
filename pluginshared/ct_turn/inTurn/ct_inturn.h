#ifndef CT_INTURN_H
#define CT_INTURN_H

#include "ct_result/model/inModel/tools/ct_inresultsbymodel.h"
#include "ct_result/model/inModel/abstract/ct_inabstractresultmodel.h"

class CT_VirtualAbstractStep;
class SettingsNodeGroup;

class PLUGINSHAREDSHARED_EXPORT CT_InTurn
{
public:
    CT_InTurn();
    ~CT_InTurn();

    /*!
     *  \brief Ajoute un résultat modèle d'entrée (les modèles seront supprimés de la mémoire)
     *
     *  \return false si le résultat modèle n'est pas valide ou si il existe (même nom) déjà dans la liste.
     */
    bool addResultModel(CT_InAbstractResultModel *model);

    /*!
     *  \brief Retourne la liste des modèles d'entrée.
     */
    const QList<CT_InAbstractResultModel*>& models() const;

    /*!
     *  \brief Retourne true si chaque modèle à au moins une possibilité de sélectionnée.
     */
    bool isValid() const;

    /*!
     *  \brief Retourne pour chaque modèle de ce tour la liste des résultats en fonction des possibilités cochées dans les modèles
     *
     *  \return une liste vide si il a eu un problème ou si le tour n'est pas valide.
     */
    QList<CT_InResultsByModel*> getResultsFromStep(CT_VirtualAbstractStep *step) const;

    /*!
     *  \brief Retourne tous les éléments nécessaire à la sauvegarde des modèles dans un fichier
     */
    QList<SettingsNodeGroup*> getAllValues() const;

    /*!
     *  \brief Initialise la fenêtre en fonction des éléments passé en paramètres
     */
    bool setAllValues(const QList<SettingsNodeGroup*> &list);

private:

    QList<CT_InAbstractResultModel*>    _models;

    /*!
     *  \brief Vérifie si le résultat modèle existe déjà dans la liste (compare le modelName du modèle avec ceux dans la liste).
     *
     *  \return false si le résultat modèle existe déjà dans la liste.
     */
    bool existInList(CT_InAbstractResultModel *model) const;
};

#endif // CT_INTURN_H
