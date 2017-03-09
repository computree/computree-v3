#include "ct_thetaphishootingpattern.h"

#include <QtMath>

#include "ct_itemdrawable/ct_scanner.h"

CT_ThetaPhiShootingPattern::CT_ThetaPhiShootingPattern(const CT_Scanner *scanner) : CT_ThetaPhiShootingPattern(scanner->getPosition(),
                                                                                                                  scanner->getHFov(),
                                                                                                                  scanner->getVFov(),
                                                                                                                  scanner->getHRes(),
                                                                                                                  scanner->getVRes(),
                                                                                                                  scanner->getInitTheta(),
                                                                                                                  scanner->getInitPhi(),
                                                                                                                  scanner->getZVector(),
                                                                                                                  scanner->getClockWise(),
                                                                                                                  true)
{

}

CT_ThetaPhiShootingPattern::CT_ThetaPhiShootingPattern(const Eigen::Vector3d &origin,
                                                       double hFov,
                                                       double vFov,
                                                       double hRes,
                                                       double vRes,
                                                       double initTheta,
                                                       double initPhi,
                                                       const Eigen::Vector3d &zVector,
                                                       bool clockWise,
                                                       bool radians) :
    CT_ShootingPattern(), m_origin(origin)
{
    m_zVector = zVector;
    m_clockWise = clockWise;

    m_hFov = hFov;
    m_vFov = vFov;
    m_hRes = hRes;
    m_vRes = vRes;

    m_initTheta = (m_clockWise ? -initTheta : initTheta);
    m_initPhi = initPhi;

    if(!radians )
    {
        m_hFov = qDegreesToRadians(m_hFov);
        m_vFov = qDegreesToRadians(m_vFov);
        m_hRes = qDegreesToRadians(m_hRes);
        m_vRes = qDegreesToRadians(m_vRes);
        m_initTheta = qDegreesToRadians(m_initTheta);
        m_initPhi = qDegreesToRadians(m_initPhi);
    }

    // Calculates the number of horizontal and vertical rays
    m_nHRays = (int)ceil(fabs(m_hFov/m_hRes));
    m_nVRays = (int)ceil(fabs(m_vFov/m_vRes));
}

CT_ThetaPhiShootingPattern::CT_ThetaPhiShootingPattern(const CT_ThetaPhiShootingPattern &other) :
    CT_ShootingPattern(),
    m_origin(other.getOrigin())
{
    m_zVector = other.m_zVector;
    m_clockWise = other.m_clockWise;

    m_hFov = other.m_hFov;
    m_vFov = other.m_vFov;
    m_hRes = other.m_hRes;
    m_vRes = other.m_vRes;

    m_initTheta = other.m_initTheta;
    m_initPhi = other.m_initPhi;

    m_nHRays = other.m_nHRays;
    m_nVRays = other.m_nVRays;
}

size_t CT_ThetaPhiShootingPattern::getNumberOfShots() const
{
    return m_nHRays*m_nVRays;
}

CT_Shot CT_ThetaPhiShootingPattern::getShotAt(const size_t &index) const
{
    size_t i = index/getNVRays();
    size_t j = index - (i*getNVRays());

    double theta = getInitTheta() + (i * getHRes());
    double phi = getInitPhi() + (j * getVRes());

    // If clockwise, then real theta equals opposite to initial
    if(isClockWise())
        theta = -theta;

    // The direction is calculated using spherical coordinates
    double sinPhi = std::sin( phi );
    double sinTheta = std::sin ( theta );
    double cosTheta = std::cos ( theta );

    Eigen::Vector3d direction;
    direction.x() = sinPhi*cosTheta;
    direction.y() = sinPhi*sinTheta;
    direction.z() = std::cos(phi);
    return CT_Shot(m_origin, direction);
}

CT_ShootingPattern* CT_ThetaPhiShootingPattern::clone() const
{
    return new CT_ThetaPhiShootingPattern(*this);
}
