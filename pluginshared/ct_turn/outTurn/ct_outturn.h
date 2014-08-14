#ifndef CT_OUTTURN_H
#define CT_OUTTURN_H

#include "ct_outabstractresultmodel.h"

#include <QList>

class CT_AbstractResult;

class PLUGINSHAREDSHARED_EXPORT CT_OutTurn
{
public:
    CT_OutTurn();
    ~CT_OutTurn();

    /*!
     *  \brief Ajoute un résultat modèle de sortie (les modèles seront supprimé de la mémoire)
     *
     *  \return false si le résultat modèle n'est pas valide ou si il existe (même nom) déjà dans la liste.
     */
    bool addResultModel(CT_OutAbstractResultModel *model);

    /*!
     *  \brief Retourne la liste des modèles de sortie.
     */
    const QList<CT_OutAbstractResultModel*>& models() const;

    /*!
     *  \brief Pour chaque modèle du tour on demande de créer le résultat pour en créer une liste et la renvoyer.
     *
     *  \return une liste vide si il y a eu une erreur
     */
    QList<CT_AbstractResult*> createResults() const;

private:

    QList<CT_OutAbstractResultModel*>    _models;

    /*!
     *  \brief Vérifie si le résultat modèle existe déjà dans la liste (compare le modelName du modèle avec ceux dans la liste).
     *
     *  \return false si le résultat modèle existe déjà dans la liste.
     */
    bool existInList(CT_OutAbstractResultModel *model) const;

    /*!
     *  \brief Supprime les résultats de la mémoire
     */
    void clearModels();
};

#endif // CT_OUTTURN_H
