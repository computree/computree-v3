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

#ifndef CT_DELAUNAYTRIANGULATION_H
#define CT_DELAUNAYTRIANGULATION_H

#include "pluginShared_global.h"

#include "ct_delaunayvertex.h"
#include "ct_delaunaytriangle.h"
#include "ct_delaunayside.h"
#include "ct_delaunayoutline.h"
#include "ct_delaunayvorovertex.h"

#include <QList>
#include <QVector>
#include <QMap>

#include <numeric>

/**
 * DelaunayTriangulation - Delaunay triangulation.
 *
 * @author A. Piboule - february 2004
 */
class PLUGINSHAREDSHARED_EXPORT CT_DelaunayTriangulation
{

public:
    CT_DelaunayTriangulation ();
    ~CT_DelaunayTriangulation ();

    void clear(bool clearToInsert = true);


    // Construct the 4 corners of the triangulation, and the 2 first triangles
    void init (double x1, double y1, double x2, double y2);


    // idem but corners are calculated from others points
    bool init ();


    // Accesors:

    // gives all vertices successfully inserted in the triangulation
    inline const QList<CT_DelaunayVertex*> &getInsertedVertices () {return _insertedVertices;}

    // gives all out of bounds vertices (which have NOT been inserted)
    inline const QList<CT_DelaunayVertex*> &getOutOfBoundsVertices () {return _outOfBoundsVertices;}

    // the returned hashMap gives for each (not inserted) duplicated vertex (in term of position)
    // a link to the same position vertex which has been inserted.
    inline const QMap<CT_DelaunayVertex*, CT_DelaunayVertex*> &getDuplicatePositions () {return _duplicatePositions;}

    // gives the 4 bounds conners which have been added in init (),
    // these 4 points are NOT in insertedVertex List
    inline const QVector<CT_DelaunayVertex*> &getCorners () {return _corners;}

    // gives a list of destructed triangles during last vertex insertion
    inline const QList<CT_DelaunayTriangle*> &getLastDestructedTriangles () {return _lastDestrLst;}

    // gives a list of border triangles during last vertex insertion
    // these triangles are destructed triangles neighbors
    // new triangles created during insertion have 1 common side with them
    inline const QList<CT_DelaunaySide*> &getLastBorderTriangles () {return _lastBorderLst;}

    // gives the triangle used to enter in triangulation to find "to insert" vertex triangle
    const CT_DelaunayTriangle* getRefTriangle () {return _refTriangle;}


    // accessors for bools
    inline void setSavingMode (bool t) {_savingMode = t;}
    inline bool getSavingMode () {return _savingMode;}

    inline bool isInitialized () {return _initialized;}



    // gives the up to date list of all triangles
    const QList<CT_DelaunayTriangle*> &getTriangles ();

    // gives the insertedVertices list with up to date computed neighbors for each vertex
    const QList<CT_DelaunayVertex*> &getVerticesNeighbors ();

    // gives the insertedVertices list with up to date computed Voronoi Diagram for each vertex
    // Voronoi diagram consist for each vertex, in a sorted list of DelaunayVoroVertex defining the Voronoi Polygon for this vertex
    const QList<CT_DelaunayVertex*> &getVoronoiDiagram ();


    // gives the same thing that getVoronoiDiagram
    // but without voronoi polygon of corners neighbors (to avoid border effects)

    const QList<CT_DelaunayVertex*> &getCleanVoronoiDiagram ();

    // gives the outline defined by the triangulation (corners excluded)
    const QList<CT_DelaunayOutline *> &getOutlines();


    // adds all triangles of the triangulation to an ArrayList
    void computeTriangles ();


    // compute for each vertex its neighbors
    const QList<CT_DelaunayVertex*> &computeVerticesNeighbors ();


    // create an Map : the keys are inserted vertex
    //					   the values are voronoi's polygons for these inserted vertex
    const QList<CT_DelaunayVertex*> &computeVoronoiDiagram ();


    // compute the Outline of the triangulation
    const QList<CT_DelaunayOutline*> &computeOutlines ();


    // add a DelaunayVertex to the toInsert list
    CT_DelaunayVertex* addVertex (Eigen::Vector3d *data, bool deleteData);


    // add all vertices present in toInsert list to the triangulation
    bool doInsertion ();

private:
    QList<CT_DelaunayVertex*> _toInsert; // working list of to be inserted vertex


    QList<CT_DelaunayVertex*>                  _insertedVertices;     // list of inserted vertices
    QList<CT_DelaunayVertex*>                  _outOfBoundsVertices;  // list of not inserted vertices because a position out of bounds
    QMap<CT_DelaunayVertex*, CT_DelaunayVertex*>   _duplicatePositions;   // list of not inserted vertices because an already existing position
    QList<CT_DelaunayTriangle*>                _triangles;            // list of all triangles of the triangulation (created in compute ())

    QVector<CT_DelaunayVertex*>                _corners;              // list of the 4 created corners (not present in insertedVertices list)

    double _minx; // working bounds of the triangulation
    double _miny;
    double _maxx;
    double _maxy;

    CT_DelaunayTriangle* _refTriangle; // actual reference triangle
                                    // used as start point to find the inserted vertex triangle

    QList<CT_DelaunayOutline*> _outlines; // outline defined by the triangulation

    bool _initialized; // check if bounds have been set
    bool _neighborsComputed; // check if neighbors are up to date
    bool _voronoiDiagramComputed; // check if voronoi diagram is up to date

    bool _savingMode; // are destr and border lists keeped in lastDestrLst and lastBorderLst ?

    QList<CT_DelaunayTriangle*> _lastDestrLst;   // contains insertion process informations
    QList<CT_DelaunaySide*>     _lastBorderLst;  // about (only) LAST vertex insertion
                                              // only if savingMode==true

};


#endif // CT_DELAUNAYTRIANGULATION_H
