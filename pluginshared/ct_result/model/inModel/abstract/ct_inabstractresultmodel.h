#ifndef CT_INABSTRACTRESULTMODEL_H
#define CT_INABSTRACTRESULTMODEL_H

#include "ct_model/inModel/abstract/ct_inabstractmodel.h"

class PLUGINSHAREDSHARED_EXPORT CT_InAbstractResultModel : public CT_InAbstractModel
{
public:
    CT_InAbstractResultModel(const QString &uniqueName,
                             const QString &description,
                             const QString &displayableName,
                             bool recursive);
    /**
     * @brief Set to true if you want to force recursivity on all new model created.
     */
    static bool FORCE_RECURSIVITY;

    /**
     * @brief Set to true if you want this model research recusively results in all step in the tree (go up in hierarchy) or false
     *        if you want just research in results of the step passed in parameter of the method "searchResultsInStep..."
     * @warning if you change this parameter you must recall method "searchResultsInStep..."
     */
    void setRecursive(bool r);

    /**
     * @brief Returns true if this model search results in all step in the tree (go up in tree) or false if search just in results of the step
     *        passed in parameter of the method "searchResultsInStep..."
     */
    bool isRecursive() const;

    /**
     * @brief Define the number minimum of possibility that a user must select for one turn. The number is not changed if method return false.
     * @return false if "n" is greater than "maximumNumberOfPossibilityThatCanBeSelectedForOneTurn()" or if at least one possibility already exist.
     */
    bool setMinimumNumberOfPossibilityThatMustBeSelectedForOneTurn(int n);

    /**
     * @brief Returns the number minimum of possibilities that a user must select for one turn
     */
    int minimumNumberOfPossibilityThatMustBeSelectedForOneTurn() const;

    /**
     * @brief Define the number maximum of possibility (result) that a user must select for one turn. The number is not changed if method return false.
     * @return false if "n" is 0 or if at least one possibility already exist.
     */
    bool setMaximumNumberOfPossibilityThatCanBeSelectedForOneTurn(int n);

    /**
     * @brief Returns the number maximum of possibilities that a user must select for one turn
     */
    int maximumNumberOfPossibilityThatCanBeSelectedForOneTurn() const;

    /**
     * @brief Verify if all models (only obligatory, recursively) have at least one possibility selected.
     * @return true is the test pass.
     */
    virtual bool recursiveIsAtLeastOnePossibilitySelectedIfItDoes() const;

    /**
     * @brief Search real results corresponding to selected possibilities from the step passed in parameter
     *        and recursively (go up in tree) if the parameter "isRecursive()" returns true.
     * @return An empty list if one of the model in the list of possibilities selected was not found or if there was no possibility selected.
     */
    virtual QList<CT_AbstractResult*> searchResultsInStepForPossibilitiesSelected(const CT_VirtualAbstractStep *step) const = 0;

    /**
     * @brief Search real results corresponding to the possibility passed in parameter from the step passed in parameter
     *        and recursively (go up in tree) if the parameter "isRecursive()" returns true.
     * @return NULL if the result was not found or if the possibility passed in parameter is not from this model
     */
    virtual CT_AbstractResult* searchResultsInStepForThisPossibility(const CT_VirtualAbstractStep *step,
                                                                     const CT_InStdModelPossibility *possibility) const = 0;

    /**
     * @brief A model that represent a result is compared to each OUPUT model (result) of a step (for multiple step). Childrens must not mix elements
     *        of one result with another. We make a copy of all childrens (INPUT model) for each OUTPUT model that represent a result so
     *        elements are not mixed. In this case each possibility has a INPUT model that was a copy of the "rootModel()" of this model.
     * @param pIndex : index of the possibility of this model
     * @param uniqueName : unique name of the model to search in tree of the INPUT model (copy) contained in the possibility at index "pIndex"
     * @return NULL if model was not found.
     */
    CT_InAbstractModel* findModelInTreeOfModelInPossibility(int pIndex, const QString &uniqueName) const;

protected:

    /**
     * @brief Returns all childrens (INPUT) that must be used to find possibilities in children of OUTPUT model. By
     *        default returns "childrens" method
     * @overload Overloaded to return a copy of all childrens if "savePossibilities" is true. A model that represent a result
     *           is compared for each OUPUT model (result) of a step of multiple step. Childrens must not mix elements of one
     *           result with another. We make a copy of all childrens for each OUTPUT model that represent a result so elements
     *           are not mixed.
     */
    virtual QList<CT_AbstractModel*> childrensToFindPossibilities(bool savePossibilities) const;

    /**
     * @brief Returns all childrens (INPUT) used by possibilities (Util for CT_InAbstractResultModel). By
     *        default returns "childrens" method.
     */
    virtual QList<CT_AbstractModel*> childrensOfPossibilities() const;

    /**
     * @brief Called when the INPUT model (a children) is finished to be compared with all OUTPUT model.
     * @param ok : the result of the comparaison, true if inModel has found all obligatory elements in at least one OUTPUT model
     * @overload Overload this method to delete the model that was compared (only if "savePossibilities" is true and "ok" is false) because
     *           we returns a copy of a model so we must delete it if ok is false.
     */
    virtual void inModelComparisonResult(CT_AbstractModel *inModel, bool ok, bool savePossibilities);

    /**
     * @brief Create a new possibility
     * @overload Overloaded to create a CT_InStdResulModelPossibility
     */
    virtual CT_InStdModelPossibility* createNewPossibility() const;

    /**
     * @brief Called when a possibility is added to this model
     * @overload Overloaded to add the last INPUT model that was compared to the possibility
     */
    virtual void possibilityCreated(CT_InStdModelPossibility *p);

    /**
     * @brief Called when a OUTPUT model already exist in a possibility so a new possibility was not created
     * @overload Overloaded to delete the last INPUT model that was compared
     */
    virtual void possibilityNotCreated();

private:
    bool                        m_recursive;
    CT_InAbstractModel          *m_backupModel;
};

#endif // CT_INABSTRACTRESULTMODEL_H
