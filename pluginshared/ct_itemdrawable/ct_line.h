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

#ifndef CT_LINE_H
#define CT_LINE_H

#include "ct_itemdrawable/abstract/ct_abstractshape.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardlinedrawmanager.h"

#include "ct_shapedata/ct_linedata.h"

/*!
 * \class CT_Line
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable managing a CT_LineData</b>
 *
 *
 * It represents a line in 3D, defined by two points.
 */
class PLUGINSHAREDSHARED_EXPORT CT_Line : public CT_AbstractShape
{
    // IMPORTANT pour avoir le nom de l'ItemDrawable
    Q_OBJECT

public:

    CT_Line();
    /**
      * \brief Contructeur avec une instance des donnes (CT_LineData*), ne peut tre NULL ! (Supprime dans le destructeur de la classe).
      */
    CT_Line(const CT_OutAbstractSingularItemModel *model,
            const CT_AbstractResult *result,
            CT_LineData *data);

    CT_Line(const QString &modelName,
            const CT_AbstractResult *result,
            CT_LineData *data);

    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    inline const QVector3D& getP1() const {return ((const CT_LineData&)getData()).getP1();}
    inline const QVector3D& getP2() const {return ((const CT_LineData&)getData()).getP2();}

    inline double getP1_X() const {return ((const CT_LineData&)getData()).x1();}
    inline double getP1_Y() const {return ((const CT_LineData&)getData()).y1();}
    inline double getP1_Z() const {return ((const CT_LineData&)getData()).z1();}
    inline double getP2_X() const {return ((const CT_LineData&)getData()).x2();}
    inline double getP2_Y() const {return ((const CT_LineData&)getData()).y2();}
    inline double getP2_Z() const {return ((const CT_LineData&)getData()).z2();}
    inline double getLength() const {return ((const CT_LineData&)getData()).length();}
    inline float getError() const {return ((const CT_LineData&)getData()).getError();}

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    /**
      * \brief Retourne une ligne 3D (rgression linaire)  partir du nuage de points pass en paramtre.
      *
      * \return NULL si le nombre de points est infrieur  2.
      */
    static CT_Line* staticCreateLineFromPointCloud(const CT_OutAbstractSingularItemModel *model,
                                                   quint64 id,
                                                   const CT_AbstractResult *result,
                                                   const CT_AbstractPointCloud &pointCloud,
                                                   const CT_AbstractPointCloudIndex &pointCloudIndex);

private:
    const static CT_StandardLineDrawManager   LINE_DRAW_MANAGER;

    CT_DEFAULT_IA_BEGIN(CT_Line)
    CT_DEFAULT_IA_V2(CT_Line, CT_AbstractCategory::staticInitDataX(), &CT_Line::getP1_X, QObject::tr("X1"))
    CT_DEFAULT_IA_V2(CT_Line, CT_AbstractCategory::staticInitDataY(), &CT_Line::getP1_Y, QObject::tr("Y1"))
    CT_DEFAULT_IA_V2(CT_Line, CT_AbstractCategory::staticInitDataZ(), &CT_Line::getP1_Z, QObject::tr("Z1"))

    CT_DEFAULT_IA_V2(CT_Line, CT_AbstractCategory::staticInitDataX(), &CT_Line::getP2_X, QObject::tr("X2"))
    CT_DEFAULT_IA_V2(CT_Line, CT_AbstractCategory::staticInitDataY(), &CT_Line::getP2_Y, QObject::tr("Y2"))
    CT_DEFAULT_IA_V2(CT_Line, CT_AbstractCategory::staticInitDataZ(), &CT_Line::getP2_Z, QObject::tr("Z2"))

    CT_DEFAULT_IA_V2(CT_Line, CT_AbstractCategory::staticInitDataLength(), &CT_Line::getLength, QObject::tr("Longueur"))
    CT_DEFAULT_IA_V2(CT_Line, CT_AbstractCategory::staticInitDataR2(), &CT_Line::getError, QObject::tr("Erreur d'ajustement de la ligne"))
    CT_DEFAULT_IA_END(CT_Line)

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_AbstractShape>(*this);
    }
};

BOOST_CLASS_EXPORT_KEY(CT_Line)

#else
};
#endif

#endif // CT_LINE_H
