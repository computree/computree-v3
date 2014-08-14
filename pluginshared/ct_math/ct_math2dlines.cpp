#include "ct_math2dlines.h"
#include "ct_math/ct_mathpoint.h"

bool CT_Math2DLines::intersectSegments(QVector2D &a, QVector2D &b, QVector2D &c, QVector2D &d, float &r, float &s, QVector2D &intersection)
{
    float denom = (b.x()-a.x())*(d.y()-c.y()) - (b.y()-a.y())*(d.x()-c.x());
    if (denom==0) {return false;} // droites parallèles ou coincidentes

    r = ((a.y()-c.y())*(d.x()-c.x()) - (a.x()-c.x())*(d.y()-c.y())) / denom; // position sur le segment AB (inclus si entre 0 et 1)
    s = ((a.y()-c.y())*(b.x()-a.x()) - (a.x()-c.x())*(b.y()-a.y())) / denom; // position sur le segment CD (inclus si entre 0 et 1)

    intersection.setX(a.x() + r*(b.x()-a.x()));
    intersection.setY(a.y() + r*(b.y()-a.y()));
    return true;
}


void CT_Math2DLines::computePerpendicularSegment(QVector2D &a, QVector2D &b, QVector2D &c, QVector2D &d, float length, bool clockwise)
{
    float k = 1;
    if (length >0)
    {
        k = length/CT_MathPoint::distance2D(a, b);
    }

    if (clockwise)
    {
        d.setX(c.x() + k*(a.y() - b.y()));
        d.setY(c.y() + k*(b.x() - a.x()));
    } else {
        d.setX(c.x() + k*(b.y() - a.y()));
        d.setY(c.y() + k*(a.x() - b.x()));
    }
}
