#ifndef CT_OUTOPFNODEGROUPMODEL_H
#define CT_OUTOPFNODEGROUPMODEL_H

#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"

#include "ct_itemdrawable/ct_topfnodegroup.h"

/**
 * @brief Use this class if you will inform that you want create a OPF Node group and add it to an
 *        other OUTPUT model that represent a group or a result
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutOPFNodeGroupModel : public CT_OutStdGroupModel
{
public:    
    /**
     * @brief Construct a model
     * @param uniqueName : a unique name in all the hierarchy
     * @param group : the real group that you will create in the method "compute" of your step. MUST NOT BE NULL !
     * @param displayableName : a displayable name
     * @param description : a description
     */
    CT_OutOPFNodeGroupModel(const QString &uniqueName,
                            CT_TOPFNodeGroup *group = new CT_TOPFNodeGroup(),
                            const QString &displayableName = "",
                            const QString &description = "");
    /**
     * @brief Set level (Scale) readed in OPF File
     */
    void setOPFLevel(const quint8 &level);

    /**
     * @brief Returns level (Scale)
     */
    quint8 opfLevel() const;

protected:
    /**
     * @brief Returns an empty instance with just the level set
     * @overload Overloaded so the copy method of the CT_OutStdGroupModel will add other elements to this copy
     */
    virtual CT_OutStdGroupModel* newInstance() const;

private:
    quint8  m_level;
};

#endif // CT_OUTOPFNODEGROUPMODEL_H
