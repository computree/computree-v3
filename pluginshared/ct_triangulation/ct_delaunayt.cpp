#include "ct_delaunayt.h"
#include "ct_nodet.h"
#include "ct_edget.h"
#include "ct_trianglet.h"

CT_DelaunayT::CT_DelaunayT()
{
}

void CT_DelaunayT::clear()
{
    _list_edges.clear();
    _list_nodes.clear();
    _list_triangles.clear();

    _hull_edge_start = QSharedPointer<CT_EdgeT>();
}

void CT_DelaunayT::insertNode(QSharedPointer<CT_NodeT> n, bool debug)
{
    Q_UNUSED(debug)

    int index_added = _list_nodes.size();

    _list_nodes.append(n);

    // si on a moins de trois neouds ->on ne fait rien
    if((index_added+1) < 3)
    {
        return;
    }

    // si on a trois noeud disponibles on construit le premier triangle
    if((index_added+1) == 3)
    {
        // on prend les trois sommets
        QSharedPointer<CT_NodeT> n1 = _list_nodes.at(0);
        QSharedPointer<CT_NodeT> n2 = _list_nodes.at(1);
        QSharedPointer<CT_NodeT> n3 = n;

        // on cree les trois aretes du triangle
        QSharedPointer<CT_EdgeT> e1 = CT_EdgeT::create(n1 ,n2);

        int side = e1.data()->onSide(n3);

        // si le point est sur la ligne de l'arete, on ne l'ajoute pas car on ne peut pas faire de vrai triangle
        if(side == 0)
        {
            _list_nodes.removeAt(index_added);

            return;
        }

        // si le point est du cote droit de l'arete
        if(side == -1)
        {
            // on inverse les points
            n1 = n2;
            n2 = _list_nodes.at(0);

            e1.data()->update(n1,n2);
        }

        QSharedPointer<CT_EdgeT> e2 = CT_EdgeT::create(n2,n3);
        QSharedPointer<CT_EdgeT> e3 = CT_EdgeT::create(n3,n1);

        // mise a jour des aretes voisine (cycle du triangle)
        e1.data()->setHullEdge(e2);
        e2.data()->setHullEdge(e3);
        e3.data()->setHullEdge(e1);

        // initialisation de l'enveloppe convexe sur la premiere arete
        _hull_edge_start = e1;

        // creation du premier triangle
        _list_triangles.append(CT_TriangleT::create(_list_edges, e1, e2, e3));

        return;
    }

    //  premiere arete de la liste
    _act_edge = _list_edges.first();

    int eid;

    // si le point est du cote droit de la premiere arete
    if(_act_edge.data()->onSide(n) == -1)
    {
        if(_act_edge.data()->getInversedEdge().isNull())
        {
            eid = -1;
        }
        else
        {
            eid = searchEdge(_act_edge.data()->getInversedEdge(), n);
        }
    }
    else
    {
        eid = searchEdge(_act_edge, n);
    }

    if(eid == 0)
    {
        _list_nodes.removeAt(index_added); return;
    }

    // si le nouveau point est a l'interieur du triangle ou sur une des aretes du triangle
    if(eid > 0)
    {
        expandTri(_act_edge, n, eid);
    }

    // sinon il est en dehors de l'enveloppe convexe
    else
    {
        expandHull(n);
    }
}

void CT_DelaunayT::removeNode(QSharedPointer<CT_NodeT> nd)
{
    if(_list_nodes.size() <= 3)
    {
        clear();

        return;
    }

    QSharedPointer<CT_EdgeT> e, ee, start;

    start = e = nd.data()->getEdge().data()->getMostRight();

    QList< QSharedPointer<CT_NodeT> > list_nodes_around = nd.data()->getNodesAround();

    _list_nodes.removeOne(nd);

    int nodetype = 0;
    int idegree = -1;

    QList< QSharedPointer<CT_EdgeT> > index;

    // suppression des aretes et triangles lies a la node
    while(nodetype == 0)
    {
        _list_edges.removeOne(ee = e.data()->getLeftEdge());

        index.append(ee);
        ++idegree;

        ee.data()->setN1Edge();

        _list_triangles.removeOne(e.data()->getTriangle());   // delete triangles involved

        _list_edges.removeOne(e);

        _list_edges.removeOne(ee.data()->getLeftEdge());

        e = ee.data()->getLeftEdge().data()->getInversedEdge();            // next left edge

        if(e.isNull()) nodetype = 2;         // nd on convex hull
        if(e == start) nodetype = 1;        // inner node
    }

    // generate new triangles and add to triangulation
    int cur_i = 0, cur_n = 0;
    int last_n = idegree;

    QSharedPointer<CT_EdgeT> e1, e2, e3;

    while(last_n >= 1)
    {
        e1 = index.at(cur_i);
        e2 = index.at(cur_i+1);

        // si on a que 3 aretes
        if(last_n == 2
           && nodetype == 1)
        {
            _list_triangles.append(CT_TriangleT::create(_list_edges, e1, e2, index.at(2)));

            swapTest(e1);
            swapTest(e2);
            swapTest(index.at(2));

            break;
        }

        // si on a que deux arete
        if(last_n == 1
           && nodetype == 1)
        {
            index.first().data()->getInversedEdge().data()->linkSymmetry(index[1].data()->getInversedEdge());
            index.first().data()->getInversedEdge().data()->setN1Edge();
            index.at(1).data()->getInversedEdge().data()->setN1Edge();

            swapTest(index.first().data()->getInversedEdge());

            break;
        }

        if(e1.data()->onSide(e2.data()->getN2()) == 1)  // left side
        {
            e3 = CT_EdgeT::create(e2.data()->getN2(), e1.data()->getN1());

            bool continue_search = true;

            QListIterator< QSharedPointer<CT_NodeT> > it(_list_nodes);

            while(continue_search && it.hasNext())
            {
                QSharedPointer<CT_NodeT> node = it.next();

                if((e1.data()->getN1() != node)
                    && (e2.data()->getN1() != node)
                    && (e3.data()->getN1() != node))
                {
                    continue_search = !nodeInTriangleTest(e1, e2, e3, node);
                }
            }

            if(!continue_search)
            {
                index.replace(cur_n++, index.at(cur_i++));
            }
            else
            {
                cur_i += 2;

                index.replace(cur_n++, e3.data()->makeSymmetry());

                _list_triangles.append(CT_TriangleT::create(_list_edges, e1, e2, e3));

                swapTest(e1);
                swapTest(e2);
            }
        }
        else
        {
            index.replace(cur_n++, index.at(cur_i++));
        }

        if(cur_i == last_n)
        {
            index.replace(cur_n++, index.at(cur_i++));
        }

        if(cur_i == (last_n+1))
        {
            if(last_n == (cur_n-1)) break;

            last_n = cur_n-1;

            cur_i = cur_n = 0;
        }
    }

    if(nodetype == 2)   // reconstruct the convex hull
    {
        index.at(last_n).data()->getInversedEdge().data()->getMostLeft().data()->setHullEdge(_hull_edge_start = index.at(last_n).data()->getInversedEdge());

        for(int i=last_n; i>0; --i)
        {
            index.at(i).data()->getInversedEdge().data()->setHullEdge(index.at(i-1).data()->getInversedEdge());
            index.at(i).data()->getInversedEdge().data()->setInversedEdge(QSharedPointer<CT_EdgeT>(NULL));
        }

        index.first().data()->getInversedEdge().data()->setHullEdge(start.data()->getHullEdge());
        index.first().data()->getInversedEdge().data()->setInversedEdge(QSharedPointer<CT_EdgeT>(NULL));

        QListIterator< QSharedPointer<CT_NodeT> > it(list_nodes_around);

        while(it.hasNext())
        {
            QSharedPointer<CT_NodeT> node_to_test = it.next();

            if(!_list_edges.contains(node_to_test.data()->getEdge()))
            {
                bool continue_search = true;

                QListIterator< QSharedPointer<CT_EdgeT> > it_edge(_list_edges);

                while(continue_search
                      && it_edge.hasNext())
                {
                    QSharedPointer<CT_EdgeT> edge_to_test = it_edge.next();

                    if(edge_to_test.data()->getInversedEdge().isNull()
                        && edge_to_test.data()->getN1() == node_to_test)
                    {
                        node_to_test.data()->setEdge(edge_to_test);
                        continue_search = false;
                    }
                }
            }
        }
    }
}

const QList< QSharedPointer<CT_NodeT> >& CT_DelaunayT::getNodeList() const
{
    return _list_nodes;
}

const QList< QSharedPointer<CT_EdgeT> >& CT_DelaunayT::getEdgeList() const
{
    return _list_edges;
}

const QList< QSharedPointer<CT_TriangleT> >& CT_DelaunayT::getTriangleList() const
{
    return _list_triangles;
}

QSharedPointer<CT_EdgeT> CT_DelaunayT::getHullEdgeStart() const
{
    return _hull_edge_start;
}

QList< QSharedPointer<CT_NodeT> > CT_DelaunayT::getHullNodes() const
{
    QList< QSharedPointer<CT_NodeT> > list;

    if(!_hull_edge_start.isNull())
    {
        QSharedPointer<CT_EdgeT> next_edge = _hull_edge_start;

        do
        {
            if(!list.contains(next_edge.data()->getN1()))
                list.append(next_edge.data()->getN1());

            if(!list.contains(next_edge.data()->getN2()))
                list.append(next_edge.data()->getN2());

            next_edge = next_edge.data()->getHullEdge();

        }while(_hull_edge_start != next_edge);
    }

    return list;
}

CT_DelaunayT* CT_DelaunayT::copy()
{
    CT_DelaunayT *d = new CT_DelaunayT();
    d->_list_nodes.append(_list_nodes);
    d->_list_edges.append(_list_edges);
    d->_list_triangles.append(_list_triangles);
    d->_hull_edge_start = _hull_edge_start;
    d->_act_edge = _act_edge;

    return d;
}

void CT_DelaunayT::expandTri(QSharedPointer<CT_EdgeT> e, QSharedPointer<CT_NodeT> nd, int type)
{
    QSharedPointer<CT_EdgeT> e1 = e;
    QSharedPointer<CT_EdgeT> e2 = e1.data()->getLeftEdge();
    QSharedPointer<CT_EdgeT> e3 = e2.data()->getLeftEdge();
    QSharedPointer<CT_NodeT> n1 = e1.data()->getN1();
    QSharedPointer<CT_NodeT> n2 = e2.data()->getN1();
    QSharedPointer<CT_NodeT> n3 = e3.data()->getN1();

    // si le nouveau point est a l'interieur du triangle
    if(type == 2)
    {
        QSharedPointer<CT_EdgeT> e10 = CT_EdgeT::create(n1, nd);
        QSharedPointer<CT_EdgeT> e20 = CT_EdgeT::create(n2, nd);
        QSharedPointer<CT_EdgeT> e30 = CT_EdgeT::create(n3, nd);

        e.data()->getTriangle().data()->removeEdgesIn(_list_edges);

        _list_triangles.removeOne(e.data()->getTriangle());
        _list_triangles.append(CT_TriangleT::create(_list_edges, e1, e20, e10.data()->makeSymmetry()));
        _list_triangles.append(CT_TriangleT::create(_list_edges, e2, e30, e20.data()->makeSymmetry()));
        _list_triangles.append(CT_TriangleT::create(_list_edges, e3, e10, e30.data()->makeSymmetry()));

        // swap test pour les trois nouveaux triangles
        swapTest(e1);
        swapTest(e2);
        swapTest(e3);
    }
    // le nouveau point est sur l'arete nd
    else
    {
        QSharedPointer<CT_EdgeT> e4 = e1.data()->getInversedEdge();

        if(e4.isNull()
            || e4.data()->getTriangle().isNull())           // one triangle involved
        {
            QSharedPointer<CT_EdgeT> e30 = CT_EdgeT::create(n3, nd);
            QSharedPointer<CT_EdgeT> e02 = CT_EdgeT::create(nd, n2);
            QSharedPointer<CT_EdgeT> e10 = CT_EdgeT::create(n1, nd);
            QSharedPointer<CT_EdgeT> e03 = e30.data()->makeSymmetry();

            e10.data()->setN1Edge();
            e1.data()->getMostLeft().data()->setHullEdge(e10);
            e10.data()->setHullEdge(e02);
            e02.data()->setHullEdge(e1.data()->getHullEdge());

            _hull_edge_start = e02;

            _list_triangles.removeOne(e1.data()->getTriangle());                   // remove oldtriangle               // add two new triangles

            _list_edges.removeOne(e1);
            _list_edges.append(e10);
            _list_edges.append(e02);
            _list_edges.append(e30);
            _list_edges.append(e03);

            _list_triangles.append(CT_TriangleT::create(e2, e30, e02));
            _list_triangles.append(CT_TriangleT::create(e3, e10, e03));

            swapTest(e2);   // swap test for the two new triangles
            swapTest(e3);
            swapTest(e30);
        }
        else         // two triangle involved
        {
            QSharedPointer<CT_EdgeT> e5 = e4.data()->getLeftEdge();
            QSharedPointer<CT_EdgeT> e6 = e5.data()->getLeftEdge();
            QSharedPointer<CT_NodeT> n4 = e6.data()->getN1();
            QSharedPointer<CT_EdgeT> e10 = CT_EdgeT::create(n1, nd);
            QSharedPointer<CT_EdgeT> e20 = CT_EdgeT::create(n2 ,nd);
            QSharedPointer<CT_EdgeT> e30 = CT_EdgeT::create(n3, nd);
            QSharedPointer<CT_EdgeT> e40 = CT_EdgeT::create(n4, nd);

            _list_triangles.removeOne(e.data()->getTriangle());                   // remove oldtriangle

            e.data()->getTriangle().data()->removeEdgesIn(_list_edges);

            _list_triangles.removeOne(e4.data()->getTriangle());               // remove old triangle

            e4.data()->getTriangle().data()->removeEdgesIn(_list_edges);

            e5.data()->setN1Edge();   // because e, e4 removed, reset edge index of node n1 and n2
            e2.data()->setN1Edge();

            _list_triangles.append(CT_TriangleT::create(_list_edges, e2, e30, e20.data()->makeSymmetry()));
            _list_triangles.append(CT_TriangleT::create(_list_edges, e3, e10, e30.data()->makeSymmetry()));
            _list_triangles.append(CT_TriangleT::create(_list_edges, e5, e40, e10.data()->makeSymmetry()));
            _list_triangles.append(CT_TriangleT::create(_list_edges, e6, e20, e40.data()->makeSymmetry()));

            swapTest(e2);   // swap test for the three new triangles
            swapTest(e3);
            swapTest(e5);
            swapTest(e6);
            swapTest(e10);
            swapTest(e20);
            swapTest(e30);
            swapTest(e40);
        }
    }
}

void CT_DelaunayT::expandHull(QSharedPointer<CT_NodeT> nd)
{
    QSharedPointer<CT_EdgeT> e1, e2, e3(NULL), enext;
    QSharedPointer<CT_EdgeT> e = _hull_edge_start;
    QSharedPointer<CT_EdgeT> comedge(NULL), lastbe(NULL);

    while(true)
    {
        enext = e.data()->getHullEdge();

        if(e.data()->onSide(nd) == -1)   // right side
        {
            if(!lastbe.isNull())
            {
                e1 = e.data()->makeSymmetry();
                e2 = CT_EdgeT::create(e.data()->getN1(), nd);
                e3 = CT_EdgeT::create(nd, e.data()->getN2());

                if(comedge.isNull())
                {
                    _hull_edge_start = lastbe;
                    lastbe.data()->setHullEdge(e2);
                    lastbe = e2;
                }
                else
                {
                    comedge.data()->linkSymmetry(e2);
                }

                comedge = e3;

                _list_triangles.append(CT_TriangleT::create(_list_edges, e1, e2, e3));

                swapTest(e);
            }
        }
        else
        {
            if(!comedge.isNull()) break;
            lastbe = e;
        }

        e = enext;
    }

    lastbe.data()->setHullEdge(e3);
    e3.data()->setHullEdge(e);
}

// recherche d'une arete
int CT_DelaunayT::searchEdge(QSharedPointer<CT_EdgeT> e, QSharedPointer<CT_NodeT> nd)
{
    int f2,f3,f4;
    QSharedPointer<CT_EdgeT> e0(NULL);

    f2 = e.data()->getLeftEdge().data()->onSide(nd);
    if(f2 == -1)
    {
        if(!e.data()->getLeftEdge().data()->getInversedEdge().isNull())
        {
            return searchEdge(e.data()->getLeftEdge().data()->getInversedEdge(), nd);
        }
        else
        {
            _act_edge = e;

            return -1;
        }
    }

    if(f2 == 0)
    {
        e0 = e.data()->getLeftEdge();
    }

    QSharedPointer<CT_EdgeT> ee = e.data()->getLeftEdge();

    f3 = ee.data()->getLeftEdge().data()->onSide(nd);
    if(f3 == -1)
    {
        if(!ee.data()->getLeftEdge().data()->getInversedEdge().isNull())
        {
            return searchEdge(ee.data()->getLeftEdge().data()->getInversedEdge(), nd);
        }
        else
        {
            _act_edge = ee.data()->getLeftEdge();

            return -1;
        }
    }

    if(f3 == 0)
    {
        e0 = ee.data()->getLeftEdge();
    }

    f4 = e.data()->onSide(nd);
    if(f4 == 0)
    {
        e0 = e;
    }

    if(!e0.isNull())
    {
        _act_edge = e0;

        if(e0.data()->getLeftEdge().data()->onSide(nd) == 0)
        {
            _act_edge = e0.data()->getLeftEdge();
            return 0;
        }

        if(e0.data()->getLeftEdge().data()->getLeftEdge().data()->onSide(nd) == 0)
        {
            return 0;
        }

        return 1;
    }

    _act_edge = ee;

    return 2;
}

void CT_DelaunayT::swapTest(QSharedPointer<CT_EdgeT> e11)
{
    QSharedPointer<CT_EdgeT> e21 = e11.data()->getInversedEdge();

    if(e21.isNull()
        || e21.data()->getTriangle().isNull())
    {
        return;
    }

    QSharedPointer<CT_EdgeT> e12 = e11.data()->getLeftEdge();
    QSharedPointer<CT_EdgeT> e13 = e12.data()->getLeftEdge();
    QSharedPointer<CT_EdgeT> e22 = e21.data()->getLeftEdge();
    QSharedPointer<CT_EdgeT> e23 = e22.data()->getLeftEdge();

    if(e11.data()->getTriangle().data()->isInCircle(e22.data()->getN2())
        || e21.data()->getTriangle().data()->isInCircle(e12.data()->getN2()))
    {
        e11.data()->update(e22.data()->getN2(), e12.data()->getN2());
        e21.data()->update(e12.data()->getN2(), e22.data()->getN2());

        e11.data()->linkSymmetry(e21);
        e13.data()->getTriangle().data()->update(e13, e22, e11);
        e23.data()->getTriangle().data()->update(e23, e12, e21);

        e12.data()->setN1Edge();
        e22.data()->setN1Edge();

        swapTest(e12);
        swapTest(e22);
        swapTest(e13);
        swapTest(e23);
    }
}

bool CT_DelaunayT::nodeInTriangleTest(QSharedPointer<CT_EdgeT> e1, QSharedPointer<CT_EdgeT> e2, QSharedPointer<CT_EdgeT> e3, QSharedPointer<CT_NodeT> n)
{
    return ((e1.data()->onSide(n) >= 0) && (e2.data()->onSide(n) >= 0) && (e3.data()->onSide(n) >= 0));
}

