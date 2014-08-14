#include "ct_outabstractgroupmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"

CT_OutAbstractGroupModel::CT_OutAbstractGroupModel(const QString &uniqueName,
                                                   const QString &description,
                                                   const QString &displayableName,
                                                   CT_AbstractItemGroup *group) : DEF_CT_AbstractGroupModelOut(uniqueName,
                                                                                                               description,
                                                                                                               displayableName)
{
    _group = group;
}

CT_OutAbstractGroupModel::~CT_OutAbstractGroupModel()
{
    clearGroup();
}

CT_AbstractItemGroup* CT_OutAbstractGroupModel::group() const
{
    return _group;
}

CT_AbstractItemDrawable *CT_OutAbstractGroupModel::item() const
{
    return group();
}

void CT_OutAbstractGroupModel::clearGroup()
{
    delete _group;
    _group = NULL;
}

void CT_OutAbstractGroupModel::setGroup(CT_AbstractItemGroup *group)
{
    clearGroup();
    _group = group;
}
