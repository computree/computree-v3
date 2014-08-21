#ifndef CT_ABSTRACTGRID4D_H
#define CT_ABSTRACTGRID4D_H

// The grid is an itemDrawableWithoutPointCloud
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"

// Using QVector4D to store bounding box
#include <QVector4D>

// Using floor()
#include <math.h>

/*!
 * \class CT_AbstractGrid4D
 * \ingroup PluginShared_Items
 * \brief <b>Commun abstract base of all templated CT_AbstractGrid4D<type> </b>
 *
 * It's usefull to manage generically a grid, without knowing it template type
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractGrid4D : public CT_AbstractItemDrawableWithoutPointCloud
{
public:
//**********************************************//
//           Constructors/Destructors           //
//**********************************************//
    /*!
     * \brief CT_AbstractGrid4D
     *  Default constructor
     *  Each attribute will be set to 0, NULL or will be cleared
     */
    CT_AbstractGrid4D();

    /*!
     * \brief CT_AbstractGrid4D
     *
     * Initialisation constructor
     *
     * \param model Itemdrawable model attached to the grid
     * \param result Result attached to the grid
     */
    CT_AbstractGrid4D(const CT_OutAbstractSingularItemModel *model, CT_AbstractResult *result);

    CT_AbstractGrid4D(const QString &modelName, CT_AbstractResult *result);


    /*!
     * \brief ~CT_AbstractGrid4D
     * Destructor
     */
    virtual ~CT_AbstractGrid4D();

//**********************************************//
//                    Getters                   //
//**********************************************//

    /**
      * \brief Gives the number of w level
      * \return Number of w level
      */
    inline size_t wdim() const {return _dimw;}

    /**
      * \brief Gives the number of columns
      * \return Number of columns
      */
    inline size_t xdim() const {return _dimx;}

    /**
      * \brief Gives the number of rows
      * \return Number of rows
      */
    inline size_t ydim() const {return _dimy;}

    /**
      * \brief Gives the number of z levels
      * \return Number of z levels
      */
    inline size_t zdim() const {return _dimz;}

    /**
      * \brief Gives the length of a voxel along the w dimension w level
      * \return Lenght of a voxel along w dimension
      */
    inline float wres() const {return _resw;}

    /**
      * \brief Gives the length of a voxel along the x dimension x level
      * \return Lenght of a voxel along x dimension
      */
    inline float xres() const {return _resx;}

    /**
      * \brief Gives the length of a voxel along the y dimension y level
      * \return Lenght of a voxel along y dimension
      */
    inline float yres() const {return _resy;}

    /**
      * \brief Gives the length of a voxel along the z dimension z level
      * \return Lenght of a voxel along z dimension
      */
    inline float zres() const {return _resz;}

    /**
      * \brief Getter
      * \return the lower w coordinate of the grid
      */
    inline float minW() const {return _bot.w();}

    /**
      * \brief Getter
      * \return the upper w coordinate of the grid
      */
    inline float maxW() const {return _top.w();}

//**********************************************//
//              Getters Tools                   //
//**********************************************//
    /*!
     * \brief Total number of cells for the grid
     * \return Number of cells
     */
    inline size_t nCells() const {return _dimw*_dimx*_dimy*_dimz;}

    /*!
     * \brief Return a [0;1] value for any type (or -1 for NA)
     * \param index index in the grid
     * \return A float value between 0 (min value) and 1 (max value)
     */
    virtual float ratioValueAtIndex(const size_t index) const = 0;

    /**
     * \brief getMinCoordinates
     * \param min Min coordinates of the grid (bottom left corner)
     */
    inline void getMinCoordinates(QVector4D &min) const
    {
        min.setW( _bot.w() );
        min.setX( _bot.x() );
        min.setY( _bot.y() );
        min.setZ( _bot.z() );
    }

    /**
     * \brief getMaxCoordinates
     * \param max Max coordinates of the grid (upper right corner)
     */
    inline void getMaxCoordinates(QVector4D &max) const
    {
        max.setW( _top.w() );
        max.setX( _top.x() );
        max.setY( _top.y() );
        max.setZ( _top.z() );
    }

    /**
     * \brief getResolutions
     * \param res Resolutions of the grid (size of voxel along each axis)
     */
    inline void getResolutions(QVector4D &res) const
    {
        res.setW( _resw );
        res.setX( _resx );
        res.setY( _resy );
        res.setZ( _resz );
    }

    /**
     * \brief getDimensions
     * \param res Dimensions of the grid (size of voxel along each axis)
     */
    inline void getDimensions(QVector4D &dim) const
    {
        dim.setW( _dimw );
        dim.setX( _dimx );
        dim.setY( _dimy );
        dim.setZ( _dimz );
    }

    /*!
     * \brief Gives the W coordinate of the center of cells of level levw
     * \param levw Column, first one is 0
     * \return W coordinate
     */
    inline float getCellCenterW(const size_t levw) const
    {
        return _bot.w() + ((double)levw)*_resw + _resw/2;
    }

    /*!
     * \brief Gives the X coordinate of the center of cells of level levx
     * \param levx Column, first one is 0
     * \return X coordinate
     */
    inline float getCellCenterX(const size_t levx) const
    {
        return _bot.x() + ((double)levx)*_resx + _resx/2;
    }

    /*!
     * \brief Gives the Y coordinate of the center of cells of level levy
     * \param levy Column, first one is 0
     * \return Y coordinate
     */
    inline float getCellCenterY(const size_t levy) const
    {
        return _bot.y() + ((double)levy)*_resy + _resy/2;
    }

    /*!
     * \brief Gives the Z coordinate of the center of cells of level levz
     * \param levz Column, first one is 0
     * \return Z coordinate
     */
    inline float getCellCenterZ(const size_t levz) const
    {
        return _bot.z() + ((double)levz)*_resz + _resz/2;
    }

    /*!
     * \brief getCellCoordinates Give min and max coordinates of a cell
     * \param index Index of the cell
     * \param bottom  Min coordinates
     * \param top Max coordinates
     * \return true if index is in the grid
     */
    inline bool getCellCoordinates(const size_t index, QVector4D &bottom, QVector4D &top) const
    {
        size_t levw, levx, levy, levz;
        if( !indexToGrid(index, levw, levx, levy, levz) )
        {
            return false;
        }

        bottom.setW( _bot.w() + levw*_resw);
        bottom.setX( _bot.x() + levx*_resx);
        bottom.setY( _bot.y() + levy*_resy);
        bottom.setZ( _bot.z() + levz*_resz);

        top.setW(bottom.w() + _resw);
        top.setX(bottom.x() + _resx);
        top.setY(bottom.y() + _resy);
        top.setZ(bottom.z() + _resz);

        return true;
    }

    /*!
     * \brief Get the left corner coordinates of the cell defined by (levw, levx, levy, levz)
     * \param levw W level
     * \param levx X level
     * \param levy Y level
     * \param levz Z level
     * \param bottom Output coordinates
     * \return A QVector3D coordinates for the bottom left corner
     */
    inline bool getCellBottomLeftCorner(const size_t levw, const size_t levx, const size_t levy, const size_t levz, QVector4D &bottom) const
    {
        if( (levw >= _dimw) || (levx >= _dimx) || (levy >= _dimy) || (levz >= _dimz) )
        {
            return false;
        }

        bottom.setW( _bot.w() + levw*_resw);
        bottom.setX( _bot.x() + levx*_resx);
        bottom.setY( _bot.y() + levy*_resy);
        bottom.setZ( _bot.z() + levz*_resz);

        return true;
    }

    /*!
     * \brief Get the left corner coordinates of the cell containing (w, x, y, z)
     * \param w W coordinate
     * \param x X coordinate
     * \param y Y coordinate
     * \param z Z coordinate
     * \param bottom Output coordinates
     * \return A QVector3D coordinates for the bottom left corner
     */
    inline bool getCellBottomLeftCornerAtXYZ(const float w, const float x, const float y, const float z, QVector4D &bottom) const
    {

        return getCellBottomLeftCorner( (size_t) floor( (w - _bot.w() ) / _resw),
                                        (size_t) floor( (x - _bot.x() ) / _resx),
                                        (size_t) floor( (y - _bot.y() ) / _resy),
                                        (size_t) floor( (z - _bot.z() ) / _resz),
                                        bottom);
    }

//**********************************************//
//                   Setters                    //
//**********************************************//

//**********************************************//
//                  Setter Tools                //
//**********************************************//
    /*!
     * \brief Generic setter taking a double as input value
     * \param index Index of cell to modify
     * \param value Double value to cast in effective type
     */
    virtual void setValueAtIndexFromDouble(const size_t index, double value) = 0;

//**********************************************//
//                Generic Tools                 //
//**********************************************//
    /*!
     * \brief Compute min and max values
     */
    virtual void computeMinMax() = 0;

//**********************************************//
//          Coordinates transformation          //
//**********************************************//
    /*!
     * \brief Compute index for given levels on (w,x,y,z)
     * \param levw w level (0 if the first)
     * \param levx x level (0 if the first)
     * \param levy y level (0 if the first)
     * \param levz z level (0 if the first)
     * \param index Returned index
     * \return false if index is invalid
     */
    inline bool index(const size_t levw, const size_t levx, const size_t levy, const size_t levz, size_t &returnedIndex) const
    {
        if( (levw >= _dimw) || (levx >= _dimx) || (levy >= _dimy) || (levz >= _dimz) )
        {
            return false;
        }

        else
        {
            returnedIndex = levw*_dimx*_dimy*_dimz + levx*_dimy*_dimz + levy*_dimz + levz;
            return true;
        }
    }

    /*!
     * \brief Return column for specified w coordinate in cartesian system
     * \param w W coordinate
     * \param levw Returned level on W containing w
     * \return false if w invalid
     */
    inline bool levW(const float w, size_t &levw) const
    {
        if ( w < _bot.w() || w > _top.w() )
        {
            return false;
        }

        if ( w == _top.w() )
        {
            levw =  _dimw - 1;
        }

        else
        {
            levw = (size_t) floor( (w - _bot.w()) / _resw );
        }
        return true;
    }

    /*!
     * \brief Return column for specified x coordinate in cartesian system
     * \param x X coordinate
     * \param levx Returned level on X containing x
     * \return false if x invalid
     */
    inline bool levX(const float x, size_t &levx) const
    {
        if ( x < _bot.x() || x > _top.x() )
        {
            return false;
        }

        if ( x == _top.x() )
        {
            levx =  _dimx - 1;
        }

        else
        {
            levx = (size_t) floor( (x - _bot.x()) / _resx );
        }
        return true;
    }

    /*!
     * \brief Return column for specified y coordinate in cartesian system
     * \param y Y coordinate
     * \param levy Returned level on Y containing y
     * \return false if y invalid
     */
    inline bool levY(const float y, size_t &levy) const
    {
        if ( y < _bot.y() || y > _top.y() )
        {
            return false;
        }

        if ( y == _top.y() )
        {
            levy =  _dimy - 1;
        }

        else
        {
            levy = (size_t) floor( (y - _bot.y()) / _resy );
        }
        return true;
    }

    /*!
     * \brief Return column for specified z coordinate in cartesian system
     * \param z Z coordinate
     * \param levz Returned level on Z containing z
     * \return false if z invalid
     */
    inline bool levZ(const float z, size_t &levz) const
    {
        if ( z < _bot.z() || z > _top.z() )
        {
            return false;
        }

        if ( z == _top.z() )
        {
            levz =  _dimz - 1;
        }

        else
        {
            levz = (size_t) floor( (z - _bot.z()) / _resz );
        }
        return true;
    }

    /*!
     * \brief indexToGrid Convert index in (levw, levx, levy, levz) grid coordinates
     * \param index Index of the cell
     * \param levw Resulting level of the cell along w
     * \param levx Resulting level of the cell along x
     * \param levy Resulting level of the cell along y
     * \param levz Resulting level of the cell along z
     * \return true if the index is in the grid
     */
    inline bool indexToGrid(const size_t index, size_t &levw, size_t &levx, size_t &levy, size_t &levz) const
    {
        if ( index>=nCells() )
        {
            return false;
        }

        size_t mod;

        levw = index / (_dimx*_dimy*_dimz);
        mod = index % (_dimx*_dimy*_dimz);

        levx = mod / (_dimy*_dimz);
        mod = mod % (_dimy*_dimz);

        levy = mod / _dimz;
        mod = mod % _dimz;

        levz = mod;

        return true;
    }

    /*!
     * \brief Compute index for given (w, x, y, z) coordinate in cartesian system
     * \param w W coordinate
     * \param x X coordinate
     * \param y Y coordinate
     * \param z Z coordinate
     * \param index Returned index
     * \return false if index is invalid
     */
    inline bool indexAtWXYZ(const float w, const float x, const float y, const float z, size_t &returnedIndex) const
    {
        size_t levw, levx, levy, levz;
        if ( !levW(w, levw) || !levX(x, levx) || !levY(y, levy) || !levZ(z, levz) )
        {
            return false;
        }

        return index(levw, levx, levy, levz, returnedIndex);
    }

//**********************************************//
//          CompuTree Core and GUI tools        //
//**********************************************//
        /*!
         * \brief getType
         * \return A string describing the hierarchy of computree types until this class
         */
        inline virtual QString getType() const { return staticGetType(); }

        /*!
         * \brief staticGetType
         * Same as getType but static
         * \return A string describing the hierarchy of computree types until this class
         */
        inline static QString staticGetType() { return CT_AbstractItemDrawableWithoutPointCloud::staticGetType() + "/CT_AbstractGrid4D"; }

        /*!
         * \brief name
         * \return The name of the object
         */
        virtual QString name() const = 0;

protected:
//**********************************************//
//                  Attributes                  //
//**********************************************//
    QVector4D       _bot;       /*!< 4D Bounding box of the Grid*/
    QVector4D       _top;       /*!< 4D Bounding box of the Grid*/
    size_t          _dimw;      /*!< Nombre de cases selon w du grid*/
    size_t          _dimx;      /*!< Nombre de cases selon x du grid*/
    size_t          _dimy;      /*!< Nombre de cases selon y du grid*/
    size_t          _dimz;      /*!< Nombre de cases selon z du grid*/
    float           _resx;      /*!< Resolution de la grille (taille d'une case en x) */
    float           _resy;      /*!< Resolution de la grille (taille d'une case en y) */
    float           _resz;      /*!< Resolution de la grille (taille d'une case en z) */
    float           _resw;      /*!< Resolution de la grille (taille d'une case en r) */

};

#endif // CT_ABSTRACTGRID4D_H
