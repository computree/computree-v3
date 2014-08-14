#include "ct_edget.h"
#include "ct_nodet.h"
#include "ct_trianglet.h"

void CT_EdgeT::update(QSharedPointer<CT_NodeT> n1, QSharedPointer<CT_NodeT> n2)
{
    _n1 = n1;
    _n2 = n2;

    computeLineEquation();

    setN1Edge();
}

QSharedPointer<CT_NodeT> CT_EdgeT::getN1() const
{
    return _n1;
}

QSharedPointer<CT_NodeT> CT_EdgeT::getN2() const
{
    return _n2;
}

QSharedPointer<CT_EdgeT> CT_EdgeT::getInversedEdge() const
{
    return _inversed_edge;
}

QSharedPointer<CT_EdgeT> CT_EdgeT::getLeftEdge() const
{
    return _next_edge;
}

QSharedPointer<CT_EdgeT> CT_EdgeT::getHullEdge() const
{
    return _next_edge_h;
}

QSharedPointer<CT_EdgeT> CT_EdgeT::getMostLeft() const
{
    QSharedPointer<CT_EdgeT> ee;
    QSharedPointer<CT_EdgeT> e = _this; // on part de cette arete

    while((!(ee = e.data()->getLeftEdge().data()->getLeftEdge().data()->getInversedEdge()).isNull())
            && (ee != _this))
    {
        e = ee;
    }

    return e.data()->getLeftEdge().data()->getLeftEdge();
}

QSharedPointer<CT_EdgeT> CT_EdgeT::getMostRight() const
{
    QSharedPointer<CT_EdgeT> ee;
    QSharedPointer<CT_EdgeT> e = _this; // on part de cette arete

    while(!e.data()->getInversedEdge().isNull()
            && ((ee = e.data()->getInversedEdge().data()->getLeftEdge()) != _this))
    {
        e = ee;
    }

    return e;
}

QSharedPointer<CT_TriangleT> CT_EdgeT::getTriangle() const
{
    return _triangle;
}

void CT_EdgeT::setInversedEdge(QSharedPointer<CT_EdgeT> inversed_edge)
{
    _inversed_edge = inversed_edge;
}

void CT_EdgeT::setLeftEdge(QSharedPointer<CT_EdgeT> next_edge)
{
    _next_edge = next_edge;
}

void CT_EdgeT::setHullEdge(QSharedPointer<CT_EdgeT> next_edge_h)
{
    _next_edge_h = next_edge_h;
}

void CT_EdgeT::setTriangle(QSharedPointer<CT_TriangleT> triangle)
{
    _triangle = triangle;
}

QSharedPointer<CT_EdgeT> CT_EdgeT::makeSymmetry()
{
    QSharedPointer<CT_EdgeT> inversed_edge = CT_EdgeT::create(_n2, _n1);

    linkSymmetry(inversed_edge);

    return inversed_edge;
}

void CT_EdgeT::linkSymmetry(QSharedPointer<CT_EdgeT> inversed_edge)
{
    setInversedEdge(inversed_edge);

    if(!_inversed_edge.isNull())
    {
        _inversed_edge.data()->setInversedEdge(_this);
    }
}

int CT_EdgeT::onSide(QSharedPointer<CT_NodeT> n) const
{
    float *p = n.data()->getPoint();

    double s = _a*(double)p[0] + _b*(double)p[1] + _c;

    if(s>0.0) return 1;
    if(s<0.0) return -1;

    return 0;
}

void CT_EdgeT::setN1Edge()
{
    _n1.data()->setEdge(_this);
}

bool CT_EdgeT::contains(const QSharedPointer<CT_NodeT> &node) const
{
    return ((_n1 == node) || (_n2 == node));
}

//////////////// PRIVATE METHOD //////////////////

CT_EdgeT::CT_EdgeT()
{
}

void CT_EdgeT::computeLineEquation()
{
    float *p1 = _n1.data()->getPoint();
    float *p2 = _n2.data()->getPoint();

    _a = (double)p2[1]-(double)p1[1];
    _b = (double)p1[0]-(double)p2[0];
    _c = ((double)p2[0]*(double)p1[1]) - ((double)p1[0]*(double)p2[1]);
}
