/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

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

#include "ct_ellipse.h"

CT_DEFAULT_IA_INIT(CT_Ellipse)
const CT_StandardEllipseDrawManager CT_Ellipse::ELLIPSE_DRAW_MANAGER;

CT_Ellipse::CT_Ellipse(): CT_AbstractShape()
{
    setBaseDrawManager(&ELLIPSE_DRAW_MANAGER);
}

CT_Ellipse::CT_Ellipse(const CT_OutAbstractSingularItemModel *model,
                       const CT_AbstractResult *result,
                       CT_EllipseData *data) : CT_AbstractShape(model, result, data)
{
    setBaseDrawManager(&ELLIPSE_DRAW_MANAGER);
}

CT_Ellipse::CT_Ellipse(const QString &modelName,
                       const CT_AbstractResult *result,
                       CT_EllipseData *data) : CT_AbstractShape(modelName, result, data)
{
    setBaseDrawManager(&ELLIPSE_DRAW_MANAGER);
}

QString CT_Ellipse::name() const
{
    return tr("3D ellipse");
}

const CT_LineData& CT_Ellipse::getAxisA() const
{
    return ((const CT_EllipseData&)getData()).getAxisA();
}

const CT_LineData& CT_Ellipse::getAxisB() const
{
    return ((const CT_EllipseData&)getData()).getAxisB();
}

double CT_Ellipse::getError() const
{
    return ((const CT_EllipseData&)getData()).getError();
}

double CT_Ellipse::getAxisRatio() const
{
    return getAxisA().length()/getAxisB().length();
}

double CT_Ellipse::getAxisAP1X() const
{
    return getAxisA().getP1().x();
}

double CT_Ellipse::getAxisAP1Y() const
{
    return getAxisA().getP1().y();
}

double CT_Ellipse::getAxisAP1Z() const
{
    return getAxisA().getP1().z();
}

double CT_Ellipse::getAxisAP2X() const
{
    return getAxisA().getP2().x();
}

double CT_Ellipse::getAxisAP2Y() const
{
    return getAxisA().getP2().y();
}

double CT_Ellipse::getAxisAP2Z() const
{
    return getAxisA().getP2().z();
}

double CT_Ellipse::getAxisALength() const
{
    return getAxisA().length();
}

double CT_Ellipse::getAxisBP1X() const
{
    return getAxisB().getP1().x();
}

double CT_Ellipse::getAxisBP1Y() const
{
    return getAxisB().getP1().y();
}

double CT_Ellipse::getAxisBP1Z() const
{
    return getAxisB().getP1().z();
}

double CT_Ellipse::getAxisBP2X() const
{
    return getAxisB().getP2().x();
}

double CT_Ellipse::getAxisBP2Y() const
{
    return getAxisB().getP2().y();
}

double CT_Ellipse::getAxisBP2Z() const
{
    return getAxisB().getP2().z();
}

double CT_Ellipse::getAxisBLength() const
{
    return getAxisB().length();
}

CT_AbstractItemDrawable* CT_Ellipse::copy(const CT_OutAbstractItemModel *model,
                                          const CT_AbstractResult *result,
                                          CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Ellipse *el = new CT_Ellipse((const CT_OutAbstractSingularItemModel *)model, result, (getPointerData() != NULL) ? ((const CT_EllipseData&)getData()).clone() : NULL);
    el->setId(id());
    el->setAlternativeDrawManager(getAlternativeDrawManager());

    return el;
}

CT_Ellipse* CT_Ellipse::staticCreateZAxisAlignedEllipseFromPointCloud(const CT_OutAbstractSingularItemModel *model,
                                                                      quint64 id,
                                                                      const CT_AbstractResult *result,
                                                                      const CT_AbstractPointCloudIndex *pointCloudIndex)
{
    Q_UNUSED(id)

    CT_EllipseData *data = CT_EllipseData::staticCreateZAxisAlignedEllipseDataFromPointCloud(pointCloudIndex);

    if(data == NULL)
        return NULL;

    return new CT_Ellipse((const CT_OutAbstractSingularItemModel *)model, result, data);
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_EllipseData)
BOOST_CLASS_EXPORT_IMPLEMENT(CT_Ellipse)
#endif
