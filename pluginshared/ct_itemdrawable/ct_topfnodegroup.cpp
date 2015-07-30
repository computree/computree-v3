#include "ct_topfnodegroup.h"

#include "ct_itemdrawable/model/outModel/ct_outopfnodegroupmodel.h"

CT_TOPFNodeGroup::CT_TOPFNodeGroup() : CT_TNodeGroup()
{
}

CT_TOPFNodeGroup::CT_TOPFNodeGroup(const CT_OutOPFNodeGroupModel *model,
                                   const CT_AbstractResult *result) : CT_TNodeGroup(model, result)
{
}

CT_TOPFNodeGroup::CT_TOPFNodeGroup(const QString &modelName,
                                   const CT_AbstractResult *result) : CT_TNodeGroup(modelName, result)
{
}

QString CT_TOPFNodeGroup::getType() const
{
    return staticGetType();
}

QString CT_TOPFNodeGroup::staticGetType()
{
    return CT_TNodeGroup::staticGetType() + "/CT_TOPFNodeGroup";
}

CT_AbstractItemDrawable* CT_TOPFNodeGroup::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    const CT_OutOPFNodeGroupModel *newModel = dynamic_cast< const CT_OutOPFNodeGroupModel* >(model);

    if(newModel == NULL)
        return NULL;

    CT_TOPFNodeGroup *itemGroup = new CT_TOPFNodeGroup(newModel, result);
    itemGroup->setId(id());
    itemGroup->setAlternativeDrawManager(getAlternativeDrawManager());

    // we can not copy successor etc... because we don't know the model

    // copy all ItemDrawable (reference) and groups (copy)
    if(!copyStructure(itemGroup))
    {
        delete itemGroup;
        return NULL;
    }

    return itemGroup;
}

void CT_TOPFNodeGroup::setOPFMatrix(const QMatrix4x4 &matrix)
{
    m_opfMatrix = matrix;
}

QMatrix4x4 CT_TOPFNodeGroup::opfMatrix() const
{
    return m_opfMatrix;
}

CT_OutOPFNodeGroupModel* CT_TOPFNodeGroup::opfModel() const
{
    return (CT_OutOPFNodeGroupModel*)model();
}

void CT_TOPFNodeGroup::setOPFID(const size_t &id)
{
    setId(id);
}
