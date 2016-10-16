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

#ifndef CT_ELLIPSE_H
#define CT_ELLIPSE_H

#include "ct_itemdrawable/abstract/ct_abstractshape.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardellipsedrawmanager.h"
#include "ct_shapedata/ct_ellipsedata.h"

#define _USE_MATH_DEFINES
#include <math.h>

/*!
 * \class CT_Ellipse
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable managing a CT_EllipseData</b>
 *
 * It represents an ellipse in 3D, defined by two 3D segments.
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_Ellipse : public CT_AbstractShape
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_Ellipse, CT_AbstractShape, 3D ellipse)

public:

    CT_Ellipse();
    /**
      * \brief Contructeur avec une instance des donnes (CT_CircleData*), ne peut tre NULL ! (Supprime dans le destructeur de la classe).
      */
    CT_Ellipse(const CT_OutAbstractSingularItemModel *model,
               const CT_AbstractResult *result,
               CT_EllipseData *data);

    CT_Ellipse(const QString &modelName,
               const CT_AbstractResult *result,
               CT_EllipseData *data);


    const CT_LineData& getAxisA() const;
    const CT_LineData& getAxisB() const;
    double getError() const;
    double getAxisRatio() const;

    double getAxisAP1X() const;
    double getAxisAP1Y() const;
    double getAxisAP1Z() const;

    double getAxisAP2X() const;
    double getAxisAP2Y() const;
    double getAxisAP2Z() const;

    double getAxisALength() const;

    double getAxisBP1X() const;
    double getAxisBP1Y() const;
    double getAxisBP1Z() const;

    double getAxisBP2X() const;
    double getAxisBP2Y() const;
    double getAxisBP2Z() const;

    double getAxisBLength() const;

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model,
                                          const CT_AbstractResult *result,
                                          CT_ResultCopyModeList copyModeList);

    /**
      * \brief Retourne une ellipse 2D  partir du nuage de points pass en paramtre.
      *
      * \return NULL si le nombre de points est infrieur  6.
      */
    static CT_Ellipse* staticCreateZAxisAlignedEllipseFromPointCloud(const CT_OutAbstractSingularItemModel *model,
                                                                     quint64 id,
                                                                     const CT_AbstractResult *result,
                                                                     const CT_AbstractPointCloudIndex *pointCloudIndex);

private:
    const static CT_StandardEllipseDrawManager  ELLIPSE_DRAW_MANAGER;

    CT_DEFAULT_IA_BEGIN(CT_Ellipse)
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataX(), &CT_Ellipse::getAxisAP1X, QObject::tr("X1 axe mineur"), "x1ami")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataY(), &CT_Ellipse::getAxisAP1Y, QObject::tr("Y1 axe mineur"), "y1ami")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataZ(), &CT_Ellipse::getAxisAP1Z, QObject::tr("Z1 axe mineur"), "z1ami")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataX(), &CT_Ellipse::getAxisAP2X, QObject::tr("X2 axe mineur"), "x2ami")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataY(), &CT_Ellipse::getAxisAP2Y, QObject::tr("Y2 axe mineur"), "y2ami")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataZ(), &CT_Ellipse::getAxisAP2Z, QObject::tr("Z2 axe mineur"), "z2ami")

    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataX(), &CT_Ellipse::getAxisBP1X, QObject::tr("X1 axe majeur"), "x1ama")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataY(), &CT_Ellipse::getAxisBP1Y, QObject::tr("Y1 axe majeur"), "y1ama")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataZ(), &CT_Ellipse::getAxisBP1Z, QObject::tr("Z1 axe majeur"), "z1ama")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataX(), &CT_Ellipse::getAxisBP2X, QObject::tr("X2 axe majeur"), "x2ama")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataY(), &CT_Ellipse::getAxisBP2Y, QObject::tr("Y2 axe majeur"), "y2ama")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataZ(), &CT_Ellipse::getAxisBP2Z, QObject::tr("Z2 axe majeur"), "z2ama")

    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataLength(), &CT_Ellipse::getAxisALength, QObject::tr("Longueur axe mineur"), "lami")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataLength(), &CT_Ellipse::getAxisBLength, QObject::tr("Longueur axe majeur"), "lama")

    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataR2(), &CT_Ellipse::getError, QObject::tr("Erreur d'ajustement de l'ellipse"), "eade")

    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataRatio(), &CT_Ellipse::getAxisRatio, QObject::tr("Ratio des axes"), "rda")

    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataAngle(), 0.0, QObject::tr("Angle de début"), "add")
    CT_DEFAULT_IA_V3(CT_Ellipse, CT_AbstractCategory::staticInitDataAngle(), (2.0*M_PI), QObject::tr("Angle de fin"), "adf")

    CT_DEFAULT_IA_END(CT_Ellipse)

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_AbstractShape>(*this);
    }
};

BOOST_CLASS_EXPORT_KEY(CT_Ellipse)

#else
};
#endif

#endif // CT_ELLIPSE_H
