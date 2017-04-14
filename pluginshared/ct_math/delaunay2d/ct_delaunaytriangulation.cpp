/****************************************************************************

 Copyright (C) 2010-2015 the Office National des Forêts (ONF), France
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre Piboule (ONF)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "ct_delaunaytriangulation.h"

#include "ct_delaunaysidelist.h"
#include "ct_delaunaytrianglesrecycler.h"


#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <QTime>

CT_DelaunayTriangulation::CT_DelaunayTriangulation()
{
    _corners.resize(4);
    _corners[0] = NULL;
    _corners[1] = NULL;
    _corners[2] = NULL;
    _corners[3] = NULL;


    _refTriangle = NULL;

    _initialized = false;
    _neighborsComputed = false;
    _voronoiDiagramComputed = false;

    _minx = std::numeric_limits<double>::max();
    _miny = std::numeric_limits<double>::max();
    _maxx = -std::numeric_limits<double>::max();
    _maxy = -std::numeric_limits<double>::max();

    std::srand(time(NULL));
}

CT_DelaunayTriangulation::~CT_DelaunayTriangulation()
{
    clear(true);
}

void CT_DelaunayTriangulation::clear(bool clearToInsert)
{
    _initialized = false;

    if (_corners[0] != NULL) {delete _corners[0];_corners[0] = NULL;}
    if (_corners[1] != NULL) {delete _corners[1];_corners[1] = NULL;}
    if (_corners[2] != NULL) {delete _corners[2];_corners[2] = NULL;}
    if (_corners[3] != NULL) {delete _corners[3];_corners[3] = NULL;}

    qDeleteAll(_duplicatePositions.keys());
    _duplicatePositions.clear();

    if (clearToInsert)
    {
        qDeleteAll(_toInsert);
        _toInsert.clear();
    }

    qDeleteAll(_insertedVertices);
    _insertedVertices.clear();

    qDeleteAll(_outOfBoundsVertices);
    _outOfBoundsVertices.clear();

    getTriangles();
    if (!_triangles.isEmpty())
    {
        qDeleteAll(_triangles);
        _triangles.clear();
    }
}

void CT_DelaunayTriangulation::init(double x1, double y1, double x2, double y2)
{
    clear(!_initialized);

    if (!_initialized)
    {
        _initialized = true;
        _minx = x1;
        _miny = y1;
        _maxx = x2;
        _maxy = y2;
    }

    // add the 4 corners
    _corners[0] = new CT_DelaunayVertex (new Eigen::Vector3d(x1, y1, 0), true);
    _corners[1] = new CT_DelaunayVertex (new Eigen::Vector3d(x2, y1, 0), true);
    _corners[2] = new CT_DelaunayVertex (new Eigen::Vector3d(x1, y2, 0), true);
    _corners[3] = new CT_DelaunayVertex (new Eigen::Vector3d(x2, y2, 0), true);

    CT_DelaunayTriangle* t1 = new CT_DelaunayTriangle (_corners[0], _corners[2], _corners[3]);
    _triangles.append(t1);

    CT_DelaunayTriangle* t2 = new CT_DelaunayTriangle (_corners[0], _corners[1], _corners[3]);
    _triangles.append(t2);

    t1->_n31 = t2;
    t2->_n31 = t1;

    _refTriangle = t1;
}

bool CT_DelaunayTriangulation::init()
{
    if (_toInsert.size () < 2) {return false;}

    for (int i = 0 ; i < _toInsert.size() ; i++)
    {
        CT_DelaunayVertex* vt = _toInsert.at(i);

        if (vt->x() < _minx) {_minx = vt->x();}
        if (vt->x() > _maxx) {_maxx = vt->x();}
        if (vt->y() < _miny) {_miny = vt->y();}
        if (vt->y() > _maxy) {_maxy = vt->y();}
    }

    _initialized = true;

    // corner should not be at the same position as an inserted vertex
    // so we add add a little (1%) offset
    double offset = (_maxx - _minx + _maxy - _miny) / 200.0;

    _minx = _minx - offset;
    _miny = _miny - offset;
    _maxx = _maxx + offset;
    _maxy = _maxy + offset;

    init (_minx, _miny, _maxx, _maxy);

    return true;
}

const QList<CT_DelaunayTriangle *> &CT_DelaunayTriangulation::getTriangles()
{
//    if (_triangles.isEmpty()) {computeTriangles();}
    return _triangles;
}

const QList<CT_DelaunayVertex *> &CT_DelaunayTriangulation::getVerticesNeighbors()
{
    if (!_neighborsComputed) {
        return computeVerticesNeighbors ();
    } else {
        return _insertedVertices;
    }
}

const QList<CT_DelaunayVertex *> &CT_DelaunayTriangulation::getVoronoiDiagram()
{
    if (!_voronoiDiagramComputed) {
        return computeVoronoiDiagram ();
    } else {
        return _insertedVertices;
    }
}

const QList<CT_DelaunayVertex *> &CT_DelaunayTriangulation::getCleanVoronoiDiagram()
{
    getVoronoiDiagram ();

    for (int i = 0 ; i < _insertedVertices.size () ; i++)
    {
        CT_DelaunayVertex* v = _insertedVertices.at(i);

        for (int j = 0 ; j < v->getVoroVertices().size() ; j++)
        {
            CT_DelaunayVoroVertex* vv = v->getVoroVertices().at(j);
            if ((vv->x() < _minx) || (vv->x() > _maxx) || (vv->y() < _miny) || (vv->y() > _maxy))
            {
                v->initVoroVertices();
            }
        }
    }
    _voronoiDiagramComputed = false;
    return _insertedVertices;
}

const QList<CT_DelaunayOutline*> &CT_DelaunayTriangulation::getOutlines()
{
    if (_outlines.isEmpty()) {
        return computeOutlines();
    } else {
        return _outlines;
    }
}

QMultiMap<CT_DelaunayVertex *, CT_DelaunayVertex *> CT_DelaunayTriangulation::getEdges()
{
    QMultiMap<CT_DelaunayVertex *, CT_DelaunayVertex *> map;

    computeVerticesNeighbors();

    for (int i = 0 ; i < _insertedVertices.size() ; i++)
    {
        CT_DelaunayVertex *baseVertex = _insertedVertices.at(i);
        QList<CT_DelaunayVertex*> &neighbours = baseVertex->getNeighbors();

        for (int j = 0 ; j < neighbours.size() ; j++)
        {
            CT_DelaunayVertex *neighbour = neighbours.at(j);

            if (!map.contains(baseVertex, neighbour) && !map.contains(neighbour, baseVertex))
            {
                map.insert(baseVertex, neighbour);
            }
        }
    }
    return map;
}

void CT_DelaunayTriangulation::computeTriangles()
{

//    CT_DelaunayTriangle* tri = _refTriangle;
//    CT_DelaunayTriangle* n1 = NULL;

//    if (tri != NULL) {_triangles.append(tri);}

//    for (int i = 0 ; i < _triangles.size() ; i++)
//    {
//        tri = _triangles.at(i);

//        n1 = tri->_n12;
//        if ((n1 != NULL) && (!_triangles.contains(n1))) {_triangles.append(n1);}

//        n1 = tri->_n23;
//        if ((n1 != NULL) && (!_triangles.contains(n1))) {_triangles.append(n1);}

//        n1 = tri->_n31;
//        if ((n1 != NULL) && (!_triangles.contains(n1))) {_triangles.append(n1);}

//    }

}

const QList<CT_DelaunayVertex *> &CT_DelaunayTriangulation::computeVerticesNeighbors()
{

    const QList<CT_DelaunayTriangle *> &lst = getTriangles ();
    CT_DelaunayTriangle* tri;

    _neighborsComputed = true;

    // clear old neighbors
    for (int i = 0 ; i < _insertedVertices.size() ; i++)
    {
        _insertedVertices.at(i)->initNeighbors();
    }

    _corners[0]->initNeighbors();
    _corners[1]->initNeighbors();
    _corners[2]->initNeighbors();
    _corners[3]->initNeighbors();

    // compute new neighbors
    for (int i = 0 ; i < lst.size() ; i++)
    {
        tri = lst.at(i);

        tri->_v1->addNeighbor(tri->_v2);
        tri->_v2->addNeighbor(tri->_v1);

        tri->_v2->addNeighbor(tri->_v3);
        tri->_v3->addNeighbor(tri->_v2);

        tri->_v1->addNeighbor(tri->_v3);
        tri->_v3->addNeighbor(tri->_v1);
    }

    // corner are not treated as valid neighbors for insertedVertices
    for (int i = 0 ; i < 4 ; i++)
    {
        QList<CT_DelaunayVertex*> &n = _corners.at(i)->getNeighbors();

        for (int j = 0 ; j < n.size() ; j++)
        {
            n.at(j)->getNeighbors().removeOne(_corners.at(i));
        }
    }

    return _insertedVertices; // corners not exported here
}

const QList<CT_DelaunayVertex *> &CT_DelaunayTriangulation::computeVoronoiDiagram()
{


    const QList<CT_DelaunayTriangle *> &lst = getTriangles();
    CT_DelaunayTriangle* tri;
    CT_DelaunayVertex* vert;
    CT_DelaunayVertex* first;
    CT_DelaunayVertex* next;

    _voronoiDiagramComputed = true;

    // clear old sides
    for (int i = 0 ; i < _insertedVertices.size() ; i++)
    {
        _insertedVertices.at(i)->initVoroVertices();
    }

    _corners[0]->initVoroVertices();
    _corners[1]->initVoroVertices();
    _corners[2]->initVoroVertices();
    _corners[3]->initVoroVertices();


    // compute new sides
    for (int i = 0 ; i < lst.size() ; i++)
    {

        tri = lst.at(i);

        tri->_v1->addVoroVertex (new CT_DelaunayVoroVertex(tri, tri->_v2, tri->_v3));
        tri->_v2->addVoroVertex (new CT_DelaunayVoroVertex(tri, tri->_v1, tri->_v3));
        tri->_v3->addVoroVertex (new CT_DelaunayVoroVertex(tri, tri->_v1, tri->_v2));
    }



    for (int i = 0 ; i < _insertedVertices.size () ; i++)
    {
        vert = _insertedVertices.at(i);

        QList<CT_DelaunayVoroVertex*> &voroList = (QList<CT_DelaunayVoroVertex*> &) vert->getVoroVertices();

        CT_DelaunayVoroVertex* voro = voroList.takeAt(0);

        vert->initVoroVertices();

        vert->addVoroVertex(voro);

        first = voro->_v1;
        next = voro->next(voro->_v1);

        while (next != first) {

            for (int j = 0 ; j < voroList.size() ; j++)
            {
                voro = voroList.at(j);
                if ((voro->_v1 == next) || (voro->_v2 == next)) {break;}
            }

            vert->addVoroVertex(voro);
            voroList.removeOne(voro);

            next = voro->next (next);
        }
    }

    return _insertedVertices;
}

const QList<CT_DelaunayOutline *> &CT_DelaunayTriangulation::computeOutlines()
{
    qDeleteAll(_outlines);
    _outlines.clear();

    const QList<CT_DelaunayTriangle *> &lst = getTriangles();
    CT_DelaunaySideList sides;
    CT_DelaunayTriangle* tri;
    bool okV1, okV2, okV3;
    CT_DelaunayOutline* outline;

    for (int i = 0 ; i < lst.size() ; i++)
    {
        tri = lst.at(i);

        okV1 = true;
        okV2 = true;
        okV3 = true;

        for (int j = 0 ; j < 4 ; j++)
        {
            if (tri->_v1 == _corners[j]) {okV1 = false;}
            if (tri->_v2 == _corners[j]) {okV2 = false;}
            if (tri->_v3 == _corners[j]) {okV3 = false;}
        }

        if (okV1 && okV2 && !okV3) {sides.appendSide(tri, tri->_v1, tri->_v2);}
        if (okV2 && okV3 && !okV1) {sides.appendSide(tri, tri->_v2, tri->_v3);}
        if (okV3 && okV1 && !okV2) {sides.appendSide(tri, tri->_v3, tri->_v1);}
    }

    if (_insertedVertices.size () > 2)
    {
        for (int i = 0 ; i < sides.size() ; i++)
        {
            for (int j = i + 1 ; j < sides.size() ; j++)
            {
                if (sides.sidesEquals(i, j))
                {
                    sides.removeSide(j);
                    sides.removeSide(i);
                    break;
                }
            }
        }

        while (sides.size () > 2)
        {
            outline = new CT_DelaunayOutline ();

            int sdIndex = 0;
            sides.removeSide(sdIndex, true);

            CT_DelaunayVertex* sd_v1 = sides.getRemovedV1();
            CT_DelaunayVertex* sd_v2 = sides.getRemovedV2();

            CT_DelaunayVertex* first = sd_v1;
            CT_DelaunayVertex* next = sd_v2;

            while (next != first)
            {
                for (int j = 0 ; j < sides.size() ; j++)
                {
                    sdIndex = j;
                    sd_v1 = sides.getV1(sdIndex);
                    sd_v2 = sides.getV2(sdIndex);

                    if ((sd_v1 == next) || (sd_v2 == next)) {break;}
                }

                outline->addVertex(next);
                sides.removeSide(sdIndex);

                next = sides.swap(sdIndex, next);
            }

            outline->addVertex (next);
            _outlines.append(outline);

        }

        sides.clear();
    }

    return _outlines;
}

CT_DelaunayVertex *CT_DelaunayTriangulation::addVertex(Eigen::Vector3d *data, bool deleteData)
{
    CT_DelaunayVertex *vt  = new CT_DelaunayVertex(data, deleteData);

// Duplicates checks have been moved to doInsertion, for process time reasons
//    for (int i = 0 ; i < _insertedVertices.size() ; i++)
//    {
//        if (vt->equals(_insertedVertices.at(i)))
//        {
//            _duplicatePositions.insert(vt, _insertedVertices.at(i));
//            return vt;
//        }
//    }


//    for (int i = 0 ; i < _toInsert.size () ; i++)
//    {
//        if (vt->equals(_toInsert.at(i)))
//        {
//            _duplicatePositions.insert(vt, _toInsert.at(i));
//            return vt;
//        }
//    }
    _toInsert.append(vt);

    return vt;
}


bool CT_DelaunayTriangulation::doInsertion()
{
    if (!_initialized) {return false;} // we need 4 corners to continue


    CT_DelaunayTriangle* t1;
    CT_DelaunayTriangle* t1old;
    CT_DelaunayTriangle* t2;
    CT_DelaunayTriangle* n1;
    CT_DelaunayVertex* vt;
    CT_DelaunayVertex* vBase;
    CT_DelaunayVertex* vNext;
    CT_DelaunayVertex* vFirst;

    double xt;
    double yt;
    int j;

    // we use arrayList and not vector for performance reasons
    QList<CT_DelaunayTriangle*> destrLst;
    CT_DelaunaySideList borderLst;
    CT_DelaunayTriangleRecycler tRecycler;

    // initialisations
    t1 = _refTriangle;
    t2 = NULL;

    QTime time;


    time.start();

    // descending order to avoid multiples Time consuming ArrayList compression
    // so we always remove the last element
    while (!_toInsert.isEmpty())
    {
        vt = _toInsert.takeLast();

        // is the point in the triangulation, else add to outOfBoundsVertices list
        // and DO NOT insert
        if ((vt->x() <= _minx) || (vt->x() >= _maxx) || (vt->y() <= _miny) || (vt->y() >= _maxy))
        {
            _outOfBoundsVertices.append(vt);

            // we have to insert the point in the triangulation
        } else {
            xt = vt->x();
            yt = vt->y();

            // if only one insertion, triangles list is not more up to date:
            //_triangles.clear();

            qDeleteAll(_outlines);
            _outlines.clear();

            _neighborsComputed = false;
            _voronoiDiagramComputed = false;


            // 1) find the triangle which contains the point to be inserted
            // faster way, actually used (walk through the triangulation from refTriangle)

            t1old = NULL; // to avoid infinite loops
            while ((!t1->contains(xt, yt)) && t1old != t1)
            {
                t1old = t1;
                t1 = t1->getNextTriangleTo(xt, yt);
            }


            if (xt == t1->_v1->x() && yt == t1->_v1->y())
            {
                _duplicatePositions.insert(vt, _insertedVertices.at(_insertedVertices.indexOf(t1->_v1)));
            } else if (xt == t1->_v2->x() && yt == t1->_v2->y())
            {
                _duplicatePositions.insert(vt, _insertedVertices.at(_insertedVertices.indexOf(t1->_v2)));
            } else if (xt == t1->_v3->x() && yt == t1->_v3->y())
            {
                _duplicatePositions.insert(vt, _insertedVertices.at(_insertedVertices.indexOf(t1->_v3)));
            } else {
                _insertedVertices.append(vt);

                // 2) create two lists :
                // - destrLst : contains triangle "to destroy"
                // - borderLst : contains the sides of the polygon formed by "to destroy" triangles
                // a side is composed of two vertices, and one bording triangle (or NULL if don't exist)

                destrLst.append(t1);
                t1->setToRemoveFlag(true);

                for (j = 0 ; j < destrLst.size() ; j++)
                {
                    t2 = destrLst.at(j);

                    // for n12 neighbor triangle
                    n1 = t2->_n12;

                    if ((n1 == NULL) || !n1->getToRemoveFlag())
                    {
                        if ((n1 != NULL) && n1->circleContains(xt,yt))
                        {
                            destrLst.append(n1);
                            n1->setToRemoveFlag(true);
                        } else {
                            borderLst.appendSide(n1, t2->_v1, t2->_v2);
                        }
                    }

                    // for n23 neighbor triangle
                    n1 = t2->_n23;

                    if ((n1 == NULL) || !n1->getToRemoveFlag()) {
                        if ((n1 != NULL) && n1->circleContains (xt,yt))
                        {
                            destrLst.append(n1);
                            n1->setToRemoveFlag(true);
                        } else {
                            borderLst.appendSide(n1, t2->_v2, t2->_v3);
                        }
                    }

                    // for n31 neighbor triangle
                    n1 = t2->_n31;

                    if ((n1 == NULL) || !n1->getToRemoveFlag()) {
                        if ((n1 != NULL) && n1->circleContains (xt,yt)) {
                            destrLst.append(n1);
                            n1->setToRemoveFlag(true);
                        } else {
                            borderLst.appendSide(n1, t2->_v3, t2->_v1);
                        }
                    }
                }

                tRecycler.addTriangles(destrLst);
                destrLst.clear();


                // 3) Construct new triangles
                // each is composed by:
                // - the point (xt, yt)
                // - a borderLst triangle side referenced in borderLst
                // after each triangle, we create the next, which has a vertex in common with it
                // (thanks to borderLst side list)
                // so each triangle is the neighbor of the next,
                // and the first is the second neighbor of the last
                // for third neighbor we use the side's border triangle


                // First triangle construction
                int sdIndex = 0;
                borderLst.removeSide(sdIndex, true);

                CT_DelaunayVertex* sd_v1 = borderLst.getRemovedV1();
                CT_DelaunayVertex* sd_v2 = borderLst.getRemovedV2();
                CT_DelaunayTriangle* sd_tri = borderLst.getRemovedTri();
                bool triangleCreated;

                vFirst = sd_v1;
                vNext = sd_v2;

                t1 = tRecycler.getTriangle(vFirst, vNext, vt, triangleCreated);
                if (triangleCreated) {_triangles.append(t1);}

                n1 = sd_tri;
                t1->_n12 = n1;
                if (n1 != NULL) {n1->setNeighbor(vFirst, vNext, t1);}

                _refTriangle = t1;
                // NB: so the refTriangle is set to be the first triangle created this step
                // to keep first triangle, see below: first and last triangles linking,
                // (and to have a refTriangle to begin next loop)


                // Others triangles construction
                while (vNext != vFirst)
                {
                    vBase = vNext;

                    // search the next side to link
                    for (j = 0 ; j < borderLst.size() ; j++)
                    {
                        sdIndex = j;
                        sd_v1 = borderLst.getV1(sdIndex);
                        sd_v2 = borderLst.getV2(sdIndex);
                        sd_tri = borderLst.getTri(sdIndex);

                        if ((sd_v1 == vBase) || (sd_v2 == vBase)) {break;}
                    }

                    vNext = borderLst.swap(sdIndex, vBase);
                    borderLst.removeSide(sdIndex);

                    t2 = tRecycler.getTriangle(vBase, vNext, vt, triangleCreated);
                    if (triangleCreated) {_triangles.append(t2);}

                    n1 = sd_tri;
                    t2->_n12 = n1;
                    if (n1 != NULL) {n1->setNeighbor (vBase, vNext, t2);}
                    t2->_n31 = t1;
                    t1->_n23 = t2;

                    t1 = t2;
                }


                // linking first triangle to last one
                t2->_n23 = _refTriangle;
                _refTriangle->_n31 = t2;


                // clearing the lists and tables for next loop
                borderLst.clear();
            }
        }
    }

    for (int i = _triangles.size() - 1; i >= 0 ; i--)
    {
        if (_triangles.at(i)->getToRemoveFlag()) {delete _triangles.takeAt(i);}
    }

    return true;
}



QList<CT_DelaunayVertex*> CT_DelaunayTriangulation::getNeighboursForCoordinates(double x, double y)
{
    QList<CT_DelaunayVertex*> neigbours;

    if (!_initialized) {return neigbours;} // we need 4 corners to continue


    CT_DelaunayTriangle* t1;
    CT_DelaunayTriangle* t1old;
    CT_DelaunayTriangle* t2;
    CT_DelaunayTriangle* n1;
    CT_DelaunayVertex* vBase;
    CT_DelaunayVertex* vNext;
    CT_DelaunayVertex* vFirst;

    // we use arrayList and not vector for performance reasons
    QList<CT_DelaunayTriangle*> destrLst;
    CT_DelaunaySideList borderLst;

    // initialisations
    t1 = _refTriangle;
    t2 = NULL;

    // is the point in the triangulation, else add to outOfBoundsVertices list
    // and DO NOT insert
    if ((x <= _minx) || (x >= _maxx) || (y <= _miny) || (y >= _maxy))
    {
        return neigbours;

        // we have to test the point in the triangulation
    } else {
        // 1) find the triangle which contains the point to be inserted
        // faster way, actually used (walk through the triangulation from refTriangle)

        t1old = NULL; // to avoid infinite loops
        while ((!t1->contains(x, y)) && t1old != t1)
        {
            t1old = t1;
            t1 = t1->getNextTriangleTo(x, y);
        }


        if (x == t1->_v1->x() && y == t1->_v1->y())
        {
            neigbours.append(t1->_v1);
            return neigbours;
        } else if (x == t1->_v2->x() && y == t1->_v2->y())
        {
            neigbours.append(t1->_v2);
            return neigbours;
        } else if (x == t1->_v3->x() && y == t1->_v3->y())
        {
            neigbours.append(t1->_v3);
            return neigbours;
        } else {

            // 2) create one list :
            // - borderLst : contains the sides of the polygon formed by "to destroy" triangles
            // a side is composed of two vertices, and one bording triangle (or NULL if don't exist)

            _refTriangle = t1;
            destrLst.append(t1);

            for (int j = 0 ; j < destrLst.size() ; j++)
            {
                t2 = destrLst.at(j);

                // for n12 neighbor triangle
                n1 = t2->_n12;

                if ((n1 == NULL) || !destrLst.contains(n1))
                {
                    if ((n1 != NULL) && n1->circleContains(x,y))
                    {
                        destrLst.append(n1);
                    } else {
                        borderLst.appendSide(n1, t2->_v1, t2->_v2);
                    }
                }

                // for n23 neighbor triangle
                n1 = t2->_n23;

                if ((n1 == NULL) || !destrLst.contains(n1)) {
                    if ((n1 != NULL) && n1->circleContains (x,y))
                    {
                        destrLst.append(n1);
                    } else {
                        borderLst.appendSide(n1, t2->_v2, t2->_v3);
                    }
                }

                // for n31 neighbor triangle
                n1 = t2->_n31;

                if ((n1 == NULL) || !destrLst.contains(n1)) {
                    if ((n1 != NULL) && n1->circleContains (x,y)) {
                        destrLst.append(n1);
                    } else {
                        borderLst.appendSide(n1, t2->_v3, t2->_v1);
                    }
                }
            }

            int sdIndex = 0;
            borderLst.removeSide(sdIndex, true);

            CT_DelaunayVertex* sd_v1 = borderLst.getRemovedV1();
            CT_DelaunayVertex* sd_v2 = borderLst.getRemovedV2();

            neigbours.append(sd_v1);
            neigbours.append(sd_v2);

            vFirst = sd_v1;
            vNext = sd_v2;

            while (vNext != vFirst)
            {
                vBase = vNext;

                // search the next side to link
                for (int j = 0 ; j < borderLst.size() ; j++)
                {
                    sdIndex = j;
                    sd_v1 = borderLst.getV1(sdIndex);
                    sd_v2 = borderLst.getV2(sdIndex);


                    if ((sd_v1 == vBase) || (sd_v2 == vBase)) {break;}
                }

                if (sd_v1 == vBase)
                {
                    neigbours.append(sd_v2);
                } else if (sd_v2 == vBase) {
                    neigbours.append(sd_v1);
                }

                vNext = borderLst.swap(sdIndex, vBase);
                borderLst.removeSide(sdIndex);
            }
        }
    }

    return neigbours;
}

void CT_DelaunayTriangulation::updateCornersZValues()
{
    computeVerticesNeighbors();
    for (int i = 0 ; i < _corners.size() ; i++)
    {
         CT_DelaunayVertex* corner  = _corners[i];

         double zmean = 0;
         int cpt = 0;

         QList<CT_DelaunayVertex*> &neighbours = corner->getNeighbors();
         for (int j =0 ; j < neighbours.size() ; j++)
         {
             CT_DelaunayVertex* neighbour  = neighbours.at(j);
             if (!_corners.contains(neighbour))
             {
                 Eigen::Vector3d* data = neighbour->getData();
                 if (data != NULL) {zmean += (*data)(2); cpt++;}
             }
         }

         Eigen::Vector3d* dataCorner = corner->getData();

         if (dataCorner!= NULL && cpt > 0) {(*dataCorner)(2) = zmean / (double)cpt;}
    }
}

const CT_DelaunayTriangle* CT_DelaunayTriangulation::findTriangleContainingPoint(double x, double y, CT_DelaunayTriangle* refTriangle)
{
    if (refTriangle == NULL)
    {
        refTriangle = _refTriangle;
    }

    CT_DelaunayTriangle* t1 = refTriangle;
    CT_DelaunayTriangle* t1old = NULL; // to avoid infinite loops

    while ((!t1->contains(x, y)) && t1old != t1)
    {
        t1old = t1;
        t1 = t1->getNextTriangleTo(x, y);
    }

    return t1;
}

const CT_DelaunayTriangle *CT_DelaunayTriangulation::getZCoordForXY(double x, double y, double &outZ, CT_DelaunayTriangle* refTriangle)
{
    const CT_DelaunayTriangle* triangle = findTriangleContainingPoint(x, y, refTriangle);

    if (!triangle->contains(x, y)) {outZ = NAN; return NULL;}

    Eigen::Vector3d vt1 = *(triangle->getVertex1()->getDataConst());
    Eigen::Vector3d vt2 = *(triangle->getVertex2()->getDataConst());
    Eigen::Vector3d vt3 = *(triangle->getVertex3()->getDataConst());

    Eigen::Vector3d u = vt2 - vt1;
    Eigen::Vector3d v = vt3 - vt1;

    outZ = vt1(2) + ( (y - vt1(1)) * (u(0)*v(2) - v(0)*u(2)) + (x - vt1(0)) * (v(1)*u(2) - u(1)*v(2)) ) / (u(0)*v(1) - u(1) * v(0));

    return triangle;
}

CT_DelaunayTriangulation *CT_DelaunayTriangulation::copy()
{
    CT_DelaunayTriangulation *cpy = new CT_DelaunayTriangulation();

    cpy->init(_minx, _miny, _maxx, _maxy);

    for (int i = 0 ; i < _insertedVertices.size() ; i++)
    {
        CT_DelaunayVertex* vertex = _insertedVertices.at(i);
        Eigen::Vector3d* v = new Eigen::Vector3d(*(vertex->getData()));
        cpy->addVertex(v, true);
    }

    cpy->doInsertion();

    return cpy;
}
