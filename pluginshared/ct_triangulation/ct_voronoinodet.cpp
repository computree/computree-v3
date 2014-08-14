#include "ct_voronoinodet.h"

CT_VoronoiNodeT::CT_VoronoiNodeT(float x, float y, float z)
{
    _x = x;
    _y = y;
    _z = z;
}

float CT_VoronoiNodeT::getX() const
{
    return _x;
}

float CT_VoronoiNodeT::getY() const
{
    return _y;
}

float CT_VoronoiNodeT::getZ() const
{
    return _z;
}
