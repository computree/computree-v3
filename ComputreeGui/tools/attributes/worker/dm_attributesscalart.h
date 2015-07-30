#ifndef DM_ATTRIBUTESSCALART_H
#define DM_ATTRIBUTESSCALART_H

#include "tools/attributes/worker/abstract/dm_abstractattributesscalar.h"
#include "tools/graphicsview/dm_colorlinearinterpolator.h"

#include "view/DocumentView/gdocumentviewforgraphics.h"

#include "ct_attributes/abstract/ct_abstractattributesscalar.h"
#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"

#include <QPropertyAnimation>
#include <QFutureWatcher>

template<typename Type>
class DM_AttributesScalarT : public DM_AbstractAttributesScalar
{
public:
    DM_AttributesScalarT();
    ~DM_AttributesScalarT();

    /**
     * @brief Check if the doc has the necessary cloud (color cloud) or set it if not
     */
    void checkAndSetNecessaryCloudToDoc() {}

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
    bool setTypeAttributes(const Type *ta, const CT_AbstractAttributesScalar *as);

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
    CT_AbstractAttributesScalar* scalarAttributes() const;
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
        DM_ColorLinearInterpolator      *m_interpolator;
        CT_AbstractAttributesScalar     *m_as;
        const CT_AbstractCloudIndex     *m_index;
        osg::ref_ptr<GOsgGraphicsView::ColorArrayType> m_cc;
        CT_FaceAccessor                 *m_fAccess;
        CT_EdgeAccessor                 *m_eAccess;
        double                          m_range;
        double                          m_manualMin;
        size_t                          m_begin;
        size_t                          m_end;
    };

    CT_AbstractAttributesScalar     *m_as;
    bool                            m_autoAdjust;
    double                          m_manualMin;
    double                          m_manualMax;
    QLinearGradient                 m_gradient;
    bool                            m_useSharedGradient;
    CT_FaceAccessor                 m_fAccess;
    CT_EdgeAccessor                 m_eAccess;
    QFutureWatcher<void>            m_watcher;

    void autoAdjustMinMax();
    void constructColorInterpolator(DM_ColorLinearInterpolator &interpolator) const;

    static void staticApply(ConcurrentMapInfo *info) { Q_UNUSED(info) }
};

// specialisation for points
template<>
void DM_AttributesScalarT<CT_AbstractPointsAttributes>::staticApply(DM_AttributesScalarT<CT_AbstractPointsAttributes>::ConcurrentMapInfo *info);

// specialisation for faces
template<>
void DM_AttributesScalarT<CT_AbstractFaceAttributes>::staticApply(DM_AttributesScalarT<CT_AbstractFaceAttributes>::ConcurrentMapInfo *info);

// specialisation for edges
template<>
void DM_AttributesScalarT<CT_AbstractEdgeAttributes>::staticApply(DM_AttributesScalarT<CT_AbstractEdgeAttributes>::ConcurrentMapInfo *info);

#include "tools/attributes/worker/dm_attributesscalart.hpp"

#endif // DM_ATTRIBUTESSCALART_H
