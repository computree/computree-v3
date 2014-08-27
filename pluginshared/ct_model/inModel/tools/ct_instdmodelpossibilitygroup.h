#ifndef CT_INSTDMODELPOSSIBILITYGROUP_H
#define CT_INSTDMODELPOSSIBILITYGROUP_H

#include "pluginShared_global.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

/**
 * @brief Call that must be used with CT_InStdModelPossibility. A group of possibilities can select/deselect possibility
 *        and have other option like "exclusive" if you want that only one possibility can be selected OR "enable" if you want
 *        to disable the group so no possibilities can be selected, etc...
 */
class PLUGINSHAREDSHARED_EXPORT CT_InStdModelPossibilityGroup
{
public:
    /**
     * @brief Constructor. By default the group is not exclusive, enable, minimum = 1 and maximum = 9999
     */
    CT_InStdModelPossibilityGroup();
    ~CT_InStdModelPossibilityGroup();

    /**
     * @brief Add a possibility to this group
     */
    void addPossibility(const CT_InStdModelPossibility *p);

    /**
     * @brief Remove a possibility from this group
     */
    bool removePossibility(const CT_InStdModelPossibility *p);

    /**
     * @brief Returns all possibilities of this group
     */
    const QList<CT_InStdModelPossibility*>& getPossibilities() const;

    /**
     * @brief Returns the number of possibilities
     */
    int nPossibilities() const;

    /**
     * @brief Remove all possibilities from this group
     */
    void clearPossibilities();

    /**
     * @brief Returns true if this possibility is selected
     */
    bool isSelected(const CT_InStdModelPossibility *p) const;

    /**
     * @brief Select this possibility
     */
    void select(CT_InStdModelPossibility *p);

    /**
     * @brief DeSelect this possibility
     */
    void unSelect(CT_InStdModelPossibility *p);

    /**
     * @brief Set this group to be exclusive. Only on possibility can be selected.
     * @warning If you set this parameter to true and multiple possibilty is selected, the group will remove all
     *          last possibilities selected while there was more than one possibility selected.
     */
    void setExclusive(bool ex);

    /**
     * @brief Returns true if this group select possibility in a exclusive mode
     */
    bool isExclusive() const;

    /**
     * @brief Enable/Disable the selection of possibility. If you set this parameter to false
     *        all possibilities selected will be cleared.
     */
    void setEnable(bool en);

    /**
     * @brief Returns true if this group is enable.
     */
    bool isEnable() const;

    /**
     * @brief Set the minimum number of possibility that must be selected in this group to be valid. No effect if this group is exclusive !
     * @return False if n is greather than "maximumNumberOfPossibilityThatCanBeSelected" or n == 0
     */
    bool setMinimumNumberOfPossibilityThatMustBeSelected(int n);
    int minimumNumberOfPossibilityThatMustBeSelected() const;

    /**
     * @brief Set the maximum number of possibility that can be selected in this group. No effect if this group is exclusive !
     * @return False if n is lower than "minimumNumberOfPossibilityThatMustBeSelected" or n == 0
     * @warning If n change and the number of possibilities is greather than n, all last selected possibilities will be removed while
     *          the number of selected possibilities is greather than n.
     */
    bool setMaximumNumberOfPossibilityThatCanBeSelected(int n);
    int maximumNumberOfPossibilityThatCanBeSelected() const;

    /**
     * @brief Returns true if this group is valid. If number of possibilities selected match the minimum and maximum criteria.
     */
    bool isValid() const;

private:

    QList<CT_InStdModelPossibility*>    m_possibilities;
    QList<CT_InStdModelPossibility*>    m_possibilitiesSelected;
    bool                                m_exclusive;
    bool                                m_enable;
    int                                 m_minMustBeSelected;
    int                                 m_maxCanBeSelected;
};

#endif // CT_INSTDMODELPOSSIBILITYGROUP_H
