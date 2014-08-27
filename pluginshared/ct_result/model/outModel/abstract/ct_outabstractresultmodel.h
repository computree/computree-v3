#ifndef CT_OUTABSTRACTRESULTMODEL_H
#define CT_OUTABSTRACTRESULTMODEL_H

#include "ct_model/outModel/abstract/ct_outabstractmodel.h"

#include <QList>

class CT_ResultInitializeData;
class CT_AbstractResult;

/**
 * @brief Represent a model that represent a result
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutAbstractResultModel : public CT_OutAbstractModel
{
public:

    /**
     * @brief Construct a OUTPUT model that represent a result
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_OutAbstractResultModel(const QString &uniqueName,
                              const QString &description,
                              const QString &displayableName,
                              const QString &resultName = "");

    /**
     * @brief Returns the name that will be set to the generated result
     */
    QString resultName() const;

    /**
     * @brief Create the result and return it.
     * @param data : initialized data for the result
     * @return NULL if it was a problem
     */
    virtual CT_AbstractResult* createResult(CT_ResultInitializeData &data) const = 0;

private:
    QString     m_resultName;
};

#endif // CT_OUTABSTRACTRESULTMODEL_H
