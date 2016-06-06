#ifndef CT_INRESULTMODELMANAGER_H
#define CT_INRESULTMODELMANAGER_H

#include "ct_result/model/inModel/abstract/ct_inabstractresultmodel.h"

class CT_VirtualAbstractStep;

/**
 * @brief Manager of model of result. You can add models to this manager and he will check if
 *        the name is really unique. You can find all elements from a step that match (or not) with
 *        models in this manager.
 */
class PLUGINSHAREDSHARED_EXPORT CT_InResultModelManager
{
public:
    CT_InResultModelManager();
    ~CT_InResultModelManager();

    /**
     * @brief Add a model that represente a result.
     * @return false if the model is not valid or if already exist (same unique name) in the collection
     */
    bool addResultModel(CT_InAbstractResultModel *model);

    /**
     * @brief Remove all models from this manager (deleted from memory too)
     */
    void clearAllResultModels();

    /**
     * @brief Check if models in this manager find elements that match in step passed in
     *        parameter or parent step if models are recursive.
     * @param step : step where find elements (or starting step if a model is recursive)
     * @return true if all obligatory elements was found
     */
    bool isAllOutputModelFoundFromStep(const CT_VirtualAbstractStep *step);

    /**
     * @brief Search all elements in step passed in parameter that match with models of this manager and save them
     *        in possibilities of models.
     * @param step : step where find elements (or starting step if a model is recursive)
     * @return true if all obligatory elements was found
     */
    bool findAllOutputModelFromStepAndSavePossibilities(const CT_VirtualAbstractStep *step);

    /**
     * @brief Returns true if no models is in this manager
     */
    bool isEmpty() const;

    /**
     * @brief Returns true if at least one model need to search elements
     */
    bool needInputResults() const;

    /**
     * @brief Return the list of models of this manager
     */
    const QList<CT_InAbstractResultModel*>& models() const;

private:

    QList<CT_InAbstractResultModel*>    _models;

    /**
     * @brief Returns true if model passed in parameter exist in this manager (compare the unique name)
     */
    bool existInList(const CT_InAbstractResultModel *model) const;

    /*!
     *  \brief Recherche simplement sans sauvegardé ou en sauvegardant les modèles de sortie à partir de l'étape passé en paramètre.
     *
     *  \param step : l'étape à partir de laquelle chercher les modèles de sortie
     *  \param searchMultipleAndSave : à true si il faut rechercher toutes les possibilités et les sauvegarder
     *  \param returnTrueIfNotFoundAndMinimumPossibilitiesEqualsZero : à true si on veut que la méthode renvoie true même si les résultats
     *                                                                 qui sont optionnel n'on pas trouvé de possibilités
     *
     *  \return true si tout a été trouvé.
     */
    bool searchOneOrSearchMultipleAndSaveAllOutputModelFromStep(const CT_VirtualAbstractStep *step,
                                                                const bool &searchAndSave,
                                                                const bool &returnTrueIfNotFoundAndMinimumPossibilitiesEqualsZero);

    /*!
     *  \brief Recherche les modèles de sortie dans les étapes parentes récursivement pour le modèle d'entrée passé en paramètre.
     *
     *  \param step : l'étape dans laquelle chercher les modèles de sortie
     *  \param inModel : le résultat modèle d'entrée qui doit rechercher dans les modèles de sortie des étapes
     *  \param savePossibilities : à true si il faut sauvegarder les résultats
     *
     *  \return false si aucun modèle de sortie ne correspond au modèle d'entrée.
     */
    static bool staticRecursiveFindOutputModelFromStepForModel(const CT_VirtualAbstractStep *step,
                                                  CT_InAbstractResultModel *inModel,
                                                  const bool &savePossibilities,
                                                  const bool &multiple);

    /*!
     *  \brief Recherche les modèles de sortie dans l'étape pour le modèle d'entrée passé en paramètre.
     *
     *  \param step : l'étape dans laquelle chercher les modèles de sortie
     *  \param inModel : le résultat modèle d'entrée qui doit rechercher dans les modèles de sortie de l'étape
     *  \param savePossibilities : à true si il faut sauvegarder les résultats
     *
     *  \return false si aucun modèle de sortie ne correspond au modèle d'entrée.
     */
    static bool staticFindOutputModelFromStepForModel(const CT_VirtualAbstractStep *step,
                                         CT_InAbstractResultModel *inModel,
                                         const bool &savePossibilities,
                                         const bool &multiple);

    /*!
     *  \brief Recherche simplement sans sauvegardé ou en sauvegardant les modèles de sortie à partir de l'étape passé en paramètre pour le modèle passé en paramètre.
     *
     *  \param model : le modele pour lequel rechercher les possibilités
     *  \param step : l'étape à partir de laquelle chercher les modèles de sortie
     *  \param searchMultipleAndSave : à true si il faut rechercher toutes les possibilités et les sauvegarder
     *  \param returnTrueIfNotFoundAndMinimumPossibilitiesEqualsZero : à true si on veut que la méthode renvoie true même si les résultats
     *                                                                 qui sont optionnel n'on pas trouvé de possibilités
     *
     *  \return true si tout a été trouvé.
     */
    static bool staticSearchOneOrSearchMultipleAndSaveAllOutputModelFromStep(CT_InAbstractResultModel *model,
                                                                             const CT_VirtualAbstractStep *step,
                                                                             const bool &searchAndSave,
                                                                             const bool &returnTrueIfNotFoundAndMinimumPossibilitiesEqualsZero);
};

#endif // CT_INRESULTMODELMANAGER_H
