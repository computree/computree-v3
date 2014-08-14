#ifndef CT_INRESULTMANAGER_H
#define CT_INRESULTMANAGER_H

#include "ct_turn/inTurn/tools/ct_inturnmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_InResultManager
{
public:
    CT_InResultManager(CT_InResultModelManager &resultModelManager,
                       CT_InTurnManager &turnManager);

    /*!
     *  \brief Pour chaque tour (valide) définis dans le gestionnaire, récupère les résultats d'entrées et crée une liste par tour.
     *
     *  \return false si un tour n'a pas renvoyé de résultats d'entrée ou si aucun tour n'existe
     */
    bool initResultsFromStep(CT_VirtualAbstractStep *step);

    /*!
     *  \brief Retourne true si il existe des résultats d'entrée pour le tour courant.
     */
    bool existResultsForCurrentTurn() const;

    /*!
     *  \brief Retourne la liste des résultats d'entrée pour le tour courant. Une liste vide si le tour courant est erroné.
     */
    QList<CT_AbstractResult*> getResultsForCurrentTurn() const;

    /*!
     *  \brief Retourne la liste des résultats d'entrée de tous les tours.
     */
    QList<CT_InResultsByModel*> getResultsSortedByModelsForCurrentTurn() const;

    /*!
     *  \brief Supprime tous les résultats de la liste (ne supprime pas de la mémoire).
     */
    void clearResults();

    /*!
     *  \brief Appel la méthode setBusy pour chaque résultat du tour courant.
     */
    void setResultsOfCurrentTurnBusy(bool busy);

private:

    CT_InResultModelManager                 *_inResultModelManager;
    CT_InTurnManager                        *_turnManager;

    QList< QList<CT_InResultsByModel*> >    _results;
};

#endif // CT_INRESULTMANAGER_H
