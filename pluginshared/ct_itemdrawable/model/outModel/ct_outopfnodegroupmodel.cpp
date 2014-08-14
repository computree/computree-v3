#include "ct_outopfnodegroupmodel.h"

CT_OutOPFNodeGroupModel::CT_OutOPFNodeGroupModel(const QString &uniqueName,
                                                 CT_TOPFNodeGroup *group,
                                                 const QString &displayableName,
                                                 const QString &description) : CT_OutStdGroupModel(uniqueName,
                                                                                                   group,
                                                                                                   displayableName,
                                                                                                   description)
{
    m_level = 0;
}

void CT_OutOPFNodeGroupModel::setOPFLevel(const quint8 &level)
{
    m_level = level;
}

quint8 CT_OutOPFNodeGroupModel::opfLevel() const
{
    return m_level;
}

CT_OutStdGroupModel* CT_OutOPFNodeGroupModel::newInstance() const
{
    CT_OutOPFNodeGroupModel *opf = new CT_OutOPFNodeGroupModel(uniqueName(),
                                                               NULL,
                                                               displayableName(),
                                                               description());
    opf->setOPFLevel(opfLevel());

    return opf;
}
