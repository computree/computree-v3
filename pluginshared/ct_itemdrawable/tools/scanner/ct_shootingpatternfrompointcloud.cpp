#include "ct_shootingpatternfrompointcloud.h"

#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h"

CT_ShootingPatternFromPointCloud::CT_ShootingPatternFromPointCloud(const Eigen::Vector3d& origin,
                                                                   CT_PCIR pcir) : CT_ShootingPattern(origin)
{
    m_pcir = pcir;
}

CT_ShootingPatternFromPointCloud::CT_ShootingPatternFromPointCloud(const CT_ShootingPatternFromPointCloud &other) : CT_ShootingPattern(other.getOrigin())
{
    m_pcir = other.m_pcir;
}

size_t CT_ShootingPatternFromPointCloud::getNumberOfShots() const
{
    return m_pcir->abstractCloudIndexT()->size();
}

void CT_ShootingPatternFromPointCloud::getShotDirectionAt(const size_t &index, Eigen::Vector3d &direction) const
{
    CT_Point p = m_pAccessor.pointAt(m_pcir->abstractCloudIndexT()->constIndexAt(index));

    direction = p - getOrigin();
}

CT_ShootingPattern* CT_ShootingPatternFromPointCloud::clone() const
{
    return new CT_ShootingPatternFromPointCloud(*this);
}
