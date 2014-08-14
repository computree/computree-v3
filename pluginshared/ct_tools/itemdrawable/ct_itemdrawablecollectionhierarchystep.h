#ifndef CT_ITEMDRAWABLECOLLECTIONHIERARCHYSTEP_H
#define CT_ITEMDRAWABLECOLLECTIONHIERARCHYSTEP_H

#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionhierarchyresult.h"

class PLUGINSHAREDSHARED_EXPORT CT_ItemDrawableCollectionHierarchyStep
{
public:
    CT_ItemDrawableCollectionHierarchyStep() { step = NULL; }

    CT_VirtualAbstractStep                              *step;
    QList< CT_ItemDrawableCollectionHierarchyResult >   results;
};

#endif // CT_ITEMDRAWABLECOLLECTIONHIERARCHYSTEP_H
