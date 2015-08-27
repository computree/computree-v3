#include "ct_trianglet.h"
#include "ct_nodet.h"

void CT_TriangleT::update(QSharedPointer<CT_EdgeT> e1, QSharedPointer<CT_EdgeT> e2, QSharedPointer<CT_EdgeT> e3)
{
    _an_edge = e1;

    e1.data()->setLeftEdge(e2);
    e2.data()->setLeftEdge(e3);
    e3.data()->setLeftEdge(e1);

    e1.data()->setTriangle(_this);
    e2.data()->setTriangle(_this);
    e3.data()->setTriangle(_this);

    findCircle();
}

QSharedPointer<CT_EdgeT> CT_TriangleT::getAnEdge() const
{
    return _an_edge;
}

void CT_TriangleT::clear()
{
   _an_edge.clear();
}

bool CT_TriangleT::isInCircle(QSharedPointer<CT_NodeT> n) const
{
    return (n.data()->distance(_circle_cx, _circle_cy) < _circle_radius);
}

void CT_TriangleT::removeEdgesIn(QList< QSharedPointer<CT_EdgeT> > &list) const
{
    list.removeOne(_an_edge.data()->getLeftEdge().data()->getLeftEdge());
    list.removeOne(_an_edge.data()->getLeftEdge());
    list.removeOne(_an_edge);
}

double CT_TriangleT::getCx() const
{
    return _circle_cx;
}

double CT_TriangleT::getCy() const
{
    return _circle_cy;
}

double CT_TriangleT::getCz() const
{
    return _circle_cz;
}

double CT_TriangleT::getCRadius() const
{
    return _circle_radius;
}

bool CT_TriangleT::contains(const QSharedPointer<CT_NodeT> &node) const
{
    if(_an_edge.data()->contains(node))
    {
        return true;
    }

    if(_an_edge.data()->getLeftEdge().data()->contains(node))
    {
        return true;
    }

    if(_an_edge.data()->getLeftEdge().data()->getLeftEdge().data()->contains(node))
    {
        return true;
    }

    return false;
}

//////////////// PRIVATE METHOD //////////////////

CT_TriangleT::CT_TriangleT()
{
}

void CT_TriangleT::findCircle()
{
    Eigen::Vector3d *p1 = _an_edge.data()->getN1().data()->getPoint();
    Eigen::Vector3d *p2 = _an_edge.data()->getN2().data()->getPoint();
    Eigen::Vector3d *p3 = _an_edge.data()->getLeftEdge().data()->getN2().data()->getPoint();

    // recuperation des coordonnees des sommets
    double x1= (*p1)(0);
    double y1= (*p1)(1);
    double z1= (*p1)(2);

    double x2= (*p2)(0);
    double y2= (*p2)(1);
    double z2= (*p2)(2);

    double x3= (*p3)(0);
    double y3= (*p3)(1);
    double z3= (*p3)(2);

    // calcul des coefficients du syteme a resoudre
    double a=(y2-y3)*(x2-x1)-(y2-y1)*(x2-x3);
    double a1=(x1+x2)*(x2-x1)+(y2-y1)*(y1+y2);
    double a2=(x2+x3)*(x2-x3)+(y2-y3)*(y2+y3);

    // resolution du systeme -> centre du cercle
    _circle_cx = (a1*(y2-y3)-a2*(y2-y1))/a/2.0;
    _circle_cy =(a2*(x2-x1)-a1*(x2-x3))/a/2.0;

    // rayon du cercle = distance du centre a l'un des sommets du triangle
    _circle_radius = _an_edge.data()->getN1().data()->distance(_circle_cx, _circle_cy);

    double a11 = 2.0*(x2-x1);
    double a12 = 2.0*(y2-y1);
    double a13 = 2.0*(z2-z1);

    double a21 = 2.0*(x3-x1);
    double a22 = 2.0*(y3-y1);
    double a23 = 2.0*(z3-z1);

    double a31 = ((y2-y1)*(z3-z1) - (y3-y1)*(z2-z1));
    double a32 = ((z2-z1)*(x3-x1) - (z3-z1)*(x2-x1));
    double a33 = ((x2-x1)*(y3-y1) - (x3-x1)*(y2-y1));

    long double b1 = x2*x2 + y2*y2 + z2*z2 - (x1*x1 + y1*y1 + z1*z1);
    long double b2 = x3*x3 + y3*y3 + z3*z3 - (x1*x1 + y1*y1 + z1*z1);
    long double b3 = x1 * ((y2-y1)*(z3-z1) - (y3-y1)*(z2-z1)) + y1 * ((z2-z1)*(x3-x1) - (z3-z1)*(x2-x1)) + z1 * ((x2-x1)*(y3-y1) - (x3-x1)*(y2-y1));

    long double detg = a11*a22*a33 + a21*a32*a13 + a12*a23*a31 - a31*a22*a13 - a32*a23*a11 - a21*a12*a33;

    long double detb3 = a11*a22*b3 + a21*a32*b1 + a12*b2*a31 - a31*a22*b1 - a32*b2*a11 - a21*a12*b3;

    _circle_cz = detb3/detg;
}
