#ifndef DM_ATTRIBUTESSCALART_H
#define DM_ATTRIBUTESSCALART_H

#include "interfaces.h"
#include "tools/attributes/worker/abstract/dm_abstractattributesscalar.h"

#include "view/DocumentView/gdocumentviewforgraphics.h"

#include <QPropertyAnimation>
#include <QFutureWatcher>

template<typename Type, typename TypeCloudIndex>
class DM_AttributesScalarT : public DM_AbstractAttributesScalar
{
public:
    DM_AttributesScalarT();
    ~DM_AttributesScalarT();

    void checkAndSetColorCloudToDoc();

    // SETTER
    /**
     * @brief set auto adjust gradient from min() and max() values of points attributes
     */
    void setAutoAdjust(bool automatic);

    /**
     * @brief Set manually min value. Auto adjust is disabled after a call of this method.
     */
    void setMin(double min);

    /**
     * @brief Set manually max value. Auto adjust is disabled after a call of this method.
     */
    void setMax(double max);

    /**
     * @brief set the gradient to use
     */
    void setGradient(const QLinearGradient &gradient);

    /**
     * @brief set if he must use a shared gradient
     */
    void setUseSharedGradient(bool val);

    /**
     * @brief accept only Type (Point, Face, etc...) and scalar
     */
    bool setTypeAttributes(const Type *ta, const IAttributesScalar *as);

    // GETTER
    /**
     * @brief must used a shared gradient ?
     */
    bool isUsedSharedGradient() const;

    /**
     * @brief auto adjust ?
     */
    bool isAutoAdjust() const;

    /**
     * @brief true if auto adjust gradient is enabled.
     */
    bool autoAdjust() const;

    /**
     * @brief min value used.
     */
    double min() const;

    /**
     * @brief max value used.
     */
    double max() const;

    /**
     * @brief the gradient used
     */
    QLinearGradient gradient() const;

    /**
     * @brief getter of I[Type]Attributes
     */
    IAttributesScalar* scalarAttributes() const;
    Type* abstractTypeAttributes() const;

protected:

    /**
     * @brief Apply the color to the [Type] of the document. Use [Type] cloud index from I[Type]Attributes.
     * @param doc : document to use
     */
    bool process(GDocumentViewForGraphics *doc);

    void attributesDeleted();

private:

    struct ConcurrentMapInfo
    {
    public:
        QPropertyAnimation      m_interpolator;
        IAttributesScalar       *m_as;
        const TypeCloudIndex    *m_index;
        IColorCloud             *m_cc;
        double                  m_range;
        double                  m_manualMin;
        double                  m_granularity;
        size_t                  m_begin;
        size_t                  m_end;
    };

    IAttributesScalar           *m_as;
    bool                        m_autoAdjust;
    double                      m_manualMin;
    double                      m_manualMax;
    QLinearGradient             m_gradient;
    bool                        m_useSharedGradient;
    QFutureWatcher<void>        m_watcher;

    void autoAdjustMinMax();
    void constructColorInterpolator(QPropertyAnimation &interpolator, int granularity) const;

    static void staticApply(ConcurrentMapInfo *info);
};

#include "tools/attributes/worker/dm_attributesscalart.hpp"

#endif // DM_ATTRIBUTESSCALART_H
