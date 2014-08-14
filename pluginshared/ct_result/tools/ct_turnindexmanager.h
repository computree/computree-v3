#ifndef CT_TURNINDEXMANAGER_H
#define CT_TURNINDEXMANAGER_H

#include "pluginShared_global.h"

class CT_InTurnManager;

class PLUGINSHAREDSHARED_EXPORT CT_TurnIndexManager
{
public:
    CT_TurnIndexManager(CT_InTurnManager &inTurnManager);

    /*!
     *  \brief Retourne le nombre de tour maximum autorisé.
     */
    uint maximumTurn() const;

    /*!
     *  \brief Retourne le nombre de tour que l'on fera.
     */
    int nTurn() const;

    /*!
     *  \brief Retourne l'index du tour courant.
     */
    uint currentTurnIndex() const;

    /*!
     *  \brief Increment l'index du tour.
     *
     *  \return false si il n'y a plus de tour
     */
    bool incrementTurnIndex();

    /*!
     *  \brief Reinitialise l'index des tours.
     */
    void resetTurnIndex();

private:

    CT_InTurnManager    *_inTurnManager;

    uint                _currentTurnIndex;
};

#endif // CT_TURNINDEXMANAGER_H
