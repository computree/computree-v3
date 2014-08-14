#ifndef CT_OUTABSTRACTGROUPMODEL_H
#define CT_OUTABSTRACTGROUPMODEL_H

#include "def_ct_abstractgroupmodelout.h"

class CT_AbstractItemGroup;

#include <QList>

/**
 * @brief Represent a OUTPUT model for a group. With a OUTPUT group model you can :
 *
 *      - add/remove/clear OUTPUT groups and singular items
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutAbstractGroupModel : public DEF_CT_AbstractGroupModelOut
{
public:

    /**
     * @brief Construct a model
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     * @param group : the real group that you will create. MUST NOT BE NULL !
     */
    CT_OutAbstractGroupModel(const QString &uniqueName,
                             const QString &description,
                             const QString &displayableName,
                             CT_AbstractItemGroup *group);

    ~CT_OutAbstractGroupModel();

    /**
     * @brief Returns the real group of this model
     */
    CT_AbstractItemGroup* group() const;

    // INHERITED FROM CT_OutAbstractItemModel //
    CT_AbstractItemDrawable* item() const;

protected:

    /**
     * @brief Delete the group from this model
     */
    void clearGroup();

    /**
     * @brief Set the group of this model
     */
    void setGroup(CT_AbstractItemGroup *group);

private:
    CT_AbstractItemGroup                    *_group;
};

#endif // CT_OUTABSTRACTGROUPMODEL_H
