#ifndef CT_OUTTURNMANAGER_H
#define CT_OUTTURNMANAGER_H

#include "ct_turnindexmanager.h"
#include "ct_outresultmodelmanager.h"
#include "ct_turn/outTurn/ct_outturn.h"

#include <QMap>

class PLUGINSHAREDSHARED_EXPORT CT_OutTurnManager
{
public:
    CT_OutTurnManager(CT_OutResultModelManager &resultModelManager);
    ~CT_OutTurnManager();

    /*!
     *  \brief Définir le gestionnaire des incrémentation des tours.
     */
    void setTurnIndexManager(const CT_TurnIndexManager *turnIndexManager);

    /*!
     *  \brief Retourne le gestionnaire d'incrémentation des tours.
     */
    CT_TurnIndexManager* getTurnIndexManager() const;

    /*!
     *  \brief Retourne le gestionnaire des résultats modèles de sortie
     */
    CT_OutResultModelManager* getResultModelManager() const;

    /*!
     *  \brief Crée un nouveau tour avec une copie des résultats modèles de sortie de base et l'ajoute à la liste.
     *
     *  \param addTurnNameToDisplayableName : à true si il faut ajouter le numéro du tour au nom des résultats modèles
     */
    CT_OutTurn* createTurn(bool addTurnNameToDisplayableName);

    /*!
     *  \brief Retourne le nombre de tour.
     */
    int nTurn() const;

    /*!
     *  \brief Retourne la liste des tours.
     */
    const QList<CT_OutTurn*>& turns() const;

    /*!
     *  \brief Supprime tous les tours.
     */
    void clearTurnList();

    /*!
     *  \brief Crée la liste des modèles de sortie pour ce tour.
     *
     *  \return false si plusieurs modèles ont le même nom !
     */
    bool createSearchModelListForCurrentTurn();

    /*!
     *  \brief Vide la liste de recherche.
     */
    void clearSearchModelList();

    /*!
     *  \brief Retourne le résultat modèle de sortie à partir de son nom.
     *
     *  \return NULL si le modèle n'a pas été trouvé.
     */
    CT_OutAbstractResultModel* getOutResultModel(QString modelName) const;

    /*!
     *  \brief Retourne le modèle de sortie à partir de son nom.
     *
     *  \return NULL si le modèle n'a pas été trouvé.
     */
    CT_OutAbstractModel* getOutModel(const CT_OutAbstractResultModel &resultModel, QString modelName) const;

    /**
     * @brief Backup turns (move all turns from the turn list to a backup list)
     */
    void backupTurns();

    /**
     * @brief Return true if turns has been backup
     */
    bool hasBackup() const;

    /**
     * @brief Clear backups
     */
    void clearBackup();

private:
    CT_OutResultModelManager            *_manager;
    CT_TurnIndexManager                 *_tIndexManager;

    QList<CT_OutTurn*>                  _turns;
    QList<CT_OutTurn*>                  m_backup;

    QList<CT_OutAbstractResultModel*>                                       _resultModelsForCurrentTurn;
    QMap<CT_OutAbstractResultModel*, QMap<QString,CT_OutAbstractModel*>* >  _modelsForCurrentTurn;

    bool existInSearchResultModelList(CT_OutAbstractResultModel *model) const;
};

#endif // CT_OUTTURNMANAGER_H
