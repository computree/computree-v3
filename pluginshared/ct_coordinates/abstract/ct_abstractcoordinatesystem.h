#ifndef CT_ABSTRACTCOORDINATESYSTEM_H
#define CT_ABSTRACTCOORDINATESYSTEM_H

#include "pluginShared_global.h"

/**
 * @brief Abstract class that define the interface of a Coordinate System class. A
 *        Coordinate System class can be a projection library per example.
 *
 *        A Coordinate System class must offer the possibility of :
 *
 *        - convert coordinates of type realEx to importable coordinates (realIm)
 *        - convert coordinates of type realIm to exportable coordinates (realEx)
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractCoordinateSystem
{
public:

    /**
     * @brief Type of real importable
     */
    typedef float           realIm;

    /**
     * @brief Type of real exportable
     */
    typedef long double     realEx;

    virtual ~CT_AbstractCoordinateSystem() {}

    /**
     * @brief Returns true if coordinates passed in parameter can be converted to realIm (import). False
     *        if the capacity of a realIm is not sufficient.
     */
    virtual bool canConvertImport(const CT_AbstractCoordinateSystem::realEx &x,
                                  const CT_AbstractCoordinateSystem::realEx &y,
                                  const CT_AbstractCoordinateSystem::realEx &z) const = 0;

    /**
     * @brief Convert a coordinate of type realEx to realIm. This method don't test if the realIm
     *        can accept the capacity of the converted value (if you want to know if its possible call
     *        the method "canConvertedForImport")
     */
    virtual void convertImport(const CT_AbstractCoordinateSystem::realEx &x,
                               const CT_AbstractCoordinateSystem::realEx &y,
                               const CT_AbstractCoordinateSystem::realEx &z,
                               CT_AbstractCoordinateSystem::realIm &xOut,
                               CT_AbstractCoordinateSystem::realIm &yOut,
                               CT_AbstractCoordinateSystem::realIm &zOut) const = 0;

    /**
     * @brief Convert a coordinate of type realIm to realEx.
     */
    virtual void convertExport(const CT_AbstractCoordinateSystem::realIm &x,
                               const CT_AbstractCoordinateSystem::realIm &y,
                               const CT_AbstractCoordinateSystem::realIm &z,
                               CT_AbstractCoordinateSystem::realEx &xOut,
                               CT_AbstractCoordinateSystem::realEx &yOut,
                               CT_AbstractCoordinateSystem::realEx &zOut) const = 0;

    /**
     * @brief Configure the coordinate system.
     * @return false if the configuration was canceled.
     */
    virtual bool configure() = 0;

private:
    mutable bool        m_used;

protected:
    friend class CT_AbstractReader;
    friend class CT_AbstractExporter;
    friend class CT_CoordinateSystemManager;

    /**
     * @brief Called by CT_AbstractReader and CT_AbstractExporter to set m_used to false
     */
    void initUsed() { m_used = false; }

    /**
     * @brief Must be called from derived class in method "convertImport" and "convertExport" to inform that this
     *        coordinate system has been used. Set m_used to true.
     */
    void informThatUsed() const { m_used = true; }

    /**
     * @brief Returns true if the coordinate system was used
     */
    bool wasUsed() const { return m_used; }
};

#endif // CT_ABSTRACTCOORDINATESYSTEM_H
