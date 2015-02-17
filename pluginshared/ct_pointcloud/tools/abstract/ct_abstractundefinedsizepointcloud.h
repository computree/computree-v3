#ifndef CT_ABSTRACTUNDEFINEDSIZEPOINTCLOUD_H
#define CT_ABSTRACTUNDEFINEDSIZEPOINTCLOUD_H

#include <QObject>

#include "ct_defines.h"
#include "ct_coordinates/abstract/ct_abstractcoordinatesystem.h"

/**
 * Use this class when you want to create a cloud of points if you
 * don't know it's size by advance.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractUndefinedSizePointCloud : public QObject
{
    Q_OBJECT

public:
    CT_AbstractUndefinedSizePointCloud();
    virtual ~CT_AbstractUndefinedSizePointCloud();

    /**
     * @brief Returns the global index of the first point in this cloud
     */
    virtual size_t beginIndex() const = 0;

    /**
     * @brief Returns the global index of the last point in this cloud
     */
    virtual size_t lastIndex() const = 0;

    /**
     * @brief Returns the current size of this cloud
     */
    virtual size_t size() const = 0;

    /**
     * @brief Add a point to this cloud. You must pass a coordinate system that will be used to transform the point in
     *        float values.
     */
    virtual void addPoint(const CT_Point &point, const CT_AbstractCoordinateSystem *coordinateSystem) = 0;

    /**
     * @brief Add a point to this cloud. You must pass the index of the coordinate system that will be used to transform the point in
     *        float values.
     */
    virtual void addPoint(const CT_Point &point, const GLuint &coordinateSystemGlobalIndex) = 0;

    /**
     * @brief Add a point to this cloud. You must pass a coordinate system that will be set for this point
     */
    virtual void addInternalPoint(const CT_PointData &point, const CT_AbstractCoordinateSystem *coordinateSystem) = 0;

    /**
     * @brief Add a point to this cloud. You must pass the index of the coordinate system that will be set for this point
     */
    virtual void addInternalPoint(const CT_PointData &point, const GLuint &coordinateSystemGlobalIndex) = 0;

protected:
    /**
     * @brief Return the global point cloud
     */
    CT_AbstractPointCloud* globalPointCloud() const;

    /**
     * @brief Returns global indexes of coordinate system of points added
     */
    virtual const std::vector<GLuint>& coordinateSystemIndexOfPointsAdded() const = 0;
};

#endif // CT_ABSTRACTUNDEFINEDSIZEPOINTCLOUD_H
