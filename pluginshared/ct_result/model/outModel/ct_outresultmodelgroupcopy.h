#ifndef CT_OUTRESULTMODELGROUPCOPY_H
#define CT_OUTRESULTMODELGROUPCOPY_H

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodelgroup.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibility.h"

/**
 * @brief Use this class if you want to create a result that was a COPY of a possibility of
 *        an INPUT result. In other terms : if you want to create a copy of a result of a parent step use this class.
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutResultModelGroupCopy : public CT_OutAbstractResultModelGroup
{
    Q_OBJECT

public:
    /**
     * @brief Construct a OUTPUT model that represent a result
     * @param uniqueName : a unique name in all the hierarchy
     * @param copy : the possibility of an INPUT model (that represent a result) to copy. Will be destroyed from memory by this object.
     * @param resultName: the name that will be set to the copied result. If empty use uniqueName. Empty by default.
     * @param displayableName : a displayable name (used in gui for user information). Empty by default.
     * @param description : a description of the result taht will be generated. Empty by default.
     */
    CT_OutResultModelGroupCopy(QString uniqueName,
                               CT_OutResultModelGroupToCopyPossibility *copy,
                               QString resultName = "",
                               QString displayableName = "",
                               QString description = "");
    ~CT_OutResultModelGroupCopy();

    /**
     * @brief Returns the model type in a displayable QString (for gui)
     */
    virtual QString modelTypeDisplayable() const;

    // OVERLOADED FROM CT_OutAbstractResultModelGroup //
    virtual void setRootGroup(DEF_CT_AbstractGroupModelOut *rootGroup);
    virtual DEF_CT_AbstractGroupModelOut* rootGroup() const;

    // INHERITED FROM CT_OutAbstractResultModel //
    virtual CT_AbstractResult* createResult(CT_ResultInitializeData &data) const;

    // INHERITED FROM CT_OutAbstractModel //
    virtual CT_OutAbstractModel* originalModel() const;

    /**
     * @brief Returns a copy of this model
     */
    virtual CT_OutAbstractResultModel* copy() const;

    /**
     * @brief Returns the possibility of the INPUT model that will be used to create the copy of the result.
     */
    CT_OutResultModelGroupToCopyPossibility* outResultModelCopy() const;

    /**
     * @brief If you want to use the method getInModelForResearchIfUseCopy of a CT_VirtualAbstractStep you must use the unique name
     *        of this INPUT model
     */
    const CT_InResultModelGroupToCopy* inResultModelCopy() const;

protected:
    friend class CT_VirtualAbstractStep;

    /**
     * @brief Used by CT_VirtualAbstractStep to set the INPUT model that correspond to this model.
     */
    void setInResultCopyModel(const CT_InResultModelGroupToCopy* model);

private:

    CT_OutResultModelGroupToCopyPossibility *_outModelPossibility;
    CT_InResultModelGroupToCopy             *_inModelCopy;
};

#endif // CT_OUTRESULTMODELGROUPCOPY_H
