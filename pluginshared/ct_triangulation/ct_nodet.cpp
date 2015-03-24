#include "ct_nodet.h"
#include "ct_trianglet.h"
#include "ct_voronoinodet.h"

#include <math.h>

CT_NodeT::CT_NodeT(Eigen::Vector3d *point, QObject *associate_object, bool toBeDeleted)
{
    _deletePoint = toBeDeleted;
    _point = point;
    _associate_object = associate_object;
}

CT_NodeT::~CT_NodeT()
{
    if (_deletePoint) {delete[] _point;}
    _point = NULL;
    _associate_object = NULL;
}

void CT_NodeT::clear()
{
    _edge.clear();
}

void CT_NodeT::setEdge(QSharedPointer<CT_EdgeT> edge)
{
    _edge = edge;
}

QSharedPointer<CT_EdgeT> CT_NodeT::getEdge() const
{
    return _edge;
}

Eigen::Vector3d* CT_NodeT::getPoint() const
{
    return _point;
}

double CT_NodeT::distance(double x, double y) const
{
    double dx = x - (*_point)(0);
    double dy = y - (*_point)(1);

    return sqrt(dx*dx + dy*dy);
}

QObject* CT_NodeT::getAssociateObject() const
{
    return _associate_object;
}

QList< QSharedPointer<CT_NodeT> > CT_NodeT::getNodesAround() const
{
    QList< QSharedPointer<CT_NodeT> > list_of_nodes_around;

    if(!_edge.isNull())
    {
        QList< QSharedPointer<CT_TriangleT> > list_triangle_verified;
        QList< QSharedPointer<CT_TriangleT> > list_triangle_contains_node;

        list_triangle_contains_node.append(_edge.data()->getTriangle());

        while(!list_triangle_contains_node.isEmpty())
        {
            QSharedPointer<CT_TriangleT> triangle = list_triangle_contains_node.takeFirst();

            list_triangle_verified.append(triangle);

            QSharedPointer<CT_EdgeT> ed = triangle.data()->getAnEdge();
            if(!list_of_nodes_around.contains(ed.data()->getN1()))
                list_of_nodes_around.append(ed.data()->getN1());

            if(!list_of_nodes_around.contains(ed.data()->getN2()))
                list_of_nodes_around.append(ed.data()->getN2());

            ed = ed.data()->getLeftEdge();

            if(!list_of_nodes_around.contains(ed.data()->getN1()))
                list_of_nodes_around.append(ed.data()->getN1());

            if(!list_of_nodes_around.contains(ed.data()->getN2()))
                list_of_nodes_around.append(ed.data()->getN2());

            ed = ed.data()->getLeftEdge();

            if(!list_of_nodes_around.contains(ed.data()->getN1()))
                list_of_nodes_around.append(ed.data()->getN1());

            if(!list_of_nodes_around.contains(ed.data()->getN2()))
                list_of_nodes_around.append(ed.data()->getN2());

            if(!ed.data()->getInversedEdge().isNull())
            {
                triangle = ed.data()->getInversedEdge().data()->getTriangle();

                if((!triangle.isNull())
                    && (!list_triangle_verified.contains(triangle))
                    && (triangle.data()->contains(_this)))
                {
                    list_triangle_contains_node.append(triangle);
                }
            }

            ed = ed.data()->getLeftEdge();

            if(!ed.data()->getInversedEdge().isNull())
            {
                triangle = ed.data()->getInversedEdge().data()->getTriangle();

                if((!triangle.isNull())
                    && (!list_triangle_verified.contains(triangle))
                    && (triangle.data()->contains(_this)))
                {
                    list_triangle_contains_node.append(triangle);
                }
            }

            ed = ed.data()->getLeftEdge();

            if(!ed.data()->getInversedEdge().isNull())
            {
                triangle = ed.data()->getInversedEdge().data()->getTriangle();

                if((!triangle.isNull())
                    && (!list_triangle_verified.contains(triangle))
                    && (triangle.data()->contains(_this)))
                {
                    list_triangle_contains_node.append(triangle);
                }
            }
        }

        list_of_nodes_around.removeOne(_this);
    }

    return list_of_nodes_around;
}

QList< QSharedPointer<CT_VoronoiNodeT> > CT_NodeT::getVoronoi() const
{
    QList< QSharedPointer<CT_VoronoiNodeT> > list_of_nodes_voronoi;

    if(!_edge.isNull())
    {
        QList< QSharedPointer<CT_TriangleT> > list_triangle_verified;
        QList< QSharedPointer<CT_TriangleT> > list_triangle_contains_node;

        list_triangle_contains_node.append(_edge.data()->getTriangle());

        while(!list_triangle_contains_node.isEmpty())
        {
            QSharedPointer<CT_TriangleT> triangle = list_triangle_contains_node.takeFirst();

            list_triangle_verified.append(triangle);
            list_of_nodes_voronoi.append(CT_VoronoiNodeT::create(triangle.data()->getCx(), triangle.data()->getCy(), triangle.data()->getCz()));

            QSharedPointer<CT_EdgeT> ed = triangle.data()->getAnEdge();

            if(!ed.data()->getInversedEdge().isNull())
            {
                triangle = ed.data()->getInversedEdge().data()->getTriangle();

                if((!triangle.isNull())
                    && (!list_triangle_verified.contains(triangle))
                    && (triangle.data()->contains(_this)))
                {
                    list_triangle_contains_node.append(triangle);
                }
            }

            ed = ed.data()->getLeftEdge();

            if(!ed.data()->getInversedEdge().isNull())
            {
                triangle = ed.data()->getInversedEdge().data()->getTriangle();

                if((!triangle.isNull())
                    && (!list_triangle_verified.contains(triangle))
                    && (triangle.data()->contains(_this)))
                {
                    list_triangle_contains_node.append(triangle);
                }
            }

            ed = ed.data()->getLeftEdge();

            if(!ed.data()->getInversedEdge().isNull())
            {
                triangle = ed.data()->getInversedEdge().data()->getTriangle();

                if((!triangle.isNull())
                    && (!list_triangle_verified.contains(triangle))
                    && (triangle.data()->contains(_this)))
                {
                    list_triangle_contains_node.append(triangle);
                }
            }
        }
    }

    return list_of_nodes_voronoi;
}
