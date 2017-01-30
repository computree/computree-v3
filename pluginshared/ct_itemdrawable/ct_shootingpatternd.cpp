#include "ct_shootingpatternd.h"

CT_ShootingPatternD::CT_ShootingPatternD() : CT_AbstractItemDrawableWithoutPointCloud()
{
    m_pattern = NULL;
}

CT_ShootingPatternD::CT_ShootingPatternD(const CT_OutAbstractSingularItemModel *model,
                                         const CT_AbstractResult *result,
                                         CT_ShootingPattern *pattern) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    m_pattern = pattern;

    if(m_pattern != NULL)
        setCenterCoordinate(m_pattern->getOrigin());
}

CT_ShootingPatternD::CT_ShootingPatternD(const QString &modelName,
                                         const CT_AbstractResult *result,
                                         CT_ShootingPattern *pattern) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    m_pattern = pattern;

    if(m_pattern != NULL)
        setCenterCoordinate(m_pattern->getOrigin());
}

CT_ShootingPatternD::CT_ShootingPatternD(const CT_ShootingPatternD& other) : CT_AbstractItemDrawableWithoutPointCloud((const CT_OutAbstractSingularItemModel*)other.model(), other.result())
{
    m_pattern = ((other.m_pattern == NULL) ? NULL : other.m_pattern->clone());
}

CT_ShootingPatternD::~CT_ShootingPatternD()
{
    delete m_pattern;
}

CT_ShootingPattern* CT_ShootingPatternD::getShootingPattern() const
{
    return m_pattern;
}

const Eigen::Vector3d &CT_ShootingPatternD::getOrigin() const
{
    return m_pattern->getOrigin();
}

size_t CT_ShootingPatternD::getNumberOfShots() const
{
    return m_pattern->getNumberOfShots();
}

CT_Shot CT_ShootingPatternD::getShotAt(const size_t &index) const
{
    return m_pattern->getShotAt(index);
}

void CT_ShootingPatternD::getShotAt(const size_t &index, CT_Shot &shot) const
{
    m_pattern->getShotAt(index, shot);
}

CT_ShootingPatternD* CT_ShootingPatternD::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    return new CT_ShootingPatternD((const CT_OutAbstractSingularItemModel*)model, result, (m_pattern == NULL) ? NULL : m_pattern->clone());
}

