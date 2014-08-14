#ifndef CT_INSTDMODELPOSSIBILITY_H
#define CT_INSTDMODELPOSSIBILITY_H

#include "pluginShared_global.h"

#include <QObject>

class CT_OutAbstractModel;
class CT_InStdModelPossibilityGroup;

/**
 * @brief Represent a possibility for a INPUT model. When an INPUT model search an OUPUT model that match
 *        he can find a lot of OUTPUT model => a lot of POSSIBILITY.
 *
 *        You must use this class with the class CT_InStdModelPossibilityGroup
 */
class PLUGINSHAREDSHARED_EXPORT CT_InStdModelPossibility : public QObject
{
    Q_OBJECT

public:
    CT_InStdModelPossibility();
    virtual ~CT_InStdModelPossibility();

    /**
     * @brief Define the OUTPUT model for this possibility
     */
    void setOutModel(const CT_OutAbstractModel &model);

    /**
     * @brief Set to true if you want that the out model of this posibility must be deleted from memory.
     */
    void setAutoDeleteOutModel(bool e);

    /**
     * @brief Returns true if the out model of this posibility must be deleted from memory. False by default.
     */
    bool mustAutoDeleteOutModel() const;

    /**
     * @brief Returns the OUPUT model of this possibility
     */
    CT_OutAbstractModel* outModel() const;

    /**
     * @brief Returns if this possibility is selected
     */
    bool isSelected() const;

    /**
     * @brief Define the group of this possibility
     */
    void setGroup(const CT_InStdModelPossibilityGroup *group);

    /**
     * @brief Returns the group of this possibility
     */
    CT_InStdModelPossibilityGroup* group() const;

    /**
     * @brief Copy this possibility.
     */
    CT_InStdModelPossibility* copy() const;

public slots:

    /**
     * @brief Select/DeSelect this possibility. You can not select a possibility if it was not in a group.
     * @return false if you cannot modify this possibility
     */
    bool setSelected(bool check);

signals:

    /**
     * @brief Selected state changed
     */
    void selectStateChange(bool check);

private:

    CT_OutAbstractModel                 *m_model;
    CT_InStdModelPossibilityGroup       *m_group;
    bool                                m_autoDeleteOutModel;

protected:

    void clearOutModel();
};

#endif // CT_INSTDMODELPOSSIBILITY_H
