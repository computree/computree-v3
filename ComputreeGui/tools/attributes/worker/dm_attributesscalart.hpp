#ifndef DM_ATTRIBUTESSCALART_HPP
#define DM_ATTRIBUTESSCALART_HPP

#include "tools/attributes/worker/dm_attributesscalart.h"

#include "dm_guimanager.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtConcurrentMap>
#else
#include <QtConcurrent/QtConcurrentMap>
#endif

template<typename Type>
DM_AttributesScalarT<Type>::DM_AttributesScalarT() : DM_AbstractAttributesScalar()
{
    m_as = NULL;
    m_autoAdjust = true;
    m_manualMin = 0;
    m_manualMax = 1;
    m_useSharedGradient = false;

    connect(&m_watcher, SIGNAL(progressRangeChanged(int,int)), this, SLOT(setProgressRanged(int,int)), Qt::DirectConnection);
    connect(&m_watcher, SIGNAL(progressValueChanged(int)), this, SLOT(setProgress(int)), Qt::DirectConnection);
    connect(this, SIGNAL(canceled()), &m_watcher, SLOT(cancel()));
}

template<typename Type>
DM_AttributesScalarT<Type>::~DM_AttributesScalarT()
{
}

template<typename Type>
void DM_AttributesScalarT<Type>::setAutoAdjust(bool automatic)
{
    m_autoAdjust = automatic;

    if(m_autoAdjust)
        autoAdjustMinMax();
}

template<typename Type>
void DM_AttributesScalarT<Type>::setMin(double min)
{
    m_manualMin = min;
    setAutoAdjust(false);
}

template<typename Type>
void DM_AttributesScalarT<Type>::setMax(double max)
{
    m_manualMax = max;
    setAutoAdjust(false);
}

template<typename Type>
void DM_AttributesScalarT<Type>::setGradient(const QLinearGradient &gradient)
{
    m_gradient = gradient;
}

template<typename Type>
void DM_AttributesScalarT<Type>::setUseSharedGradient(bool val)
{
    m_useSharedGradient = val;
}

template<typename Type>
bool DM_AttributesScalarT<Type>::setTypeAttributes(const Type *ta, const CT_AbstractAttributesScalar *as)
{
    if(ta != dynamic_cast<const Type*>(as))
        return false;

    setAttributes(ta);
    m_as = (CT_AbstractAttributesScalar*)as;

    if(m_autoAdjust)
        autoAdjustMinMax();

    return true;
}

template<typename Type>
bool DM_AttributesScalarT<Type>::isUsedSharedGradient() const
{
    return m_useSharedGradient;
}

template<typename Type>
bool DM_AttributesScalarT<Type>::isAutoAdjust() const
{
    return m_autoAdjust;
}

template<typename Type>
bool DM_AttributesScalarT<Type>::autoAdjust() const
{
    return m_autoAdjust;
}

template<typename Type>
double DM_AttributesScalarT<Type>::min() const
{
    return m_manualMin;
}

template<typename Type>
double DM_AttributesScalarT<Type>::max() const
{
    return m_manualMax;
}

template<typename Type>
QLinearGradient DM_AttributesScalarT<Type>::gradient() const
{
    return m_gradient;
}

template<typename Type>
bool DM_AttributesScalarT<Type>::process(GDocumentViewForGraphics *doc)
{
    if(m_as == NULL)
        return false;

    double range = m_manualMax-m_manualMin;

    if(range == 0)
        range = 1;

    DM_ColorLinearInterpolator interpolator;
    constructColorInterpolator(interpolator);

    const CT_AbstractCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();
    size_t size = index->size();

    osg::ref_ptr<GOsgGraphicsView::ColorArrayType> colorArray = doc->getOrCreateGlobalColorArrayForPoints();

    if(colorArray.valid())
    {
        QList<ConcurrentMapInfo*>   list;

        size_t i = 0;

        while(i < size)
        {
            ConcurrentMapInfo *info = new ConcurrentMapInfo();
            info->m_as = m_as;
            info->m_begin = i;

            i += 100000;

            if(i >= size)
                i = size;

            info->m_end = i;
            info->m_cc = colorArray;
            info->m_index = index;
            info->m_interpolator = &interpolator;
            info->m_manualMin = m_manualMin;
            info->m_range = range;
            info->m_fAccess = &m_fAccess;
            info->m_eAccess = &m_eAccess;

            list << info;
        };

        QFuture<void> future = QtConcurrent::map(list, staticApply);
        m_watcher.setFuture(future);

        future.waitForFinished();

        qDeleteAll(list.begin(), list.end());

        doc->dirtyColorsOfPoints();

        return true;
    }

    return false;
}

template<typename Type>
void DM_AttributesScalarT<Type>::attributesDeleted()
{
    m_as = NULL;
}

template<typename Type>
CT_AbstractAttributesScalar* DM_AttributesScalarT<Type>::scalarAttributes() const
{
    return m_as;
}

template<typename Type>
Type* DM_AttributesScalarT<Type>::abstractTypeAttributes() const
{
    return dynamic_cast<Type*>(abstractAttributes());
}

template<typename Type>
void DM_AttributesScalarT<Type>::autoAdjustMinMax()
{
    m_manualMin = m_as->dMin();
    m_manualMax = m_as->dMax();
}

template<typename Type>
void DM_AttributesScalarT<Type>::constructColorInterpolator(DM_ColorLinearInterpolator &interpolator) const
{
    QGradientStops stops = m_gradient.stops();

    if(!stops.isEmpty())
    {
        QGradientStop lastStop;
        lastStop.first = -1;

        QGradientStop firstStop;
        firstStop.first = 2;

        QVectorIterator< QGradientStop > it(stops);

        while(it.hasNext())
        {
            const QGradientStop &stop = it.next();

            interpolator.setKeyValueAt(stop.first, stop.second);

            if(stop.first > lastStop.first)
                lastStop = stop;

            if(stop.first < firstStop.first)
                firstStop = stop;
        }
    }

    interpolator.finalize();
}

#endif // DM_ATTRIBUTESSCALART_HPP
