#ifndef CT_GRID4D_H
#define CT_GRID4D_H

#include "ct_itemdrawable/abstract/ct_abstractgrid4d.h"
#include "ct_itemdrawable/tools/ct_itemplateddata4darray.h"
#include <typeinfo>

//#include "ct_math/ct_math.h"
//#include <QMutex>

template< typename DataT > class CT_StandardGrid4DDrawManager;

/*!
 * \class CT_Grid4D
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable managing a 4D grid</b>
 *
 * It represents grid in 4D, with templated values in cells.
 */
template< typename DataT>
class   CT_Grid4D  : public CT_AbstractGrid4D, public CT_ITemplatedData4DArray<DataT>
{
    /*!
     *  \brief How to manage central cell when using neighboursValues method
     *
     */
    enum CenterMode
    {
        CM_KeepCenter = 1,      /*!< Gives neighbours and central value */
        CM_NAasCenter = 2,      /*!< Gives neighbours and NA for central value */
        CM_DropCenter = 3       /*!< Gives neighbours only */
    };



public:
    //**********************************************//
    //           Constructors/Destructors           //
    //**********************************************//
    /**
      * \brief Default constructor
      *  Each attribute will be set to 0, NULL or will be cleared
      */
    CT_Grid4D();

    /*!
     * \brief Initialisation constructor
     *
     * Grid is created thanks to bottom left point of bounding box (4D) and number of cells along each dimension.
     *
     * \param model Item model for creation
     * \param result Result containing the item
     * \param wmin Minimum W coordinate (bottom left corner)
     * \param xmin Minimum X coordinate (bottom left corner)
     * \param ymin Minimum Y coordinate (bottom left corner)
     * \param zmin Minimum Z coordinate (bottom left corner)
     * \param dimw Number of w levels
     * \param dimx Number of colums
     * \param dimy Number of rows
     * \param dimz Number of zlevels
     * \param resw Length of a cell on w
     * \param resx Length of a cell on x
     * \param resy Length of a cell on y
     * \param resz Length of a cell on z
     * \param na Value used to code NA
     * \param initValue Initialisation value for grid cells
     */
    CT_Grid4D(const CT_OutAbstractSingularItemModel *model,
              const CT_AbstractResult *result,
              double wmin,
              double xmin,
              double ymin,
              double zmin,
              size_t dimw,
              size_t dimx,
              size_t dimy,
              size_t dimz,
              double resw,
              double resx,
              double resy,
              double resz,
              DataT na,
              DataT initValue);

    CT_Grid4D(const QString &modelName,
              const CT_AbstractResult *result,
              double wmin,
              double xmin,
              double ymin,
              double zmin,
              size_t dimw,
              size_t dimx,
              size_t dimy,
              size_t dimz,
              double resw,
              double resx,
              double resy,
              double resz,
              DataT na,
              DataT initValue);

    /*!
     * \brief Initialisation constructor
     *
     * Grid is created thanks to the bounding box (4D) of the grid
     *
     * \param model Item model for creation
     * \param result Result containing the item
     * \param wmin Minimum W coordinate (bottom left corner)
     * \param xmin Minimum X coordinate (bottom left corner)
     * \param ymin Minimum Y coordinate (bottom left corner)
     * \param zmin Minimum Z coordinate (bottom left corner)
     * \param wmax Maximum W coordinate (top right corner)
     * \param xmax Maximum X coordinate (top right corner)
     * \param ymax Maximum Y coordinate (top right corner)
     * \param zmax Maximum Z coordinate (top right corner)
     * \param resw Length of a cell on w
     * \param resx Length of a cell on x
     * \param resy Length of a cell on y
     * \param resz Length of a cell on z
     * \param na Value used to code NA
     * \param initValue Initialisation value for grid cells
     * \param coordConstructor Not used, only to ensure constructor different signatures
     */
    CT_Grid4D(const CT_OutAbstractSingularItemModel *model,
              const CT_AbstractResult *result,
              double wmin,
              double xmin,
              double ymin,
              double zmin,
              double wmax,
              double xmax,
              double ymax,
              double zmax,
              double resw,
              double resx,
              double resy,
              double resz,
              DataT na,
              DataT initValue);

    /*!
     * \brief Initialisation constructor
     *
     * Grid is created thanks to the bounding box (4D) of the grid
     *
     * \param modelName model name for creation
     * \param result Result containing the item
     * \param wmin Minimum W coordinate (bottom left corner)
     * \param xmin Minimum X coordinate (bottom left corner)
     * \param ymin Minimum Y coordinate (bottom left corner)
     * \param zmin Minimum Z coordinate (bottom left corner)
     * \param wmax Maximum W coordinate (top right corner)
     * \param xmax Maximum X coordinate (top right corner)
     * \param ymax Maximum Y coordinate (top right corner)
     * \param zmax Maximum Z coordinate (top right corner)
     * \param resw Length of a cell on w
     * \param resx Length of a cell on x
     * \param resy Length of a cell on y
     * \param resz Length of a cell on z
     * \param na Value used to code NA
     * \param initValue Initialisation value for grid cells
     * \param coordConstructor Not used, only to ensure constructor different signatures
     */
    CT_Grid4D(const QString& modelName,
              const CT_AbstractResult *result,
              double wmin,
              double xmin,
              double ymin,
              double zmin,
              double wmax,
              double xmax,
              double ymax,
              double zmax,
              double resw,
              double resx,
              double resy,
              double resz,
              DataT na,
              DataT initValue);


    /*!
     * \brief Factory
     *
     * Grid is created thanks to the bounding box (4D) of the grid
     *
     * \param model Item model for creation
     * \param result Result containing the item
     * \param wmin Minimum W coordinate (bottom left corner)
     * \param xmin Minimum X coordinate (bottom left corner)
     * \param ymin Minimum Y coordinate (bottom left corner)
     * \param zmin Minimum Z coordinate (bottom left corner)
     * \param wmax Maximum W coordinate (top right corner)
     * \param xmax Maximum X coordinate (top right corner)
     * \param ymax Maximum Y coordinate (top right corner)
     * \param zmax Maximum Z coordinate (top right corner)
     * \param resw Length of a cell on w
     * \param resx Length of a cell on x
     * \param resy Length of a cell on y
     * \param resz Length of a cell on z
     * \param na Value used to code NA
     * \param initValue Initialisation value for grid cells
     * \param coordConstructor Not used, only to ensure constructor different signatures
     */
    static CT_Grid4D<DataT>* createGrid4DFromWXYZCoords(const CT_OutAbstractSingularItemModel *model,
                                                        const CT_AbstractResult *result,
                                                        double wmin,
                                                        double xmin,
                                                        double ymin,
                                                        double zmin,
                                                        double wmax,
                                                        double xmax,
                                                        double ymax,
                                                        double zmax,
                                                        double resw,
                                                        double resx,
                                                        double resy,
                                                        double resz,
                                                        DataT na,
                                                        DataT initValue);

    static CT_Grid4D<DataT>* createGrid4DFromWXYZCoords(const QString &modelName,
                                                        const CT_AbstractResult *result,
                                                        double wmin,
                                                        double xmin,
                                                        double ymin,
                                                        double zmin,
                                                        double wmax,
                                                        double xmax,
                                                        double ymax,
                                                        double zmax,
                                                        double resw,
                                                        double resx,
                                                        double resy,
                                                        double resz,
                                                        DataT na,
                                                        DataT initValue);

    /*!
     * \brief Destructor
     */
    virtual ~CT_Grid4D();

    //**********************************************//
    //              Constructor Tools               //
    //**********************************************//
    /*!
     * \brief Initialize all grid cells values with val
     *
     * Used by Constructors.
     *
     * \param val
     */
    void initGridWithValue(const DataT val);

    //**********************************************//
    //                    Getters                   //
    //**********************************************//
    /**
      * \brief Gives the NA value
      * \return Valeur NA value
      */
    inline DataT NA() const {return _NAdata;}

    /*!
     * \brief Gives the max value of the grid
     * \return Max value
     */
    inline DataT dataMax() const { return _dataMax; }

    /*!
     * \brief Gives the min value of the grid
     * \return Min value
     */
    inline DataT dataMin() const { return _dataMin; }

    //**********************************************//
    //                  Getters Tools               //
    //**********************************************//
    /*!
     * \brief ives the value at specified index
     * \param index Index
     * \return Value
     */
    inline DataT valueAtIndex(const size_t index) const
    {
        if ( index >= nCells() )
        {
            return NA();
        }

        return _data[index];
    }

    /*!
     * \brief Generic [0;1]  (or -1 for NA) value accessor for use as CT_AbstractGrid4D
     * \param index index in the grid
     * \return A double value between 0 (min value) and 1 (max value), or -1 for NA : (valueAt(i)-dataMin) / (dataMax-dataMin)
     */
    virtual double ratioValueAtIndex(const size_t index) const;

    /*!
     * \brief Gives the value at (levw, levx, levy, levz)
     * \param levw w level (0 is the first)
     * \param levx x level (0 is the first)
     * \param levy y level (0 is the first)
     * \param levz z level (0 is the first)
     * \return Value at (levw, levx, levy, levz)
     */
    DataT value(const size_t levw, const size_t levx, const size_t levy, const size_t levz) const;

    /**
      * \brief Gives the value of the cell containing (w,x,y,z) coordinate (in cartesian space)
      * \param w W coordinate
      * \param x X coordinate
      * \param y Y coordinate
      * \param z Z coordinate
      * \return Value of the cell containing (w,x,y,z) coordinate (in cartesian space)
      */
    DataT valueAtWXYZ(const double w, const double x, const double y, const double z) const;

    //    /**
    //      * \brief Set the value for the cell at specified index, if value is superior to actual one
    //      * \param index Index
    //      * \param value Value
    //      * \return True if the value has actually been set
    //      */
    //    bool setMaxValueAtIndex(const size_t index, const DataT value);

    //    /**
    //      * \brief Set the value for the cell containing (x,y,z) coordinate to value, if value is superior to actual one
    //      * \param x X coordinate
    //      * \param y Y coordinate
    //      * \param z Z coordinate
    //      * \param value Value
    //      * \return True if the value has actually been set
    //      */
    //    bool setMaxValueAtXYZ(const double x, const double y, const double z, const DataT value);

    //    /**
    //      * \brief Set the value for the cell at specified index, if value is inferior to actual one
    //      * \param index Index
    //      * \param value Value
    //      * \return True if the value has actually been set
    //      */
    //    bool setMinValueAtIndex(const size_t index, const DataT value);

    //    /**
    //      * \brief Set the value for the cell containing (x,y,z) coordinate to value, if value is inferior to actual one
    //      * \param x X coordinate
    //      * \param y Y coordinate
    //      * \param z Z coordinate
    //      * \param value Value
    //      * \return True if the value has actually been set
    //      */
    //    bool setMinValueAtXYZ(const double x, const double y, const double z, const DataT value);

    /**
      * \brief Add value to the cell at the specified index
      * \param index Index
      * \param value Value
      * \return True if the value has actually been set
      */
    bool addValueAtIndex(const size_t index, const DataT value);

    /**
      * \brief Add value to the cell containing (w,x,y,z) coordinate in cartesian space
      * \param w W coordinate
      * \param x X coordinate
      * \param y Y coordinate
      * \param z Z coordinate
      * \param value Value
      * \return True if the value has actually been updated
      */
    bool addValueAtWXYZ(const double w, const double x, const double y, const double z, const DataT value);


    bool addValue( size_t levw, size_t levx, size_t levy, size_t levz, DataT value );

    /**
      * \brief Gives neighbours values
      * \param levw w level (first is 0)
      * \param levx x level (first is 0)
      * \param levy y level (first is 0)
      * \param levz z level (first is 0)
      * \param distance : Distance for neighbours search in cells
      * \param outNeighboursValues : Values in the neighbourhood of (levw, levx, levy, levz)
      * \param keepNAs : Should the NA values be kept ?
      * \param centermode : How should be treated the central cell ?
      * \return False if distance was less than 1 or the cell was outside the grid
      */
    bool neighboursValues(const size_t levw,
                          const size_t levx,
                          const size_t levy,
                          const size_t levz,
                          const size_t distance,
                          QList<DataT>& outNeighboursValues,
                          const bool keepNAs = false,
                          const CenterMode centermode = CT_Grid4D::CM_DropCenter) const;

    QString valueAtIndexAsString(const size_t index) const;

    QString NAAsString() const;

    //**********************************************//
    //                    Setters                   //
    //**********************************************//

    //**********************************************//
    //                  Setters Tools               //
    //**********************************************//
    /*!
     * \brief Generic setter taking a double as input value
     * \param index Index of cell to modify
     * \param value Double value to cast in effective type
     */
    inline virtual void setValueAtIndexFromDouble(const size_t &index, const double &value) { setValueAtIndex(index, (DataT) value); }

    /*!
     * \brief Set value at specified index
     * \param index Index
     * \param value Value
     * \return True if the value has actually been set
     */
    inline bool setValueAtIndex(const size_t index, const DataT value)
    {
        if ( index >= nCells() )
        {
            return false;
        }

        _data[index] = value;

        return true;
    }

    /*!
     * \brief setValue
     * \param levw w level, first one is 0
     * \param levx x level, first one is 0
     * \param levy y level, first one is 0
     * \param levz z level, first one is 0
     * \param value Value
     * \return True if the value has actually been set
     */
    inline bool setValue(const size_t levw, const size_t levx, const size_t levy, const size_t levz, DataT value)
    {
        size_t i;
        if( index(levw, levx, levy, levz, i) )
        {
            return setValueAtIndex(i, value);
        }
        return false;
    }

    /*!
     * \brief Set the value for the cell containing (w,x,y,z) coordinate to value
     * \param w W coordinate
     * \param x X coordinate
     * \param y Y coordinate
     * \param z Z coordinate
     * \param value Value
     * \return True if the value has actually been set
     */
    inline bool setValueAtWXYZ(const double w, const double x, const double y, const double z, const DataT value)
    {
        size_t i;
        if( indexAtWXYZ(w, x, y, z, i) )
        {
            return setValueAtIndex(i, value);
        }
        return false;
    }

    //**********************************************//
    //    Getters from CT_ITemplatedData4DArray     //
    //**********************************************//
    /*!
     * \brief Same as value(w,x,y,z). Redefined Since grid4d heritates from CT_ITemplatedData4DArray
     * \param w w level (0 is first)
     * \param x x level (0 is first)
     * \param y y level (0 is first)
     * \param z z level (0 is first)
     * \return Value at (w,x,y,z)
     */
    DataT dataFromArray(const size_t &w, const size_t &x, const size_t &y, const size_t &z) const;

    /*!
     * \brief Same as valueAtIndex. Redefined Since grid4d heritates from CT_ITemplatedData4DArray
     * \param index Index
     * \return Value at index
     */
    DataT dataFromArray(const size_t &index) const;

    //**********************************************//
    //    Setters from CT_ITemplatedData4DArray     //
    //**********************************************//
    /*!
     * \brief wArraySize
     * Gives the number of cells along the w axis
     * \return the number of cells along the w axis
     */
    inline size_t wArraySize() const { return wdim(); }

    /*!
     * \brief xArraySize
     * Gives the number of cells along the x axis
     * \return the number of cells along the x axis
     */
    inline size_t xArraySize() const { return xdim(); }

    /*!
     * \brief yArraySize
     * Gives the number of cells along the y axis
     * \return the number of cells along the y axis
     */
    inline size_t yArraySize() const { return ydim(); }

    /*!
     * \brief zArraySize
     * Gives the number of cells along the z axis
     * \return the number of cells along the z axis
     */
    inline size_t zArraySize() const { return zdim(); }

    //**********************************************//
    //          CompuTree Core and GUI tools        //
    //**********************************************//
    /*!
     * \brief getType
     * \return A string describing the hierarchy of computree types and the type of grid (4dgrid and type of data in it)
     */
    inline virtual QString getType() const { return staticGetType(); }

    /*!
     * \brief staticGetType
     *
     * Same as getType but static
     *
     * \return A string describing the hierarchy of computree types and the type of the grid (4dgrid and type of data in it)
     */
    inline static QString staticGetType() { return CT_AbstractGrid4D::staticGetType() + "/CT_Grid4D<" + CT_TypeInfo::name<DataT>() + QString(">"); }

    /*!
     * \brief name
     * \return The name of the object (4dgrid and type of data in it)
     */
    inline virtual QString name() const { return QString("CT_Grid4D<") + CT_TypeInfo::name<DataT>() + QString(">"); }

    /*!
     * \brief Copy method
     *
     * \param model Item model for the copy
     * \param result Result containing the copy
     * \param copyModeList Copy mode
     * \return Item copy
     */
    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
    virtual CT_AbstractItemDrawable* copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    inline const CT_ITemplatedData4DArray<DataT>* iTemplatedData4DArray()
    const { return this; }

    /*!
     * \brief Compute min and max values
     */
    void computeMinMax();

protected:

    const static    CT_StandardGrid4DDrawManager<DataT> ABSGRID4D_DRAW_MANAGER;

    // -------------------------------------------------------
    // Attributes
    // -------------------------------------------------------
    DataT       _NAdata;            /*!< Valeur codant NA */
    DataT       _dataMax;           /*!< valeur maximale du grid*/
    DataT       _dataMin;           /*!< valeur minimale du grid*/
    std::vector<DataT> _data;       /*!< Tableau contenant les donnees pour chaque case de la grille*/

private:
    CT_DEFAULT_IA_BEGIN(CT_Grid4D<DataT>)
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataDimension(), &CT_Grid4D<DataT>::wdim, QObject::tr("W dimension"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataXDimension(), &CT_Grid4D<DataT>::xdim, QObject::tr("X dimension"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataYDimension(), &CT_Grid4D<DataT>::ydim, QObject::tr("Y dimension"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataZDimension(), &CT_Grid4D<DataT>::zdim, QObject::tr("Z dimension"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataCoordinate(), &CT_Grid4D<DataT>::minW, QObject::tr("W min"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataX(), &CT_Grid4D<DataT>::minX, QObject::tr("X min"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataY(), &CT_Grid4D<DataT>::minY, QObject::tr("Y min"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataZ(), &CT_Grid4D<DataT>::minZ, QObject::tr("Z min"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataResolution(), &CT_Grid4D<DataT>::wres, QObject::tr("Resolution W"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataXResolution(), &CT_Grid4D<DataT>::xres, QObject::tr("Resolution X"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataYResolution(), &CT_Grid4D<DataT>::yres, QObject::tr("Resolution Y"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataZResolution(), &CT_Grid4D<DataT>::zres, QObject::tr("Resolution Z"))
    CT_DEFAULT_IA_V2(CT_Grid4D<DataT>, CT_AbstractCategory::staticInitDataNa(), &CT_Grid4D<DataT>::NA, QObject::tr("NA"))
    CT_DEFAULT_IA_END(CT_Grid4D<DataT>)
};

// Includes the template implementations
#include "ct_itemdrawable/ct_grid4d.hpp"

#endif // CT_GRID4D_H
