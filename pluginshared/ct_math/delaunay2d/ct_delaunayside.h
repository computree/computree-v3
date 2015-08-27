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

#ifndef CT_DELAUNAYSIDE_H
#define CT_DELAUNAYSIDE_H


#include "pluginShared_global.h"
#include "ct_delaunayvertex.h"
#include "ct_delaunaytriangle.h"


/**
 * DelaunaySide - side of the Delaunay triangulation.
 *
 * @author A. Piboule - february 2004
 */
class PLUGINSHAREDSHARED_EXPORT CT_DelaunaySide
{

public:
    CT_DelaunaySide (CT_DelaunayTriangle* trit, CT_DelaunayVertex* v1t, CT_DelaunayVertex* v2t);

    // give the other vertex of the side
    CT_DelaunayVertex *next(CT_DelaunayVertex* vt);

    bool equals (CT_DelaunaySide* sd);

private:
    friend class CT_DelaunayTriangulation;

    CT_DelaunayVertex*     _v1;
    CT_DelaunayVertex*     _v2;
    CT_DelaunayTriangle*   _tri;

};



#endif // CT_DELAUNAYSIDE_H
