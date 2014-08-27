#ifndef CT_OUTABSTRACTRESULTMODELGROUP_H
#define CT_OUTABSTRACTRESULTMODELGROUP_H

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/def_ct_abstractgroupmodelout.h"

/**
 * @brief Represent a model that represent a result group
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutAbstractResultModelGroup : public CT_OutAbstractResultModel
{
public:
    /**
     * @brief Construct a OUTPUT model that represent a result
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     * @param resultName: the name that will be set to the result. If empty use uniqueName. Empty by default.
     */
    CT_OutAbstractResultModelGroup(const QString &uniqueName,
                                   const QString &description,
                                   const QString &displayableName,
                                   const QString &resultName = "");

    // INHERITED FROM CT_AbstractModel //
    QList<CT_AbstractModel*> childrens() const;
    bool isEmpty() const;
    //                  //

    /**
     * @brief Return the root model that represent a group
     */
    virtual DEF_CT_AbstractGroupModelOut* rootGroup() const = 0;

    /**
     * @brief Set the root model that represent a group (delete the last root group from memory if exist).
     */
    virtual void setRootGroup(DEF_CT_AbstractGroupModelOut *rootGroup) = 0;
};

#endif // CT_OUTABSTRACTRESULTMODELGROUP_H
