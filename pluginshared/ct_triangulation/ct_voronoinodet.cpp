#include "ct_voronoinodet.h"

CT_VoronoiNodeT::CT_VoronoiNodeT(double x, double y, double z)
{
    _x = x;
    _y = y;
    _z = z;
}

double CT_VoronoiNodeT::getX() const
{
    return _x;
}

double CT_VoronoiNodeT::getY() const
{
    return _y;
}

double CT_VoronoiNodeT::getZ() const
{
    return _z;
}
