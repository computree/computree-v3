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

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"                           // Inherits from CT_abstractItemDrawableWithoutPointCloud
#include "ct_point.h"                                                           // Used to get rays from a scanner
#include <math.h>                                                               // Used for M_PI constant
#include "ct_itemdrawable/ct_beam.h"                                                             // A scan creates some rays
#include "ct_itemdrawable/tools/drawmanager/ct_standardscannerdrawmanager.h"

/*! \def    DEG2RAD
            Linear constant to cast an angle in degrees to this angle in radians.
            */
#define DEG2RAD (M_PI/180.0)

/*! \def    RAD2DEG
            Linear constant to cast an angle in radians to this angle in degrees.
            */
#define RAD2DEG (180.0/M_PI)

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
private :
    Q_OBJECT

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
    CT_Scanner(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, int scanId = 0, bool clocWise = true );

    /*!
    *  \brief Constructor
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


    CT_Scanner(const QString &modelName, const CT_AbstractResult *result, int scanId = 0, bool clocWise = true );

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

    /*!
    *  \brief Getter of the class
    *
    *  \return Returns the ID of the scanner
    */
    inline Eigen::Vector3d getZVector() const { return _zVector; }

    inline double getZVectorX() const {return _zVector(0);}
    inline double getZVectorY() const {return _zVector(1);}
    inline double getZVectorZ() const {return _zVector(2);}


    /*!
    *  \brief Getter of the class
    *
    *  \return Returns the horizontal field of view of the scanner
    */
    inline double getHFov() const { return _hFov; }

    /*!
    *  \brief Getter of the class
    *
    *  \return Returns the vertical field of view of the scanner
    */
    inline double getVFov() const { return _vFov; }

    /*!
    *  \brief Getter of the class
    *
    *  \return Returns the horizontal resolution of the scanner
    */
    inline double getHRes() const { return _hRes; }

    /*!
    *  \brief Getter of the class
    *
    *  \return Returns the vertical resolution of the scanner
    */
    inline double getVRes() const { return _vRes; }

    /*!
    *  \brief Getter of the class
    *
    *  \return Returns the initial theta of the scanner
    */
    inline double getInitTheta() const { return _initTheta; }

    /*!
    *  \brief Getter of the class
    *
    *  \return Returns the initial phi of the scanner
    */
    inline double getInitPhi() const { return _initPhi; }

    /*!
    *  \brief Getter of the class
    *
    *  \return Returns the number of horizontal rays of the scanner
    */
    inline int getNHRays() const { return _nHRays; }

    /*!
    *  \brief Getter of the class
    *
    *  \return Returns the number of vertical rays of the scanner
    */
    inline double getNVRays() const { return _nVRays; }

    /*!
    *  \brief Getter of the class
    *
    *  \return Returns true if the scanner is clockwise, false else
    */
    inline double getClockWise() const { return _clockWise; }

//********************************************//
//                  Setters                   //
//********************************************//
    /*!
    *  \brief Setter of the class
    */
    inline void setID ( int scanID ) { _scanID = scanID; }

    /*!
    *  \brief Setter of the class
    */
    inline void setPosition ( const QVector3D& position ) { setCenterX(position.x());setCenterY(position.y());setCenterZ(position.z()); }

    /*!
    *  \brief Setter of the class
    */
    inline void setZVector ( const Eigen::Vector3d& zVector ) { _zVector = zVector; }

    /*!
    *  \brief Setter of the class
    */
    inline void setHFov ( double hFov ) { _hFov = hFov; }

    /*!
    *  \brief Setter of the class
    */
    inline void setVFov ( double vFov ) { _vFov = vFov; }

    /*!
    *  \brief Setter of the class
    */
    inline void setHRes ( double hRes ) { _hRes = hRes; }

    /*!
    *  \brief Setter of the class
    */
    inline void setVRes ( double vRes ) { _vRes = vRes; }

    /*!
    *  \brief Setter of the class
    */
    inline void setInitTheta ( double initTheta ) { _initTheta = initTheta; }

    /*!
    *  \brief Setter of the class
    */
    inline void setInitPhi ( double initPhi ) { _initPhi = initPhi; }

    /*!
    *  \brief Setter of the class
    */
    inline void setNHRays ( int nHRays ) { _nHRays = nHRays; }

    /*!
    *  \brief Setter of the class
    */
    inline void setNVRays ( int nVRays ) { _nVRays = nVRays; }

    /*!
    *  \brief Setter of the class
    */
    inline void setClockWise ( bool clockWise ) { _clockWise = clockWise; }

//***********************************************************************************//
//      Virtual/redefined methods from CT_AbstractItemDrawableWithoutPointCloud      //
//***********************************************************************************//
    /*!
    *  \brief Method inherited from CT_AbstractItemDrawableWithoutPointCloud that needs to be redefined
    *  Useful for the computree results/itemDrawable management/drawing/printing
    *
    *  \warning This method MUST be redefined in each class that inherits from this one
    *  \return Returns a string representing the type of object
    */
    virtual QString getType() const;

    /*!
    *  \brief Method inherited from CT_AbstractItemDrawableWithoutPointCloud that needs to be redefined
    *  Useful for the computree results/itemDrawable management/drawing/printing
    *  Method similar to "QString getType()" but declared as static
    *
    *  \warning This method MUST be redefined in each class that inherits from this one
    *  \return Returns a string representing the type of object
    */
    static QString staticGetType();

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
    *  \param moreStability : if equals true, then direction component close to 0 are set to 0. This avoids some numerical instabilities in some cases.
    *
    *  \return Returns the (ith, jth) beam
    */
    CT_Beam* beam ( int i, int j, bool moreStability = false ) const;

    /*!
    *  \brief Provides access the (ith, jth) beam
    *
    *  \param i : horizontal index of the beam
    *  \param j : vertical index of the beam
    *  \param moreStability : if equals true, then direction component close to 0 are set to 0. This avoids some numerical instabilities in some cases.
    *  \param beam : Returns the (ith, jth) beam
    */
    void beam ( int i, int j, CT_Beam &beam, bool moreStability = false ) const;

private :
    int             _scanID;        /*!< ID of the scan*/
    Eigen::Vector3d	_zVector;		/*!< direction of the scan's vertica in the world coordinate system*/
    double          _hFov;			/*!< horizontal field of view*/
    double          _vFov;			/*!< vertical field of view*/
    double          _hRes;			/*!< horizontal angle resolution of the scan*/
    double          _vRes;			/*!< vertical angle resolution of the scan*/
    double          _initTheta;		/*!< initial horizontal angle of the measurement in the world coordinate system*/
    double          _initPhi;		/*!< initial vertical angle of the measurement in the world coordinate system*/
    int             _nHRays;		/*!< number of ray on a entire horizontal move of the scan*/
    int             _nVRays;		/*!< number of ray on a entire horizontal move of the scan*/
    bool            _clockWise;     /*!< Whether the scan has been done in clockwise or not*/

    CT_DEFAULT_IA_BEGIN(CT_Scanner)
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataId(), &CT_Scanner::getScanID, QObject::tr("ScanID"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataAngle(), &CT_Scanner::getHFov, QObject::tr("HFov"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataAngle(), &CT_Scanner::getVFov, QObject::tr("VFov"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataResolution(), &CT_Scanner::getHRes, QObject::tr("HRes"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataResolution(), &CT_Scanner::getVRes, QObject::tr("VRes"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataAngle(), &CT_Scanner::getInitTheta, QObject::tr("InitTheta"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataAngle(), &CT_Scanner::getInitPhi, QObject::tr("InitPhi"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataNumber(), &CT_Scanner::getNHRays, QObject::tr("NHRays"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataNumber(), &CT_Scanner::getNVRays, QObject::tr("NVRays"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataValue(), &CT_Scanner::getClockWise, QObject::tr("ClockWise"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataX(), &CT_Scanner::getPositionX, QObject::tr("PositionX"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataY(), &CT_Scanner::getPositionY, QObject::tr("PositionY"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataZ(), &CT_Scanner::getPositionZ, QObject::tr("PositionZ"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataXDirection(), &CT_Scanner::getZVectorX, QObject::tr("ZVectorX"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataYDirection(), &CT_Scanner::getZVectorY, QObject::tr("ZVectorY"))
    CT_DEFAULT_IA_V2(CT_Scanner, CT_AbstractCategory::staticInitDataZDirection(), &CT_Scanner::getZVectorZ, QObject::tr("ZVectorZ"))
    CT_DEFAULT_IA_END(CT_Scanner)



    const static CT_StandardScannerDrawManager CT_SCANNER_DRAW_MANAGER;
};

#endif // CT_SCANNER_H
