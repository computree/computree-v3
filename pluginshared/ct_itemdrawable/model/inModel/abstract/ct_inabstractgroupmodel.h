#ifndef CT_INABSTRACTGROUPMODEL_H
#define CT_INABSTRACTGROUPMODEL_H

#include "def_ct_abstractgroupmodelin.h"
#include "ct_itemdrawable/model/outModel/abstract/def_ct_abstractgroupmodelout.h"

/**
 * @brief Represent a model for a INPUT group. With a INPUT group model you can :
 *
 *      - add/remove/clear INPUT groups and singular items
 *      - find/clear possibilities (a list of INPUT models that match with model researched) in OUTPUT item model
 *      - save/restore this model
 */
class PLUGINSHAREDSHARED_EXPORT CT_InAbstractGroupModel : public DEF_CT_AbstractGroupModelIn
{
public:

    /**
     * @brief A group must have a choice mode (C_DontChoose was removed)
     */
    enum GroupChoiceMode
    {
        CG_ChooseOneIfMultiple = CT_InAbstractModel::C_ChooseOneIfMultiple,
        CG_ChooseMultipleIfMultiple = CT_InAbstractModel::C_ChooseMultipleIfMultiple
    };

    /**
     * @brief A group cannot be optionnal
     */
    enum GroupFinderMode
    {
        FG_IsObligatory = CT_InAbstractModel::F_IsObligatory
    };

    /**
     * @brief Construct a group model
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_InAbstractGroupModel(QString uniqueName,
                            QString description,
                            QString displayableName);

    /**
     * @brief Return the type of the group researched (CT_GroupXXX::statiGetType)
     */
    QString groupType() const;

    /**
     * @brief Return the short type of the group (the string after the last '/' character in the type string)
     */
    QString groupShortType() const;

    /**
     * @brief Returns values to backup this model and its children
     */
    virtual QList<SettingsNodeGroup*> getAllValues() const;

    /**
     * @brief Restore this model and its children by values passed in parameter
     * @return false if it was an error in values
     */
    virtual bool setAllValues(const QList<SettingsNodeGroup*> &list);

protected:

    /**
     * @brief Set the group's type
     */
    void setGroupType(const QString &groupType);

private:

    QString                     m_groupType;
    QString                     m_groupShortType;
};

#endif // CT_INABSTRACTGROUPMODEL_H
