#ifndef CT_ABSTRACTATTRIBUTES_H
#define CT_ABSTRACTATTRIBUTES_H

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

/**
 * @brief Represents a cloud attribute. This is a singular item a bit special since
 *        it can also be used to define colors for a cloud of points or normals in a view of a document.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractAttributes : public CT_AbstractSingularItemDrawable
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractAttributes, CT_AbstractSingularItemDrawable, Attributes)

public:
    CT_AbstractAttributes();
    CT_AbstractAttributes(const CT_OutAbstractSingularItemModel *model,
                          const CT_AbstractResult *result);

    CT_AbstractAttributes(const QString &modelName,
                          const CT_AbstractResult *result);

    /**
     * @brief Returns the size of the cloud of attributes
     */
    virtual size_t attributesSize() const = 0;

};

#endif // CT_ABSTRACTATTRIBUTES_H
