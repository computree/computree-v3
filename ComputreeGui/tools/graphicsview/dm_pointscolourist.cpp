#include "dm_pointscolourist.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtConcurrentMap>
#else
#include <QtConcurrent/QtConcurrentMap>
#endif

#include "ct_accessor/ct_pointaccessor.h"
#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"
#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"

#define N_POINTS_MAX_BY_THREAD 10000

DM_PointsColourist::DM_PointsColourist()
{
}

void DM_PointsColourist::colorGlobalPointsWithUniqueColor(CT_AbstractColorCloud *colorCloud,
                                                          const QList<CT_AbstractCloudIndex *> &l,
                                                          const QColor &color)
{
    QList<DM_PointsColourist::ConcurrentMapInfo> list = staticCreateGenericConcurrentMap(colorCloud, l);

    if(list.empty())
        return;

    CT_Color uniqueColor = staticConvertQColorToCtColor(color);

    QMutableListIterator<DM_PointsColourist::ConcurrentMapInfo> it(list);

    while(it.hasNext())
    {
        DM_PointsColourist::ConcurrentMapInfo info = it.next();
        info.m_uniqueColor = uniqueColor;

        it.setValue(info);
    };

    if(list.size() > 1) {
        QFuture<void> future = QtConcurrent::map(list, &DM_PointsColourist::staticApplyUniqueColor);
        future.waitForFinished();
    } else {
        staticApplyUniqueColor(list.first());
    }
}

void DM_PointsColourist::colorGlobalPointsByXCoordinate(CT_AbstractColorCloud *colorCloud,
                                                        const QList<CT_AbstractCloudIndex *> &l,
                                                        const QGradient &gradient,
                                                        Eigen::Vector2d optionnalMinMaxX,
                                                        bool useOptionnal)
{
    colorGlobalPointsByCoordinate(colorCloud,
                                  l,
                                  gradient,
                                  CT_Point::X,
                                  optionnalMinMaxX,
                                  useOptionnal);
}

void DM_PointsColourist::colorGlobalPointsByYCoordinate(CT_AbstractColorCloud *colorCloud,
                                                        const QList<CT_AbstractCloudIndex *> &l,
                                                        const QGradient &gradient,
                                                        Eigen::Vector2d optionnalMinMaxY,
                                                        bool useOptionnal)
{
    colorGlobalPointsByCoordinate(colorCloud,
                                  l,
                                  gradient,
                                  CT_Point::Y,
                                  optionnalMinMaxY,
                                  useOptionnal);
}

void DM_PointsColourist::colorGlobalPointsByZCoordinate(CT_AbstractColorCloud *colorCloud,
                                                        const QList<CT_AbstractCloudIndex *> &l,
                                                        const QGradient &gradient,
                                                        Eigen::Vector2d optionnalMinMaxZ,
                                                        bool useOptionnal)
{
    colorGlobalPointsByCoordinate(colorCloud,
                                  l,
                                  gradient,
                                  CT_Point::Z,
                                  optionnalMinMaxZ,
                                  useOptionnal);
}

void DM_PointsColourist::colorGlobalPointsByCoordinate(CT_AbstractColorCloud *colorCloud,
                                                       const QList<CT_AbstractCloudIndex *> &l,
                                                       const QGradient &gradient,
                                                       quint8 coordinateIndex,
                                                       Eigen::Vector2d optionnalMinMax,
                                                       bool useOptionnal)
{
    CT_PointAccessor pAccess;

    if(!useOptionnal)
        staticComputeMinAndMaxOfPoints(l, optionnalMinMax, coordinateIndex, pAccess);

    QList<DM_PointsColourist::ConcurrentMapInfo> list = staticCreateGenericConcurrentMap(colorCloud, l);

    if(list.empty())
        return;

    DM_ColorLinearInterpolator interpolator;
    interpolator.constructFromQGradient(gradient);

    staticUpdateConcurrentMapForCoordinate(list, pAccess, interpolator, optionnalMinMax, coordinateIndex);

    if(list.size() > 1) {
        QFuture<void> future = QtConcurrent::map(list, &DM_PointsColourist::staticApplyGradientColorByCoordinate);
        future.waitForFinished();
    } else {
        staticApplyGradientColorByCoordinate(list.first());
    }
}

CT_Color DM_PointsColourist::staticConvertQColorToCtColor(const QColor &color)
{
    return CT_Color(color.red(), color.green(), color.blue(), color.alpha());
}

void DM_PointsColourist::staticApplyUniqueColor(DM_PointsColourist::ConcurrentMapInfo &info)
{
    for(size_t i=info.m_begin; i<info.m_end; ++i) {
        size_t globalIndex = info.m_indexCloud->indexAt(i);

        CT_Color &color = info.m_colorCloud->colorAt(globalIndex);

        if(info.m_uniqueColor.a() == KEEP_ALPHA_COLOR) {
            color.r() = info.m_uniqueColor.r();
            color.g() = info.m_uniqueColor.g();
            color.b() = info.m_uniqueColor.b();
        } else {
            color.setColor(info.m_uniqueColor);
        }
    }
}

void DM_PointsColourist::staticApplyGradientColorByCoordinate(DM_PointsColourist::ConcurrentMapInfo &info)
{
    CT_Point p;

    for(size_t i=info.m_begin; i<info.m_end; ++i) {
        size_t globalIndex = info.m_indexCloud->indexAt(i);

        info.m_pAccess->pointAt(globalIndex, p);

        CT_Color &color = info.m_colorCloud->colorAt(globalIndex);

        // convert the coordinate to be between 0 and 1
        double key = (p[info.m_coordinateIndex]-info.m_min)/info.m_range;

        QColor newColor = info.m_colorInterpolator->intermediateColor(key);
        color = newColor;
    }
}

void DM_PointsColourist::staticComputeMinAndMaxOfPoints(const QList<CT_AbstractCloudIndex *> &l,
                                                        Eigen::Vector2d &minMax,
                                                        quint8 coordinateIndex,
                                                        CT_PointAccessor &pAccess)
{
    CT_Point p;

    minMax[0] = std::numeric_limits<double>::max();
    minMax[1] = -minMax[0];

    QListIterator<CT_AbstractCloudIndex*> it(l);

    while(it.hasNext()) {
        CT_AbstractCloudIndex *ci = it.next();

        size_t size = ci->size();

        for(size_t i=0; i<size; ++i) {
             pAccess.pointAt(ci->indexAt(i), p);

             minMax[0] = qMin(minMax[0], p[coordinateIndex]);
             minMax[1] = qMax(minMax[1], p[coordinateIndex]);
        }
    }
}

QList<DM_PointsColourist::ConcurrentMapInfo> DM_PointsColourist::staticCreateGenericConcurrentMap(CT_AbstractColorCloud *colorCloud,
                                                                                                  const QList<CT_AbstractCloudIndex *> &l)
{
    QList<DM_PointsColourist::ConcurrentMapInfo> list;
    QListIterator<CT_AbstractCloudIndex*> it(l);

    while(it.hasNext())
    {
        CT_AbstractCloudIndex *ci = it.next();
        size_t size = ci->size();

        size_t i = 0;

        while(i < size)
        {
            DM_PointsColourist::ConcurrentMapInfo info;
            info.m_colorCloud = colorCloud;
            info.m_indexCloud = ci;
            info.m_begin = i;

            i += N_POINTS_MAX_BY_THREAD;

            if(i >= size)
                i = size;

            info.m_end = i;

            list << info;
        };
    };

    return list;
}

void DM_PointsColourist::staticUpdateConcurrentMapForCoordinate(QList<DM_PointsColourist::ConcurrentMapInfo> &list,
                                                                CT_PointAccessor &pAccess,
                                                                DM_ColorLinearInterpolator &interpolator,
                                                                Eigen::Vector2d minMax,
                                                                quint8 coordinateIndex)
{
    QMutableListIterator<DM_PointsColourist::ConcurrentMapInfo> it(list);

    while(it.hasNext())
    {
        DM_PointsColourist::ConcurrentMapInfo info = it.next();
        info.m_pAccess = &pAccess;
        info.m_coordinateIndex = coordinateIndex;
        info.m_colorInterpolator = &interpolator;
        info.m_min = minMax[0];
        info.m_range = minMax[1]-minMax[0];

        if(info.m_range == 0)
            info.m_range = 1;

        it.setValue(info);
    };
}
