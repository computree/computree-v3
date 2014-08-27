#ifndef CT_INTURNMANAGER_H
#define CT_INTURNMANAGER_H

#include "ct_result/tools/ct_turnindexmanager.h"
#include "ct_result/model/inModel/tools/ct_inresultmodelmanager.h"
#include "ct_turn/inTurn/ct_inturn.h"

#include <QMap>

class CT_OutAbstractResultModel;

/**
 * @brief Manager of INPUT turn for a step
 */
class PLUGINSHAREDSHARED_EXPORT CT_InTurnManager
{
public:
    CT_InTurnManager(CT_VirtualAbstractStep &step,
                     CT_InResultModelManager &resultModelManager);
    ~CT_InTurnManager();

    /*!
     *  \brief Définir le gestionnaire d'incrémentation des tours.
     */
    void setTurnIndexManager(const CT_TurnIndexManager *turnIndexManager);

    /*!
     *  \brief Retourne le gestionnaire d'incrémentation des tours.
     */
    CT_TurnIndexManager* getTurnIndexManager() const;

    /*!
     *  \brief Retourne le gestionnaire des résultats modèles d'entrée.
     */
    CT_InResultModelManager* getResultModelManager() const;

    /*!
     *  \brief Définir le nombre de tour maximum que l'on peut créer. (par défaut 9999)
     *
     *  ATTENTION : si vous définissez un nombre de tour maximum inférieur au nombre de tour présent dans la liste, ils seront supprimés de la liste.
     */
    void setMaximumTurn(int n);

    /*!
     *  \brief Retourne le nombre de tour maximum autorisé.
     */
    int maximumTurn() const;

    /*!
     *  \brief Crée un nouveau tour avec les résultats modèles par défault et l'ajoute à la liste.
     *
     *  Le tour ne sera pas valide tant que chaque modèle n'a pas au moins une possibilité de coché.
     *
     *  \return NULL si le dernier tour créé n'est pas valide ou si vous avez dépassé le nombre de tour maximum autorisé.
     */
    CT_InTurn* createTurn();

    /*!
     *  \brief Retourne le nombre de tour.
     */
    int nTurn() const;

    /*!
     *  \brief Retourne le nombre de tour valide.
     */
    int nValidTurn() const;

    /*!
     *  \brief Supprime le tour i.
     */
    void removeTurn(int i);

    /*!
     *  \brief Retourne le tour i.
     */
    const CT_InTurn* getTurn(int i) const;

    /*!
     *  \brief Retourne la liste des tours.
     */
    const QList<CT_InTurn*>& turns() const;

    /*!
     *  \brief Supprime tous les tours.
     */
    void clearTurnList();

    /*!
     *  \brief Crée la liste des modèles d'entrée pour ce tour.
     *
     *  \return false si plusieurs modèles ont le même nom !
     */
    bool createSearchModelListForCurrentTurn();

    /*!
     *  \brief Vide la liste de recherche.
     */
    void clearSearchModelList();

    /*!
     *  \brief Retourne le résultat modèle d'entrée du tour courant à partir de son nom.
     *
     *  \return NULL si le modèle n'a pas été trouvé.
     */
    CT_InAbstractResultModel* getInResultModel(const QString &uniqueName) const;

    /*!
     *  \brief Retourne le modèle d'entrée du tour courant à partir de son nom. Utile pour la recherche dans les résultats de sortie.
     *
     *  \param outResultModel : le résultat modèle de sortie (pas celui
     *                          récupéré avec "getInResultModel()" mais avec "model()" du résultat de sortie)
     *
     *  \param modelName : le nom du modèle d'entrée que vous avez défini
     *
     *  \return NULL si le modèle n'a pas été trouvé.
     */
    CT_InAbstractModel* getInModel(const CT_OutAbstractResultModel &outResultModel, QString modelName) const;

    CT_InAbstractModel* getInModel(const QString &outResultUniqueName, const QString &uniqueName) const;

    /*!
     *  \brief Retourne l'étape à qui appartient ce gestionnaire
     */
    CT_VirtualAbstractStep* step() const;

private:
    CT_VirtualAbstractStep                                                  *_step;

    CT_InResultModelManager                                                 *_manager;
    CT_TurnIndexManager                                                     *_tIndexManager;

    QList<CT_InTurn*>                                                       _turns;
    int                                                                     _maximumTurn;

    QMap<CT_OutAbstractResultModel*, QMap<QString,CT_InAbstractModel*>* >   _modelsForCurrentTurn;

    bool isCurrentTurnIndexValid() const;
};

#endif // CT_INTURNMANAGER_H
