#ifndef DM_ATTRIBUTESSCALART_HPP
#define DM_ATTRIBUTESSCALART_HPP

#include "tools/attributes/worker/dm_attributesscalart.h"

#include "dm_guimanager.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtConcurrentMap>
#else
#include <QtConcurrent/QtConcurrentMap>
#endif

template<typename Type, typename TypeCloudIndex>
DM_AttributesScalarT<Type, TypeCloudIndex>::DM_AttributesScalarT() : DM_AbstractAttributesScalar()
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

template<typename Type, typename TypeCloudIndex>
DM_AttributesScalarT<Type, TypeCloudIndex>::~DM_AttributesScalarT()
{
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesScalarT<Type, TypeCloudIndex>::checkAndSetColorCloudToDoc()
{
    GDocumentViewForGraphics *doc = document();

    if(doc->colorCloudRegistered<Type>().data() == NULL)
        doc->createColorCloudRegistered<Type>();
}


template<typename Type, typename TypeCloudIndex>
void DM_AttributesScalarT<Type, TypeCloudIndex>::setAutoAdjust(bool automatic)
{
    m_autoAdjust = automatic;

    if(m_autoAdjust)
        autoAdjustMinMax();
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesScalarT<Type, TypeCloudIndex>::setMin(double min)
{
    m_manualMin = min;
    setAutoAdjust(false);
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesScalarT<Type, TypeCloudIndex>::setMax(double max)
{
    m_manualMax = max;
    setAutoAdjust(false);
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesScalarT<Type, TypeCloudIndex>::setGradient(const QLinearGradient &gradient)
{
    m_gradient = gradient;
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesScalarT<Type, TypeCloudIndex>::setUseSharedGradient(bool val)
{
    m_useSharedGradient = val;
}

template<typename Type, typename TypeCloudIndex>
bool DM_AttributesScalarT<Type, TypeCloudIndex>::setTypeAttributes(const Type *ta, const IAttributesScalar *as)
{
    if(ta != dynamic_cast<const Type*>(as))
        return false;

    setAttributes(ta);
    m_as = (IAttributesScalar*)as;

    if(m_autoAdjust)
        autoAdjustMinMax();

    return true;
}

template<typename Type, typename TypeCloudIndex>
bool DM_AttributesScalarT<Type, TypeCloudIndex>::isUsedSharedGradient() const
{
    return m_useSharedGradient;
}

template<typename Type, typename TypeCloudIndex>
bool DM_AttributesScalarT<Type, TypeCloudIndex>::isAutoAdjust() const
{
    return m_autoAdjust;
}

template<typename Type, typename TypeCloudIndex>
bool DM_AttributesScalarT<Type, TypeCloudIndex>::autoAdjust() const
{
    return m_autoAdjust;
}

template<typename Type, typename TypeCloudIndex>
double DM_AttributesScalarT<Type, TypeCloudIndex>::min() const
{
    return m_manualMin;
}

template<typename Type, typename TypeCloudIndex>
double DM_AttributesScalarT<Type, TypeCloudIndex>::max() const
{
    return m_manualMax;
}

template<typename Type, typename TypeCloudIndex>
QLinearGradient DM_AttributesScalarT<Type, TypeCloudIndex>::gradient() const
{
    return m_gradient;
}

template<typename Type, typename TypeCloudIndex>
bool DM_AttributesScalarT<Type, TypeCloudIndex>::process(GDocumentViewForGraphics *doc)
{
    if(m_as == NULL)
        return false;

    double range = m_manualMax-m_manualMin;

    if(range == 0)
        range = 1;

    double granularity = 10000000;

    QPropertyAnimation interpolator;
    constructColorInterpolator(interpolator, granularity);

    const TypeCloudIndex *index = abstractTypeAttributes()->cloudIndex();
    size_t size = index->indexSize();

    QSharedPointer<ColorCloudRegisteredInterface> spcc = doc->colorCloudRegistered<Type>();

    if(spcc.data() != NULL)
    {
        IColorCloud *cc = spcc->colorCloud();

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
            info->m_cc = cc;
            info->m_granularity = granularity;
            info->m_index = index;
            constructColorInterpolator(info->m_interpolator, granularity);
            info->m_manualMin = m_manualMin;
            info->m_range = range;

            list << info;
        };

        QtConcurrent::blockingMap(list, staticApply);

        qDeleteAll(list.begin(), list.end());

        doc->redrawGraphics();

        return true;
    }

    return false;
}


template<typename Type, typename TypeCloudIndex>
void DM_AttributesScalarT<Type, TypeCloudIndex>::attributesDeleted()
{
    m_as = NULL;
}

template<typename Type, typename TypeCloudIndex>
IAttributesScalar* DM_AttributesScalarT<Type, TypeCloudIndex>::scalarAttributes() const
{
    return m_as;
}

template<typename Type, typename TypeCloudIndex>
Type* DM_AttributesScalarT<Type, TypeCloudIndex>::abstractTypeAttributes() const
{
    return dynamic_cast<Type*>(abstractAttributes());
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesScalarT<Type, TypeCloudIndex>::autoAdjustMinMax()
{
    m_manualMin = m_as->dMin();
    m_manualMax = m_as->dMax();
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesScalarT<Type, TypeCloudIndex>::constructColorInterpolator(QPropertyAnimation &interpolator, int granularity) const
{
    interpolator.setEasingCurve(QEasingCurve::Linear);
    interpolator.setDuration(granularity);

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

        if(interpolator.keyValueAt(1).isNull())
            interpolator.setKeyValueAt(1, lastStop.second);

        if(interpolator.keyValueAt(0).isNull())
            interpolator.setKeyValueAt(0, firstStop.second);
    }
}

template<typename Type, typename TypeCloudIndex>
void DM_AttributesScalarT<Type, TypeCloudIndex>::staticApply(ConcurrentMapInfo *info)
{
    size_t indexP;

    for(size_t i=info->m_begin; i<info->m_end; ++i)
    {
        double vv = info->m_as->dValueAt(i);
        // convert the value to be between 0 and granularity
        int time = ((vv-info->m_manualMin)*info->m_granularity)/info->m_range;
        info->m_interpolator.setCurrentTime(time);

        // get the color
        QVariant val = info->m_interpolator.currentValue();
        QColor color = val.value<QColor>();
        info->m_index->indexAt(i, indexP);

        // set the color of the point at this document
        quint8 *bgra = info->m_cc->valueAt(indexP);
        *bgra = (color.blueF()*255.0);
        *(bgra+1) = (color.greenF()*255.0);
        *(bgra+2) = (color.redF()*255.0);
    }
}

#endif // DM_ATTRIBUTESSCALART_HPP
