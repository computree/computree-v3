#ifndef CT_OUTABSTRACTITEMMODEL_H
#define CT_OUTABSTRACTITEMMODEL_H

#if defined(_MSC_VER)
#pragma warning( disable: 4250 )    // disable dominance warning
#endif

#include "ct_model/outModel/abstract/ct_outabstractmodel.h"

/**
 * @brief Represent a OUTPUT model for an item (singular or group)
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutAbstractItemModel : public CT_OutAbstractModel
{
    Q_OBJECT

public:

    /**
     * @brief Construct a model
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_OutAbstractItemModel(const QString &uniqueName,
                            const QString &description,
                            const QString &displayableName);

    /**
     * @brief Returns the real item of this model
     */
    virtual CT_AbstractItemDrawable* item() const = 0;
};

#endif // CT_OUTABSTRACTITEMMODEL_H
