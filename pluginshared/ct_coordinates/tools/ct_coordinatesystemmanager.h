#ifndef CT_COORDINATESYSTEMMANAGER_H
#define CT_COORDINATESYSTEMMANAGER_H

#include "ct_coordinates/abstract/ct_abstractcoordinatesystem.h"
#include "ct_coordinates/tools/ct_coordinatesystemcloudindex.h"
#include "ct_cloud/registered/ct_stdcloudregisteredt.h"
#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h"

#include <QVector>
#include <QSharedPointer>

/**
 * @brief Manage available coordinate system. At least one coordinate system
 * is present in this manager (the CT_DefaultCoordinateSystem)
 */
class PLUGINSHAREDSHARED_EXPORT CT_CoordinateSystemManager
{
public:
    CT_CoordinateSystemManager();
    ~CT_CoordinateSystemManager();

    /**
     * @brief Register a new coordinate system to this manager and set it to be the current. The result is a QSharedPointer that must
     *        be passed to all your scene that will use points that use this coordinate system.
     *
     *        All points created in the global points cloud will automatically had the index of the current coordinate system.
     */
    CT_CSR registerCoordinateSystem(CT_AbstractCoordinateSystem *cs);

    /**
     * @brief Returns the index cloud that contains for each points the index of the coordinate system used. You can modify
     *        this cloud but be careful not to put an invalid value.
     *
     * @warning Don't add or remove index ! just replace index.
     */
    CT_CoordinateSystemCloudIndex* indexCloudOfCoordinateSystemOfPoints() const;

    /**
     * @brief Returns the number of coordinate system installed. At least one coordinate system
     *        of type CT_DefaultCoordinateSystem is present in this manager.
     */
    int size() const;

    /**
     * @brief Returns the index of the coordinate system passed in parameter
     */
    GLuint indexOfCoordinateSystem(const CT_AbstractCoordinateSystem *cs) const;

    /**
     * @brief Returns the coordinate system at 'index'
     */
    CT_AbstractCoordinateSystem* coordinateSystemAt(const GLuint &index) const;

    /**
     * @brief Returns the coordinate system for the point at index 'globalIndex' in the global points cloud
     */
    CT_AbstractCoordinateSystem* coordinateSystemForPointAt(const size_t &globalIndex) const;

    /**
     * @brief Returns the coordinate system index for the point at index 'globalIndex' in the global points cloud
     */
    GLuint coordinateSystemIndexForPointAt(const size_t &globalIndex) const;

    /**
     * @brief Change the index of the coordinate system of the point at index 'globalIndex' in the global points cloud
     */
    void setCoordinateSystemForPointAt(const size_t &globalIndex, const GLuint &coordinateSystemIndex) const;

    /**
     * @brief Use this method to change indexes (in the CT_CoordinateSystemCloudIndex) for all points in pcir by the index of the coordinate system "cs"
     */
    void setCoordinateSystemForPoints(CT_PCIR pcir, CT_AbstractCoordinateSystem *cs);

    /**
     * @brief Use this method to change indexes (in the CT_CoordinateSystemCloudIndex) for all points in pcir by the index of the coordinate system "cs"
     */
    void setCoordinateSystemIndexForPoints(CT_PCIR pcir, const GLuint &coordinateSystemIndex);

    /**
     * @brief Returns the current coordinate system manager. Can not be NULL.
     */
    CT_AbstractCoordinateSystem* currentCoordinateSystem() const;

    /**
     * @brief Returns true if at least one coordinate system was used since the protected method "initUsedOfAllCoordinateSystem" was called
     */
    bool wasAtLeastOneUsed() const;

    /**
     * @brief Set the passed coordinate system the current used.
     */
    void setCurrentCoordinateSystem(CT_AbstractCoordinateSystem *cs);

    /**
     * @brief Set the coordinate system at 'index' the current used.
     */
    void setCurrentCoordinateSystem(int index);

private:
    QVector<CT_AbstractCoordinateSystem*>                                       m_cs;                                   // list of coordinate system
    CT_AbstractCoordinateSystem                                                 *m_current;                             // current coordinate system (never NULL)
    QSharedPointer< CT_StdCloudRegisteredT< CT_CoordinateSystemCloudIndex > >   m_indexOfCoordinateSystemOfPoints;      // index for all points (sync) of the coordinate system to use
    CT_CSR                                                                      m_default;


    /**
     * @brief Remove the coordinate system passed in parameter
     * @return false if it was the first coordinate system (default)
     */
    bool removeCoordinateSystem(CT_AbstractCoordinateSystem *cs);

    /**
     * @brief Remove the coordinate system at 'index'
     * @return false if index == 0.
     */
    bool removeCoordinateSystem(int index);

    /**
     * @brief Called when a coordinate system will no longer be used
     */
    static void staticUnregisterCoordinateSystem(CT_AbstractCoordinateSystem *cs);

protected:
    friend class CT_AbstractReader;
    friend class CT_AbstractExporter;

    /**
     * @brief Called by CT_AbstractReader and CT_AbstractExporter to call initUsed() of all coordinate system
     */
    void initUsedOfAllCoordinateSystem();
};

#endif // CT_COORDINATESYSTEMMANAGER_H
