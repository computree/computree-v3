#ifndef CT_OUTRESULTMODELMANAGER_H
#define CT_OUTRESULTMODELMANAGER_H

#include "ct_outabstractresultmodel.h"

#include <QList>

class PLUGINSHAREDSHARED_EXPORT CT_OutResultModelManager
{
public:
    CT_OutResultModelManager();
    ~CT_OutResultModelManager();

    /*!
     *  \brief Ajoute un résultat modèle d'entrée.
     *
     *  \return false si le résultat modèle n'est pas valide ou si il existe (même nom) déjà dans la liste.
     */
    bool addResultModel(CT_OutAbstractResultModel *model);

    /*!
     *  \brief Vide la liste des résultats modèles ajoutés et les supprime de la mémoire.
     */
    void clearAllResultModels();

    /*!
     *  \brief Retourne true si la liste des résultats modèles est vide.
     */
    bool isEmpty() const;

    /*!
     *  \brief Retourne la liste des résultats modèles.
     */
    const QList<CT_OutAbstractResultModel *> &models() const;

    /*!
     *  \brief Retourne la liste des résultats modèles en les enlevant de la liste (vous d vez vous occupez de les supprimer de la mémoire !).
     */
    QList<CT_OutAbstractResultModel *> takeModels();

private:

    QList<CT_OutAbstractResultModel*>   _models;

    /*!
     *  \brief Vérifie si le résultat modèle existe déjà dans la liste (compare le modelName du modèle avec ceux dans la liste).
     *
     *  \return false si le résultat modèle existe déjà dans la liste.
     */
    bool existInList(CT_OutAbstractResultModel *model) const;
};

#endif // CT_OUTRESULTMODELMANAGER_H
