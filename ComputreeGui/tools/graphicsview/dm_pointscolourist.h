#ifndef DM_POINTSCOLOURIST_H
#define DM_POINTSCOLOURIST_H

#include <QList>
#include <QGradient>
#include "eigen/Eigen/Core"
#include "ct_color.h"
#include "tools/graphicsview/dm_colorlinearinterpolator.h"

// set alpha color to this value to keep alpha color of previous color
#define KEEP_ALPHA_COLOR 0

class CT_PointAccessor;
class CT_AbstractColorCloud;
class CT_AbstractCloudIndex;

/**
 * @brief This tool class can be used to color points in a color cloud of a graphics view or in a simple color cloud
 */
class DM_PointsColourist
{
public:
    DM_PointsColourist();

    /**
     * @brief This method will color all global points defined in the list of index cloud with a unique color
     * @param colorCloud : the color cloud that must be modified
     * @param l : the list of cloud index that contains global index of points (and global index of color in the color cloud)
     * @param color : the unique color to apply
     */
    static void colorGlobalPointsWithUniqueColor(CT_AbstractColorCloud *colorCloud, const QList<CT_AbstractCloudIndex*> &l, const QColor &color);

    /**
     * @brief This method will color all global points defined in the list of index cloud by X coordinate and use a gradient to map the color
     * @param colorCloud : the color cloud that must be modified
     * @param gradient : a gradient to map a value to a color
     * @param l : the list of cloud index that contains global index of points (and global index of color in the color cloud)
     * @param optionnalMinMaxX : the min and max X coordinate if you know it
     * @param useOptionnal : set to true if you have set the parameter optionnalMinMaxX
     */
    static void colorGlobalPointsByXCoordinate(CT_AbstractColorCloud *colorCloud,
                                               const QList<CT_AbstractCloudIndex*> &l,
                                               const QGradient &gradient,
                                               Eigen::Vector2d optionnalMinMaxX = Eigen::Vector2d(),
                                               bool useOptionnal = false);

    /**
     * @brief This method will color all global points defined in the list of index cloud by Y coordinate and use a gradient to map the color
     * @param colorCloud : the color cloud that must be modified
     * @param gradient : a gradient to map a value to a color
     * @param l : the list of cloud index that contains global index of points (and global index of color in the color cloud)
     * @param optionnalMinMaxY : the min and max Y coordinate if you know it
     * @param useOptionnal : set to true if you have set the parameter optionnalMinMaxY
     */
    static void colorGlobalPointsByYCoordinate(CT_AbstractColorCloud *colorCloud,
                                               const QList<CT_AbstractCloudIndex*> &l,
                                               const QGradient &gradient,
                                               Eigen::Vector2d optionnalMinMaxY = Eigen::Vector2d(),
                                               bool useOptionnal = false);

    /**
     * @brief This method will color all global points defined in the list of index cloud by Z coordinate and use a gradient to map the color
     * @param colorCloud : the color cloud that must be modified
     * @param gradient : a gradient to map a value to a color
     * @param l : the list of cloud index that contains global index of points (and global index of color in the color cloud)
     * @param optionnalMinMaxZ : the min and max Z coordinate if you know it
     * @param useOptionnal : set to true if you have set the parameter optionnalMinMaxZ
     */
    static void colorGlobalPointsByZCoordinate(CT_AbstractColorCloud *colorCloud,
                                               const QList<CT_AbstractCloudIndex*> &l,
                                               const QGradient &gradient,
                                               Eigen::Vector2d optionnalMinMaxZ = Eigen::Vector2d(),
                                               bool useOptionnal = false);


    /**
     * @brief This method will color all global points defined in the list of index cloud by a coordinate (X, Y or Z) and use a gradient to map the color
     * @param colorCloud : the color cloud that must be modified
     * @param gradient : a gradient to map a value to a color
     * @param l : the list of cloud index that contains global index of points (and global index of color in the color cloud)
     * @param coordinateIndex : the coordinate index to use (CT_Point::X or CT_Point::Y or CT_Point::Z)
     * @param optionnalMinMax : the min and max coordinate if you know it
     * @param useOptionnal : set to true if you have set the parameter optionnalMinMax
     */
    static void colorGlobalPointsByCoordinate(CT_AbstractColorCloud *colorCloud,
                                              const QList<CT_AbstractCloudIndex*> &l,
                                              const QGradient &gradient,
                                              quint8 coordinateIndex,
                                              Eigen::Vector2d optionnalMinMax = Eigen::Vector2d(),
                                              bool useOptionnal = false);

    /**
     * @brief Convert a QColor to a CT_Color
     */
    static CT_Color staticConvertQColorToCtColor(const QColor &color);

private:
    struct ConcurrentMapInfo {
        CT_AbstractColorCloud   *m_colorCloud;
        CT_AbstractCloudIndex   *m_indexCloud;
        CT_Color                m_uniqueColor;
        size_t                  m_begin;
        size_t                  m_end;

        CT_PointAccessor            *m_pAccess;
        quint8                      m_coordinateIndex;
        DM_ColorLinearInterpolator  *m_colorInterpolator;
        double                      m_min;
        double                      m_range;
    };

    /**
     * @brief Apply unique color to points (can be used in multithread)
     */
    static void staticApplyUniqueColor(ConcurrentMapInfo &info);

    /**
     * @brief Apply a gradient color to points (can be used in multithread)
     */
    static void staticApplyGradientColorByCoordinate(ConcurrentMapInfo &info);

    /**
     * @brief Get the min and max of the coordinate "coordinateIndex" (CT_Point::X or CT_Point::Y, etc...) of all points that was in the list of index cloud
     */
    static void staticComputeMinAndMaxOfPoints(const QList<CT_AbstractCloudIndex*> &l, Eigen::Vector2d &minMax, quint8 coordinateIndex, CT_PointAccessor &pAccess);

    /**
     * @brief Create the list of DM_PointsColourist::ConcurrentMapInfo, you must use it after to fill with your parameters
     */
    static QList<DM_PointsColourist::ConcurrentMapInfo> staticCreateGenericConcurrentMap(CT_AbstractColorCloud *colorCloud, const QList<CT_AbstractCloudIndex *> &l);

    /**
     * @brief Update the list of DM_PointsColourist::ConcurrentMapInfo to be used after with a method that change color of points by coordinate
     */
    static void staticUpdateConcurrentMapForCoordinate(QList<DM_PointsColourist::ConcurrentMapInfo> &list, CT_PointAccessor &pAccess, DM_ColorLinearInterpolator &interpolator, Eigen::Vector2d minMax, quint8 coordinateIndex);
};

#endif // DM_POINTSCOLOURIST_H
