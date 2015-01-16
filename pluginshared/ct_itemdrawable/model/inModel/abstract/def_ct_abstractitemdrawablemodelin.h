#ifndef DEF_CT_ABSTRACTITEMDRAWABLEMODELIN_H
#define DEF_CT_ABSTRACTITEMDRAWABLEMODELIN_H

#include "ct_itemdrawable/model/abstract/ct_abstractsingularitemmodelt.h"

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.h"
#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractsingularitemmodel.h"
#include "ct_attributes/model/inModel/abstract/ct_inabstractitemattributemodel.h"

typedef CT_AbstractSingularItemModelT<CT_InAbstractGroupModel, CT_InAbstractItemAttributeModel, CT_InAbstractSingularItemModel> DEF_CT_AbstractItemDrawableModelIn;

#endif // DEF_CT_ABSTRACTITEMDRAWABLEMODELIN_H
