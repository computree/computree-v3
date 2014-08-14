#ifndef CT_OUTRESULTMANAGER_H
#define CT_OUTRESULTMANAGER_H

#include <QObject>

#include "ct_turn/outTurn/tools/ct_outturnmanager.h"

class Result;

class PLUGINSHAREDSHARED_EXPORT CT_OutResultManager : public QObject
{
    Q_OBJECT

public:
    CT_OutResultManager(CT_OutResultModelManager &resultModelManager,
                        CT_OutTurnManager &turnManager);
    ~CT_OutResultManager();

    /*!
     *  \brief Crée la liste des résultats nécessaire au tour courant.
     *
     *  \return false si les résultats ont déjà été créé pour ce tour ou si aucun tour n'existe
     */
    bool initResultsForCurrentTurn();

    /*!
     *  \brief Retourne la liste des résultats pour le tour courant. Une liste vide si le tour courant est erroné.
     */
    QList<CT_AbstractResult*> getResultsForCurrentTurn() const;

    /*!
     *  \brief Retourne la liste des résultats de sortie de tous les tours.
     */
    QList<QList<CT_AbstractResult *>* > getResultsForAllTurn() const;

    /*!
     *  \brief Retourne le nombres totales de résultats.
     */
    int nResults() const;

    /*!
     *  \brief Retourne le résultat n ou NULL si il n'existe pas.
     */
    CT_AbstractResult* getResult(int n) const;

    /*!
     *  \brief Supprime de la liste des résultats le résultat n et le retourne.
     *
     *  \return NULL si il n'existe pas
     */
    CT_AbstractResult* takeResult(int n);

    /*!
     *  \brief Supprime tous les résultats de la liste (supprime de la mémoire).
     */
    void clearResults();

    /*!
     *  \brief Finalise les résultats du tour courant (à n'appeler que si l'étape n'a pas eu d'erreur lors de son traitement).
     *
     *  Dis au résultat qu'il est complet et emet le signal pour dire qu'il y a un nouveau résultat de disponible.
     */
    void completesResultsOfCurrentTurn();

private:

    CT_OutResultModelManager            *_resultModelManager;
    CT_OutTurnManager                   *_turnManager;

    QList< QList<CT_AbstractResult*>* > _results;

signals:

    void resultAdded(CT_AbstractResult *res);
};

#endif // CT_OUTRESULTMANAGER_H
