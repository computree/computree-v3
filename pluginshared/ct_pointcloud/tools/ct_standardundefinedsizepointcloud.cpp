#include "ct_standardundefinedsizepointcloud.h"

#include "ct_global/ct_context.h"
#include "ct_point.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

size_t CT_StandardUndefinedSizePointCloud::beginIndex() const
{
    return m_bIndex;
}

size_t CT_StandardUndefinedSizePointCloud::lastIndex() const
{
    size_t s = size();

    if(s == 0)
        return m_bIndex;

    return m_bIndex+(s-1);
}

size_t CT_StandardUndefinedSizePointCloud::size() const
{
    return m_pc->size()-m_bIndex;
}

void CT_StandardUndefinedSizePointCloud::addPoint(const Eigen::Vector3d &point, const CT_AbstractCoordinateSystem *coordinateSystem)
{
    CT_PointData &pData = m_pc->addT();

    if(coordinateSystem == NULL) {
        m_csIndexes.push_back(0);
        PS_COORDINATES_SYS_MANAGER->coordinateSystemAt(0)->convertImport(point(CT_Point::X), point(CT_Point::Y), point(CT_Point::Z), pData(CT_PointData::X), pData(CT_PointData::Y), pData(CT_PointData::Z));
    } else {
        m_csIndexes.push_back(PS_COORDINATES_SYS_MANAGER->indexOfCoordinateSystem(coordinateSystem));
        coordinateSystem->convertImport(point(CT_Point::X), point(CT_Point::Y), point(CT_Point::Z), pData(CT_PointData::X), pData(CT_PointData::Y), pData(CT_PointData::Z));
    }
}

void CT_StandardUndefinedSizePointCloud::addPoint(const Eigen::Vector3d &point, const GLuint &coordinateSystemGlobalIndex)
{
    CT_AbstractCoordinateSystem *sys = PS_COORDINATES_SYS_MANAGER->coordinateSystemAt(coordinateSystemGlobalIndex);

    CT_PointData &pData = m_pc->addT();
    m_csIndexes.push_back(coordinateSystemGlobalIndex);

    sys->convertImport(point(CT_Point::X), point(CT_Point::Y), point(CT_Point::Z), pData(CT_PointData::X), pData(CT_PointData::Y), pData(CT_PointData::Z));
}

void CT_StandardUndefinedSizePointCloud::addInternalPoint(const CT_PointData &point, const GLuint &coordinateSystemGlobalIndex)
{
    m_pc->addT(point);
    m_csIndexes.push_back(coordinateSystemGlobalIndex);
}

void CT_StandardUndefinedSizePointCloud::addInternalPoint(const CT_PointData &point, const CT_AbstractCoordinateSystem *coordinateSystem)
{
    m_pc->addT(point);

    if(coordinateSystem == NULL)
        m_csIndexes.push_back(0);
    else
        m_csIndexes.push_back(PS_COORDINATES_SYS_MANAGER->indexOfCoordinateSystem(coordinateSystem));
}

CT_StandardUndefinedSizePointCloud::CT_StandardUndefinedSizePointCloud(const size_t &beginIndex, const CT_PointCloudStdVector *cloud) : CT_AbstractUndefinedSizePointCloud()
{
    m_bIndex = beginIndex;
    m_pc = (CT_PointCloudStdVector*)cloud;
}

const std::vector<GLuint>& CT_StandardUndefinedSizePointCloud::coordinateSystemIndexOfPointsAdded() const
{
    return m_csIndexes;
}
