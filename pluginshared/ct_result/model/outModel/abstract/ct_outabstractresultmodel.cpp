#include "ct_outabstractresultmodel.h"

CT_OutAbstractResultModel::CT_OutAbstractResultModel(const QString &uniqueName,
                                                     const QString &description,
                                                     const QString &displayableName,
                                                     const QString &resultName) : CT_OutAbstractModel(uniqueName,
                                                                                                      description,
                                                                                                      displayableName)
{
    m_resultName = resultName;
}

QString CT_OutAbstractResultModel::resultName() const
{
    return m_resultName;
}
