#ifndef DEF_CT_ABSTRACTITEMDRAWABLEMODELIN_H
#define DEF_CT_ABSTRACTITEMDRAWABLEMODELIN_H

#include "ct_itemdrawable/model/abstract/ct_abstractsingularitemmodelt.h"

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.h"
#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractsingularitemmodel.h"
#include "ct_attributes/model/inModel/abstract/ct_inabstractitemattributemodel.h"

#define DEF_CT_AbstractItemDrawableModelIn CT_AbstractSingularItemModelT<CT_InAbstractGroupModel, CT_InAbstractItemAttributeModel, CT_InAbstractSingularItemModel>

#endif // DEF_CT_ABSTRACTITEMDRAWABLEMODELIN_H
