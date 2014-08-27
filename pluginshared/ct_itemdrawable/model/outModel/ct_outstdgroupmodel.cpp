#include "ct_outstdgroupmodel.h"

CT_OutStdGroupModel::CT_OutStdGroupModel(const QString &uniqueName,
                                         CT_AbstractItemGroup *group,
                                         const QString &displayableName,
                                         const QString &description) : CT_OutAbstractGroupModel(uniqueName,
                                                                                                description,
                                                                                                displayableName,
                                                                                                group)
{
}

QString CT_OutStdGroupModel::modelTypeDisplayable() const
{
    return QString("CT_OutStdGroupModel");
}

CT_OutAbstractModel* CT_OutStdGroupModel::copy() const
{
    return copyGroup();
}

DEF_CT_AbstractGroupModelOut* CT_OutStdGroupModel::copyGroup() const
{
    CT_OutStdGroupModel *cpy = newInstance();

    QListIterator< DEF_CT_AbstractGroupModelOut* > itG(groups());

    while(itG.hasNext())
        cpy->addGroupWithoutVerification((DEF_CT_AbstractGroupModelOut*)itG.next()->copyGroup());

    QListIterator<CT_OutAbstractSingularItemModel*> itI(items());

    while(itI.hasNext())
        cpy->addItemWithoutVerification((CT_OutAbstractSingularItemModel*)itI.next()->copy());

    if(group() != NULL)
        cpy->setItem(group()->copy(cpy, this->result(), CT_ResultCopyModeList()));

    cpy->setOriginalModel((const CT_OutAbstractItemModel*)this);

    return cpy;
}

CT_OutStdGroupModel* CT_OutStdGroupModel::newInstance() const
{
    return new CT_OutStdGroupModel(uniqueName(),
                                   NULL,
                                   displayableName(),
                                   description());
}
