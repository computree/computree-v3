#include "ct_stdcategory.h"

CT_StdCategory::CT_StdCategory(const QString &uniqueName,
                               CT_AbstractCategory::ValueType valueType,
                               const QString &categoryEquivalentUniqueName,
                               const QString &displayableName,
                               const QString &description) : CT_AbstractCategory(uniqueName,
                                                                                 valueType,
                                                                                 categoryEquivalentUniqueName,
                                                                                 displayableName,
                                                                                 description)
{

}

CT_StdCategory::CT_StdCategory(const QString &uniqueName,
                               CT_AbstractCategory::ValueType valueType,
                               const QList<QString> &categoryEquivalentUniqueNameCollection,
                               const QString &displayableName,
                               const QString &description) : CT_AbstractCategory(uniqueName,
                                                                                 valueType,
                                                                                 categoryEquivalentUniqueNameCollection,
                                                                                 displayableName,
                                                                                 description)
{

}
