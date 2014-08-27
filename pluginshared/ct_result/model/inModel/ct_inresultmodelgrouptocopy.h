#ifndef CT_INRESULTMODELGROUPTOCOPY_H
#define CT_INRESULTMODELGROUPTOCOPY_H

#include "ct_result/model/inModel/ct_inresultmodelgroup.h"

class CT_OutResultModelGroupToCopyPossibility;
class CT_OutAbstractResultModelGroup;

/**
 * @brief A INPUT model that represent a CT_ResultGroup that you want to search and COPY ! Per example if you want to
 *        add other information like Item or Attributes, etc...
 */
class PLUGINSHAREDSHARED_EXPORT CT_InResultModelGroupToCopy : public CT_InResultModelGroup
{
public:

    /**
     * @brief Constructor
     * @param uniqueName : the unique name (in the hierarchy) of the model. Typically a DEF_...
     * @param rootGroupModel : the root model that represent a group.
     * @param displayableName : a displayable name for this model. If empty by default used the unique name. Empty by default.
     * @param description : a description of the result you search. If empty by default used the displayable name. Empty by default.
     * @param recursive : true if you want this model research recusively results in all step in the tree (go up in hierarchy) or false
     *                    if you want just research in results of the step passed in parameter of the method "searchResultsInStep...". False by default.
     */
    CT_InResultModelGroupToCopy(const QString &uniqueName,
                                CT_InAbstractGroupModel *rootGroupModel,
                                const QString &displayableName = "",
                                const QString &description = "",
                                bool recursive = false);

    /**
     * @brief Returns the model type in a displayable QString (for gui)
     */
    virtual QString modelTypeDisplayable() const;

    /*!
     *  \brief Retourne une liste des modèles de sortie que vous
     *         pouvez modifier si vous le souhaitez.
     *
     *  ATTENTION : Cette méthode crée de nouveaux objets. Vous êtes responsable
     *              de leur vie en mémoire ! Donc n'oubliez pas de les supprimer si vous
     *              ne vous en servez pas !!!!!
     */
    QList<CT_OutResultModelGroupToCopyPossibility*> getOutResultModelGroupsSelectedToCopy() const;

    /*!
     *  \brief Retourne une liste des résultats modèles de sortie que vous devez utiliser pour
     *         rechercher vos modèles d'entrée avec "getInModel(...)". Exemple :
     *
     *         CT_InResultModelGroupToCopy *intResModel = (CT_InResultModelGroupToCopy*)getInResultModel(DEF_SearchInResult);
     *
     *         const CT_OutAbstractResultModelGroup *outModelForSearchInModel = intResModel->getOutResultModelForSearchInModel().first();
     *
     *         CT_InAbstractSingularItemModel *inPointClusterModel = (CT_InAbstractSingularItemModel*)getInModel(*outModelForSearchInModel, DEF_SearchInPointCloud);
     */
    QList<const CT_OutAbstractResultModelGroup*> getOutResultModelForSearchInModel() const;

    /**
     * @brief Returns all values to saved a model to restore them later with the method "setAllValues"
     */
    virtual QList<SettingsNodeGroup*> getAllValues() const;

    /**
     * @brief Restore this model with values previously saved by the method "getAllValues"
     */
    virtual bool setAllValues(const QList<SettingsNodeGroup*> &list);

    // INHERITED FROM CT_InAbstractModel //
    CT_InAbstractModel* copy(bool withPossibilities) const;

protected:

    /**
     * @brief Overload the superclass method to copy the out model of the possibility so it can be modified
     */
    void possibilityCreated(CT_InStdModelPossibility *p);
};

#endif // CT_INRESULTMODELGROUPTOCOPY_H
