#ifndef CT_INABSTRACTMODEL_H
#define CT_INABSTRACTMODEL_H

#include "ct_model/abstract/ct_abstractmodel.h"

class CT_OutAbstractModel;
class CT_InStdModelPossibility;
class CT_InStdModelPossibilityGroup;

/**
 * @brief Represent a INPUT model. An INPUT model :
 *
 *          - search correspondent OUTPUT model in a tree collection.
 *          - can be Olbigatory or Optionnal
 *          - can have zero, 1 or multiple possibilities
 */
class PLUGINSHAREDSHARED_EXPORT CT_InAbstractModel : public CT_AbstractModel
{
public:
    enum FinderMode
    {
        F_IsObligatory = 0,         // the model must obligatory founded
        F_IsOptional                // the model is optional
    };

    enum ChoiceMode
    {
        C_DontChoose = 0,           // nothing
        C_ChooseOneIfMultiple,      // select one among multiple possibilities
        C_ChooseMultipleIfMultiple  // select multiple among multiple possibilities
    };

    /**
     * @brief Construct a model
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_InAbstractModel(const QString &uniqueName,
                       const QString &description,
                       const QString &displayableName);

    ~CT_InAbstractModel();

    /**
     * @brief Returns the choice mode
     */
    CT_InAbstractModel::ChoiceMode choiceMode() const;

    /**
     * @brief Returns the finder mode
     */
    CT_InAbstractModel::FinderMode finderMode() const;

    /**
     * @brief Start a new save cycle that can be undoable
     */
    void startSaveCycle();

    /**
     * @brief End the current save cycle
     */
    void finishSaveCycle();

    /**
     * @brief Returns the number of save cycle created
     */
    int nSaveCycle() const;

    /**
     * @brief Returns true if this model need OUTPUT model. Otherwise you have not to call method "recursiveFind..." because it will
     *        always return true and zero possibility will be saved. By default return true.
     */
    virtual bool needOutputModel() const;

    /**
     * @brief Find one possibility in the out model for this model and its children
     * @param model : the ouput model where begin the search
     * @param savePossibilities : true if you want this model and its children save possibility in the current save cycle
     * @return true if all model had at least one possibility if it must have one. Otherwise return false. If the out model
     *         cannot be compared with this model the method return automatically false.
     */
    bool recursiveFindOnePossibilityInModel(const CT_OutAbstractModel &model,
                                            bool savePossibilities);

    /**
     * @brief Find all possibilities in the out model for this model and its children
     * @param model : the ouput model where begin the search
     * @param savePossibilities : true if you want this model ans its children save all possibilities in the current save cycle
     * @return true if all model had at least one possibility if it must have one. Otherwise return false. If the out model
     *         cannot be compared with this model the method return automatically false.
     */
    bool recursiveFindAllPossibilitiesInModel(const CT_OutAbstractModel &model,
                                              bool savePossibilities);

    /**
     * @brief Verify if this model (optionnal and obligatory) have at least one possibility selected.
     * @return true is the test pass.
     */
    bool isAtLeastOnePossibilitySelected() const;

    /**
     * @brief Verify if all models (optionnal and obligatory, recursively) have at least one possibility selected.
     * @return true is the test pass.
     */
    bool recursiveIsAtLeastOnePossibilitySelected() const;

    /**
     * @brief Verify if this model (only obligatory) have at least one possibility selected.
     * @return true is the test pass.
     */
    virtual bool isAtLeastOnePossibilitySelectedIfItDoes() const;

    /**
     * @brief Verify if all models (only obligatory, recursively) have at least one possibility selected.
     * @return true is the test pass.
     */
    virtual bool recursiveIsAtLeastOnePossibilitySelectedIfItDoes() const;

    /**
     * @brief Returns true if we can select possibilities of this model by default. If the user must choose among possibilities the
     *        method return false.
     */
    virtual bool canSelectPossibilitiesByDefault() const;

    /**
     * @brief If you want to select possibilities and know if you select them if the model can pass the "default select test" you
     *        can call this method and pass the list of index of possibilities you want to select. By default returns false
     *          - if the number of possibilities passed in parameter is greather than the number of possibilities of this model
     *          - if the number of possibilities passed in parameter is greather than 1 and the choiceMode() is C_ChooseOneIfMultiple
     *          - if an index passed in parameter is not valid
     *
     * @param possibilitiesIndex : the list of index of possibilities that you want to select
     * @param selectChildrensTooRecursively : true if you want to check if childrens (recursively) can be selected by default
     * @return true if the test pass
     */
    virtual bool canSelectPossibilitiesByDefault(const QList<int> &possibilitiesIndex, bool selectChildrensTooRecursively) const;

    /**
     * @brief Returns true if we can select possibilities for all models (recursively) by default. If the user must choose among possibilities the
     *        method return false.
     */
    bool recursiveCanSelectPossibilitiesByDefault() const;

    /**
     * @brief Select all possibilities of this models if its possible by default. By default select the first possibility if exist.
     * @return false if select possibilities by default is impossible (used the method "canSelectPossibilitiesByDefault")
     */
    virtual bool selectAllPossibilitiesByDefault();

    /**
     * @brief Select all possibilities of all models (recursively) if its possible by default.
     * @return false if select possibilities by default is impossible (used the method "recursiveCanSelectPossibilitiesByDefault")
     */
    bool recursiveSelectAllPossibilitiesByDefault();

    /**
     * @brief Select possibilities of this model by default
     * @param possibilitiesIndex : the list of index of possibilities that you want to select
     * @param selectChildrensTooRecursively : true if you want to select childrens (recursively) too
     * @return true if the test "canSelectPossibilitiesByDefault" pass
     */
    virtual bool selectPossibilitiesByDefault(const QList<int> &possibilitiesIndex, bool selectChildrensTooRecursively);

    /**
     * @brief Returns the number of possibilities saved by this model
     */
    int nPossibilitiesSaved() const;

    /**
     * @brief Returns all possibilities saved by this model
     */
    QList<CT_InStdModelPossibility*> getPossibilitiesSaved() const;

    /**
     * @brief Returns all possibilities saved by this model and selected by the user or automatically
     */
    QList<CT_InStdModelPossibility*> getPossibilitiesSavedSelected() const;

    /**
     * @brief Clear all possibitilies saved by this model
     */
    void clearPossibilitiesSaved();

    /**
     * @brief Clear all possibitilies saved by this model and its children
     */
    void recursiveClearPossibilitiesSaved();

    /**
     * @brief Returns values to backup this model and its children
     */
    virtual QList<SettingsNodeGroup*> getAllValues() const;

    /**
     * @brief Restore this model and its children by values passed in parameter
     * @return false if it was an error in values
     */
    virtual bool setAllValues(const QList<SettingsNodeGroup*> &list);

    /**
     * @brief Returns a copy of this model and its children.
     * @param withPossibilities : true if you want to copy possibilities in the copy of the model
     */
    virtual CT_InAbstractModel* copy(bool withPossibilities) const = 0;

    /**
     * @brief Return true if this model can be compared with the OUTPUT model passed in parameter
     */
    virtual bool canBeComparedWith(const CT_OutAbstractModel &model) const = 0;

    /**
     * @brief Get errors that was happened since the last "clearError" was called. Errors is added when you call
     *        method "isAtLeastOnePossibilitySelectedIfItDoes"
     */
    QString errors() const;

    /**
     * @brief Clear all errors
     */
    void clearError();

protected:

    /**
     * @brief Class tools for save cycle
     */
    class CT_InModelSaveCycle : public QList<CT_InStdModelPossibility*>
    {
    public:
        CT_InModelSaveCycle() : QList<CT_InStdModelPossibility*>() {}
    };

    /**
     * @brief Must return true if the model passed in parameter can be added in possibilities
     */
    virtual bool canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const = 0;

    /**
     * @brief Must return true if the current OUTPUT model can be compared with children of the OUTPUT model.
     *
     *        This method is called after the method "canBeComparedWith(....)" if it returns true
     */
    virtual bool mustCompareThisWithChildrenOfOutModel() const { return false; }

    /**
     * @brief Must return true if the current OUTPUT model must be compared with children of this INPUT model.
     *
     *        This method is called after methods "canBeComparedWith(....)" "canAddPossibilityForThisModel(...)" if
     *        it returns true.
     */
    virtual bool mustCompareOutModelWithChildrenOfThisModel() const { return false; }

    /**
     * @brief Must return true if we can continue the research with children of the
     *        current OUTPUT model.
     *
     *        This method is called after the method "canBeComparedWith(....)" if it returns true
     */
    virtual bool mustCompareChildrenWithChildrenOfOutModel() const { return true; }

    /**
     * @brief Returns all childrens (INPUT) that must be used to find possibilities in children of OUTPUT model. By
     *        default returns "childrens" method
     */
    virtual QList<CT_AbstractModel*> childrensToFindPossibilities(bool savePossibilities) const { Q_UNUSED(savePossibilities) return childrens(); }

    /**
     * @brief Returns all childrens (INPUT) used by possibilities (Util for CT_InAbstractResultModel). By
     *        default returns "childrens" method.
     */
    virtual QList<CT_AbstractModel*> childrensOfPossibilities() const { return childrens(); }

    /**
     * @brief Remove from the list (m_saveCycles) the last save cycle
     *        and all possibilities of this cycle from the group (m_possibilitiesGroup)
     * @return false if the list is empty
     */
    bool deleteLastSaveCycle();

    /**
     * @brief Add a possibility to the list of possibilities and add it to the current save cycle
     * @return the possibility created if you want to modify something
     */
    void addToPossibility(const CT_OutAbstractModel &model);

    /**
     * @brief Create a new possibility
     */
    virtual CT_InStdModelPossibility* createNewPossibility() const;

    /**
     * @brief Return true if the model already exist in the list of possibilities (compare uniqueName())
     */
    bool existModelInPossibilities(const CT_OutAbstractModel &model) const;

    /**
     * @brief Set the choice mode (internal) and modify the possibities group in function of this parameter :
     *        C_DontChoose -> setEnable(false)
     *        C_ChooseOneIfMultiple -> setExclusive(true) / setEnable(true)
     *        C_ChooseMultipleIfMultiple -> setExclusive(false) / setEnable(true)
     *
     * @warning the modification of this mode can have repercussions on possibilities selected or saved !!!
     */
    void internalSetChoiceMode(CT_InAbstractModel::ChoiceMode mode);

    /**
     * @brief Set the finder mode (internal).
     * @warning If you change this mode from optionnal to obligatory you must recheck if possibilities checked by
     *          default is always ok.
     */
    void internalSetFinderMode(CT_InAbstractModel::FinderMode mode);

    /**
     * @brief Copy all possibilities from one model to another
     */
    static void staticCopyPossibilitiesToModel(const CT_InAbstractModel *src, CT_InAbstractModel *dst);

    /**
     * @brief Returns the group that contains possibilities
     * @warning If you remove or add possibility to this group you must add them with the method "addToPossibility(...)"
     */
    CT_InStdModelPossibilityGroup* possibilitiesGroup() const;

    /**
     * @brief Called when a possibility is created and added to this model
     */
    virtual void possibilityCreated(CT_InStdModelPossibility *p) { Q_UNUSED(p) }

    /**
     * @brief Called when a OUTPUT model already exist in a possibility so a new possibility was not created
     */
    virtual void possibilityNotCreated() {}

    /**
     * @brief Called before a INPUT model (a children) will be compared with OUTPUT models
     */
    virtual void inModelToBeCompared(CT_AbstractModel *inModel, bool savePossibilities) { Q_UNUSED(inModel) Q_UNUSED(savePossibilities) }

    /**
     * @brief Called before the INPUT model (a children) will be compared with the OUTPUT model
     */
    virtual void inModelToBeCompared(CT_AbstractModel *inModel, const CT_AbstractModel *outModel, bool savePossibilities) { Q_UNUSED(inModel) Q_UNUSED(outModel) Q_UNUSED(savePossibilities) }

    /**
     * @brief Called after the INPUT model (a children) was compared with the OUTPUT model
     */
    virtual void inModelCompared(CT_AbstractModel *inModel, const CT_AbstractModel *outModel, bool savePossibilities) { Q_UNUSED(inModel)  Q_UNUSED(outModel) Q_UNUSED(savePossibilities) }

    /**
     * @brief Called when the INPUT model (a children) is finished to be compared with all OUTPUT model.
     * @param ok : the result of the comparaison, true if inModel has found all obligatory elements in at least one OUTPUT model
     */
    virtual void inModelComparisonResult(CT_AbstractModel *inModel, bool ok, bool savePossibilities) { Q_UNUSED(inModel) Q_UNUSED(ok) Q_UNUSED(savePossibilities) }

    /**
     * @brief Add a new line to error
     */
    void addToError(const QString &err) const;

    /**
     * @brief Replace all erros by this error
     */
    void setError(const QString &err) const;

private:
    CT_InAbstractModel::ChoiceMode          m_choiceMode;
    CT_InAbstractModel::FinderMode          m_finderMode;

    QList< CT_InModelSaveCycle >            m_saveCycles;
    CT_InStdModelPossibilityGroup           *m_possibilitiesGroup;

    int                                     m_nPossibilitiesWithoutSaving;

    mutable QString                         m_errors;

    /**
     * @brief Find one/all possibilities in the out model for this model and its children
     * @param model : the ouput model where begin the search
     * @param savePossibilities : true if you want this model ans its children save all possibilities in the current save cycle
     * @param searchMultiple : true if you want to search multiple possibility, false if you want to search only one
     * @return 1 if all model had at least one possibility if it must have one. 2 if it's ok recursively. Otherwise return 0. If the out model
     *         cannot be compared with this model the method return automatically 0.
     */
    int recursiveFindPossibilitiesInModel(const CT_OutAbstractModel &model, bool savePossibilities, bool searchMultiple);
};

#endif // CT_INABSTRACTMODEL_H
