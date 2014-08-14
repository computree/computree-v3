#ifndef CT_MATH2DLINES_H
#define CT_MATH2DLINES_H

#include "qvector2d.h"

#include "pluginShared_global.h"

class PLUGINSHAREDSHARED_EXPORT CT_Math2DLines
{
public:

    static bool intersectSegments(QVector2D &a, QVector2D &b, QVector2D &c, QVector2D &d, float &r, float &s, QVector2D &intersection);

    static void computePerpendicularSegment(QVector2D &a, QVector2D &b, QVector2D &c, QVector2D &d, float length = -1, bool clockwise = false);

};

#endif // CT_MATH2DLINES_H
