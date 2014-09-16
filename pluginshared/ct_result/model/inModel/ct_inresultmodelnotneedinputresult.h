#ifndef CT_INRESULTMODELNOTNEEDINPUTRESULT_H
#define CT_INRESULTMODELNOTNEEDINPUTRESULT_H

#include "pluginShared_global.h"

#include "ct_result/model/inModel/abstract/ct_inabstractresultmodel.h"
#include "ct_model/outModel/abstract/ct_outabstractmodel.h"

class PLUGINSHAREDSHARED_EXPORT CT_InResultModelNotNeedInputResult : public CT_InAbstractResultModel
{
    Q_OBJECT

public:
    CT_InResultModelNotNeedInputResult();

    /**
     * @brief Returns the model type in a displayable QString (for gui)
     */
    virtual QString modelTypeDisplayable() const;

    // INHERITED FROM CT_AbstractModel //
    virtual QList<CT_AbstractModel*> childrens() const { return QList<CT_AbstractModel*>(); }
    virtual bool isEmpty() const { return true; }
    //                  //

    /**
     * @brief This model can not search in step for results
     */
    virtual QList<CT_AbstractResult*> searchResultsInStepForPossibilitiesSelected(const CT_VirtualAbstractStep *step) const { Q_UNUSED(step) return QList<CT_AbstractResult*>(); }

    /**
     * @brief This model can not search in step for results
     */
    virtual CT_AbstractResult* searchResultsInStepForThisPossibility(const CT_VirtualAbstractStep *step, const CT_InStdModelPossibility *possibility) const { Q_UNUSED(step) Q_UNUSED(possibility) return NULL; }

    /**
     * @brief This model not need output model
     */
    virtual bool needOutputModel() const { return false; }

    /**
     * @brief Returns values to backup this model
     */
    virtual QList<SettingsNodeGroup*> getAllValues() const;

    /**
     * @brief Restore this model by values passed in parameter
     * @return false if it was an error in values
     */
    virtual bool setAllValues(const QList<SettingsNodeGroup*> &list);

    /**
     * @brief Copy of this model
     */
    virtual CT_InAbstractModel* copy(bool withPossibilities) const;

    /**
     * @brief Return true if this model can be compared with the OUTPUT model passed in parameter
     */
    virtual bool canBeComparedWith(const CT_OutAbstractModel &model) const { Q_UNUSED(model); return false; }

protected:

    /**
     * @brief Must return true if the model passed in parameter can be added in possibilities
     */
    virtual bool canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const { Q_UNUSED(model); return false; }
};

#endif // CT_INRESULTMODELNOTNEEDINPUTRESULT_H
