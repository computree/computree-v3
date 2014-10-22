#ifndef DM_ABSTRACTATTRIBUTESSCALAR_H
#define DM_ABSTRACTATTRIBUTESSCALAR_H

#include "tools/attributes/worker/abstract/dm_abstractattributes.h"
#include "ct_global/ct_repository.h"

#include <QLinearGradient>

class DM_AbstractAttributesScalar : public DM_AbstractAttributes
{
public:
    DM_AbstractAttributesScalar();

    // GETTER
    /**
     * @brief set auto adjust gradient from min() and max() values of points attributes
     */
    virtual void setAutoAdjust(bool automatic) = 0;

    /**
     * @brief Set manually min value. Auto adjust is disabled after a call of this method.
     */
    virtual void setMin(double min) = 0;

    /**
     * @brief Set manually max value. Auto adjust is disabled after a call of this method.
     */
    virtual void setMax(double max) = 0;

    /**
     * @brief set the gradient to use
     */
    virtual void setGradient(const QLinearGradient &gradient) = 0;

    /**
     * @brief set if he must use a shared gradient
     */
    virtual void setUseSharedGradient(bool val) = 0;

    // GETTER
    /**
     * @brief must used a shared gradient ?
     */
    virtual bool isUsedSharedGradient() const = 0;

    /**
     * @brief auto adjust ?
     */
    virtual bool isAutoAdjust() const = 0;

    /**
     * @brief true if auto adjust gradient is enabled.
     */
    virtual bool autoAdjust() const = 0;

    /**
     * @brief min value used.
     */
    virtual double min() const = 0;

    /**
     * @brief max value used.
     */
    virtual double max() const = 0;

    /**
     * @brief the gradient used
     */
    virtual QLinearGradient gradient() const = 0;
};

#endif // DM_ABSTRACTATTRIBUTESSCALAR_H
