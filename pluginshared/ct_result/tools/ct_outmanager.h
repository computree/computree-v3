#ifndef CT_OUTMANAGER_H
#define CT_OUTMANAGER_H

#include "ct_outresultmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_OutManager
{
public:
    CT_OutManager(CT_VirtualAbstractStep &step,
                  CT_InTurnManager &inTurnManager);
    ~CT_OutManager();

    void setTurnIndexManager(const CT_TurnIndexManager *turnIndexManager);

    CT_OutResultModelManager *getResultModelManager() const;
    CT_OutTurnManager* getTurnManager() const;
    CT_OutResultManager* getResultManager() const;

    /*!
     *  \brief Crée pour chaque tours défini en entrée des tours de sortie
     */
    bool createAllTurn();

protected:

    friend class CT_VirtualAbstractStep;

    /**
     * @brief Called from step to inform that result is cleared
     */
    void resultCleared();

private:

    CT_VirtualAbstractStep          *_step;

    CT_OutResultModelManager        _outResultModelManager;
    CT_OutTurnManager               *_outTurnManager;
    CT_OutResultManager             *_outResultManager;

    CT_InTurnManager                *_inTurnManager;
};

#endif // CT_OUTMANAGER_H
