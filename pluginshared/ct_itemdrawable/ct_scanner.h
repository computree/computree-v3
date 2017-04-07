/****************************************************************************

 Copyright (C) 2012-2012 Université de Sherbrooke, Québec, CANADA
                     All rights reserved.

 Contact :  richard.fournier@usherbrooke.ca
            jean-francois.cote@nrcan-rncan.gc.ca
            joris.ravaglia@gmail.com

 Developers : Joris RAVAGLIA

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#ifndef CT_SCANNER_H
#define CT_SCANNER_H

/** \file ct_scanner.h
    \author J. Ravaglia - mail to : joris.ravaglia@gmail.com
    \version 0.1
*/
#include <memory>
#include <math.h>                                                               // Used for M_PI constant
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"                           // Inherits from CT_abstractItemDrawableWithoutPointCloud
#include "ct_point.h"                                                           // Used to get rays from a scanner
#include "ct_itemdrawable/ct_beam.h"                                                             // A scan creates some rays
#include "ct_itemdrawable/tools/drawmanager/ct_standardscannerdrawmanager.h"
#include "ct_itemdrawable/tools/scanner/ct_thetaphishootingpattern.h"

/*! \def    SCANNER_EPSILON
            Used to avoid some instabilities : in some cases, we want values less than this constant (10e-5) become 0
            */
#define SCANNER_EPSILON 0.00001

/** \class  CT_Scanner
    \brief  This class represents a terrestrial LiDAR scanner.
            Angles are measured in radians and the theta/phi attributes are calculated on a spherical coordinates basis.
    \ingroup PluginShared_Items
    \todo   Un scanner est-il un item drawable ? (Affichage de sa position) ?
*/
class PLUGINSHAREDSHARED_EXPORT CT_Scanner : public CT_AbstractItemDrawableWithoutPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_Scanner, CT_AbstractItemDrawableWithoutPointCloud, Scan position)

public:
//********************************************//
//         Constructors/Destructors           //
//********************************************//
    /*!
    *  \brief Default constructor
    *
    *  Default constructor of the class
    *  Each attribute will be set to 0 or NULL
    *  Each vector will be empty
    *  \warning The _zVector attribute is set to (0,0,1) by default
    *
    */
    CT_Scanner( int scanID = 0, bool clockWise = true );

    /*!
    *  \brief Constructor
    *
    *  Constructor of the class
    *  Only take into account the CT_AbstractItemDrawableWithoutPointCloud members
    *  Each attribute will be set to 0 or NULL
    *  Each vector will be empty
    *  \warning The _zVector attribute is set to (0,0,1) by default
    *
    */
    CT_Scanner(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, int scanId = 0, bool clockWise = true );

    /*!
    *  \brief Constructor for scanner with theta phi shooting pattern
    *
    *  Constructor of the class
    *
    *  \param scanID : ID of the scan
    *  \param position : position of the scan in world coordinate system
    *  \param zVector : vertica of the scan : (0,0,1) by default
    *  \param hFov : horizontal field of view
    *  \param vFov : vertical field of view
    *  \param hRes : horizontal resolution (angle between two consecutive horizontal rays)
    *  \param vRes : vertical resolution
    *  \param initTheta : (horizontal) angle between the first ray and the Ox axis
    *  \param initPhi : (vertical) angle between the first vertical ray and the Oz axis
    *  \param radians : type of angle (radians or degrees), degrees by default
    */
    CT_Scanner(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, int scanID, const Eigen::Vector3d &position, const Eigen::Vector3d &zVector, double hFov, double vFov, double hRes, double vRes, double initTheta, double initPhi, bool clockWise, bool radians = false );


    CT_Scanner(const QString &modelName, const CT_AbstractResult *result, int scanId = 0, bool clockWise = true );

    CT_Scanner(const QString &modelName, const CT_AbstractResult *result, int scanID, const Eigen::Vector3d& position, const Eigen::Vector3d& zVector, double hFov, double vFov, double hRes, double vRes, double initTheta, double initPhi, bool clockWise, bool radians = false );


    /*!
    *  \brief Destructor
    *
    *  Destructor of the class
    *
    */
    ~CT_Scanner();

//********************************************//
//                  Getters                   //
//********************************************//
    /*!
    *  \brief Getter of the class
    *
    *  \return Returns the ID of the scanner
    */
    inline int getScanID() const { return _scanID; }

    /*!
    *  \brief Getter of the class
    *
    *  \return Returns the position of the scanner
    */
    inline Eigen::Vector3d getPosition() const { return getCenterCoordinate(); }

    inline double getPositionX() const {return getCenterX();}
    inline double getPositionY() const {return getCenterY();}
    inline double getPositionZ() const {return getCenterZ();}

    /**
     * @brief Returns the shooting pattern
     */
    CT_ShootingPattern* getShootingPattern() const;

//********************************************//
//                  Setters                   //
//********************************************//
    /*!
    *  \brief Setter of the class
    */
    inline void setID ( int scanID ) { _scanID = scanID; }

//***********************************************************************************//
//      Virtual/redefined methods from CT_AbstractItemDrawableWithoutPointCloud      //
//***********************************************************************************//

    /*!
    *  \brief Method inherited from CT_AbstractItemDrawableWithoutPointCloud that needs to be redefined (pure virtual)
    *  \return Returns a copy of the itemDrawable with a different id and a different result
    */
    virtual CT_Scanner* copy(const CT_OutAbstractItemModel *model,
                             const CT_AbstractResult *result,
                             CT_ResultCopyModeList copyModeList);

//********************************************//
//                   Tools                    //
//********************************************//
public :

    /*!
    *  \brief Provides access the (ith, jth) beam
    *
    *  \param i : horizontal index of the beam
    *  \param j : vertical index of the beam
    *  \param beam : Returns the (ith, jth) beam
    */
    void beam(int i, CT_Beam &beam) const;

private :
    int                             _scanID;        /*!< ID of the scan*/

    std::unique_ptr<CT_ShootingPattern>     m_shootingPattern;

    CT_DEFAULT_IA_BEGIN(CT_Scanner)
    CT_DEFAULT_IA_V3(CT_Scanner, CT_AbstractCategory::staticInitDataId(), &CT_Scanner::getScanID, QObject::tr("ScanID"), "sid")
    CT_DEFAULT_IA_V3(CT_Scanner, CT_AbstractCategory::staticInitDataX(), &CT_Scanner::getPositionX, QObject::tr("PositionX"), "px")
    CT_DEFAULT_IA_V3(CT_Scanner, CT_AbstractCategory::staticInitDataY(), &CT_Scanner::getPositionY, QObject::tr("PositionY"), "py")
    CT_DEFAULT_IA_V3(CT_Scanner, CT_AbstractCategory::staticInitDataZ(), &CT_Scanner::getPositionZ, QObject::tr("PositionZ"), "pz")
    CT_DEFAULT_IA_END(CT_Scanner)

    const static CT_StandardScannerDrawManager CT_SCANNER_DRAW_MANAGER;
};

#endif // CT_SCANNER_H
