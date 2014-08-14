/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forets (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Metiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

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

#ifndef CT_ABSTRACTPROFILE_H
#define CT_ABSTRACTPROFILE_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"
#include "ct_math/ct_math.h"

#include <QVector3D>
#include <QMutex>

/*!
 * \class CT_AbstractProfile
 * \ingroup PluginShared_Items
 * \brief <b>Commun abstract base of all templated CT_AbstractProfile<type> </b>
 *
 * It's usefull to manage generically a profile, without knowing its template type
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractProfile : public CT_AbstractItemDrawableWithoutPointCloud
{
public:

    /**
      * \brief Empty Contructor
      */
    CT_AbstractProfile();

    CT_AbstractProfile(const CT_OutAbstractItemModel *model, CT_AbstractResult *result);
    CT_AbstractProfile(const QString &modelName, CT_AbstractResult *result);

    virtual ~CT_AbstractProfile();

    virtual QString getType() const;
    static QString staticGetType();

    virtual QString name() const = 0;

    /*!
     * \brief Return a [0;1] value for any type (or -1 for NA)
     * \param index index in the grid
     * \return A float value between 0 (min value) and 1 (max value)
     */
    virtual float ratioValueAtIndex(const size_t &index) const = 0;

    /*!
     * \brief Return a double value for any type (or NAN for NA)
     * \param index index in the grid
     * \return A double value
     */
    virtual double valueAtIndexAsDouble(const size_t &index) const = 0;


    /*!
     * \brief Return a QString value for any type (or -1 for NA)
     * \param index index in the grid
     * \return A QString représenting value
     */
    virtual QString valueAtIndexAsString(const size_t &index) const = 0;

    /*!
     * \brief return na value as a string
     *
     */
    virtual QString NAAsString() const = 0;

    /*!
     * \brief Return index for specified length
     * \param l Length from the profil origin
     * \param index Returned index
     * \return false if length invalid
     */
    inline bool indexForLength(const float &l, size_t &index) const
    {
        if (l < 0 || l > maxLength()) {return false;}
        if (l == maxLength())
        {
            index =  _dim - 1;
        } else {
            index = (size_t) floor(l / _res);
        }
        return true;
    }

    /*!
     * \brief Return index for specified length
     * \param l Length from the profil origin
     * \param index Returned index
     * \return false if length invalid
     */
    inline bool indexForXYZ(const float &x, const float &y, const float &z, size_t &index) const
    {
        float length = _direction.x()*(x - minX()) + _direction.y()*(y - minY()) + _direction.z()*(z - minZ());
        return indexForLength(length, index);
    }

    /*!
     * \brief Generic setter taking a double as input value
     * \param index Index of cell to modify
     * \param value Double value to cast in effective type
     */
    virtual void setValueAtIndexFromDouble(const size_t &index, const double &value) = 0;

    /*!
     * \brief Compute min and max values
     */
    virtual void computeMinMax() = 0;

    /**
      * \brief Gives the resolution
      * \return Resolution (m)
      */
    inline float resolution() const {return _res;}

    /**
     * \brief getMinCoordinates
     * \param min Min coordinates of the grid (bottom left corner)
     */
    inline void getMinCoordinates(QVector3D &min) const
    {
        min.setX(minX());
        min.setY(minY());
        min.setZ(minZ());
    }

    /**
     * \brief getMaxCoordinates
     * \param max Max coordinates of the grid (upper right corner)
     */
    inline void getMaxCoordinates(QVector3D &max) const
    {
        max.setX(maxX());
        max.setY(maxY());
        max.setZ(maxZ());
    }


    /*!
     * \brief Profil length on along the axis
     * \return Max Lenght
     */
    inline float maxLength() const {return _maxLength;}


    /*!
     * \brief Total number of cells for the grid
     * \return Number of cells
     */
    inline size_t nCells() const {return _dim;}

    /*!
     * \brief Give length on the profil axis For specified Index
     * \param index
     * \return length (could be outside of the profil range)
     */
    inline float lengthForIndex(const size_t &index) const
    {
        if (index >= _dim) {return maxLength();}
        return (index * _res) + _res / 2.0;
    }

    /*!
     * \brief Gives the X coordinate of the center of cells at index
     * \return X coordinate
     */
    inline float getCellCenterX(const size_t &index) const
    {
        float length = lengthForIndex(index) ;
        return minX() + length * _direction.x();
    }

    /*!
     * \brief Gives the Y coordinate of the center of cells at index
     * \return Y coordinate
     */
    inline float getCellCenterY(const size_t &index) const
    {
        float length = lengthForIndex(index) ;
        return minY() + length * _direction.y();
    }

    /*!
     * \brief Gives the Z coordinate of the center of cells at index
     * \return Z coordinate
     */
    inline float getCellCenterZ(const size_t &index) const
    {
        float length = lengthForIndex(index) ;
        return minZ() + length * _direction.z();
    }

    /*!
     * \brief Gives the (X,Y,Z) coordinates of the center of cell at index
     */
    inline bool getCellCenterXYZ(const size_t &index, QVector3D &center) const
    {
        if (index >= _dim) {return false;}

        float length = lengthForIndex(index) ;
        center.setX(minX() + length * _direction.x());
        center.setY(minY() + length * _direction.y());
        center.setZ(minZ() + length * _direction.z());
        return true;
    }

    /*!
     * \brief getCellCoordinates Give min and max coordinates of a cell (on the profil axis)
     * \param index Index of the cell
     * \param bottom  Min coordinates
     * \param top Max coordinates
     * \return true if index is in the grid
     */
    inline bool getCellCoordinates(const size_t &index, QVector3D &bottom, QVector3D &top) const
    {
        if (index >= _dim) {return false;}

        float length = lengthForIndex(index) ;
        float lengthBottom = length - (_res/2.0);
        float lengthTop = length + (_res/2.0);

        bottom.setX(minX() + lengthBottom * _direction.x());
        bottom.setY(minY() + lengthBottom * _direction.y());
        bottom.setZ(minZ() + lengthBottom * _direction.z());

        top.setX(minX() + lengthTop * _direction.x());
        top.setY(minY() + lengthTop * _direction.y());
        top.setZ(minZ() + lengthTop * _direction.z());

        return true;
    }

    inline const QVector3D &getDirection() const {return _direction;}


protected:
    size_t         _dim;       /*!< Nombre de cases du profile */
    float          _res;       /*!< Resolution du profile (taille d'une case) */
    float          _maxLength; /*!< Longueur du profil le long de l'axe */

    QVector3D      _direction; /*!< Direction of the profil axis */

};

#endif // CT_ABSTRACTPROFILE_H
