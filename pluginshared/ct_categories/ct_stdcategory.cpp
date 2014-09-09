#include "ct_stdcategory.h"

CT_StdCategory::CT_StdCategory(const QString &uniqueName,
                               const QString &categoryEquivalentUniqueName,
                               const QString &displayableName,
                               const QString &description) : CT_AbstractCategory(uniqueName,
                                                                                 categoryEquivalentUniqueName,
                                                                                 displayableName,
                                                                                 description)
{

}

CT_StdCategory::CT_StdCategory(const QString &uniqueName,
                               const QList<QString> &categoryEquivalentUniqueNameCollection,
                               const QString &displayableName,
                               const QString &description) : CT_AbstractCategory(uniqueName,
                                                                                 categoryEquivalentUniqueNameCollection,
                                                                                 displayableName,
                                                                                 description)
{

}
