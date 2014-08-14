#include "ct_outabstractresultmodelgroup.h"

CT_OutAbstractResultModelGroup::CT_OutAbstractResultModelGroup(const QString &uniqueName,
                                                               const QString &description,
                                                               const QString &displayableName,
                                                               const QString &resultName) : CT_OutAbstractResultModel(uniqueName,
                                                                                                                      description,
                                                                                                                      displayableName.isEmpty() ? resultName : displayableName)
{
    m_resultName = resultName;
}

QList<CT_AbstractModel *> CT_OutAbstractResultModelGroup::childrens() const
{
    QList<CT_AbstractModel *> l;

    if(rootGroup() != NULL)
        l.append(rootGroup());

    return l;
}

bool CT_OutAbstractResultModelGroup::isEmpty() const
{
    return (rootGroup() == NULL);
}

QString CT_OutAbstractResultModelGroup::resultName() const
{
    return m_resultName;
}
