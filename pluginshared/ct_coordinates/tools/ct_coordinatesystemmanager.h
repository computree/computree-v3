#ifndef CT_COORDINATESYSTEMMANAGER_H
#define CT_COORDINATESYSTEMMANAGER_H

#include "ct_coordinates/ct_defaultcoordinatesystem.h"

#include <QList>

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
     * @brief Add a new coordinate system to this manager
     */
    void addCoordinateSystem(CT_AbstractCoordinateSystem *cs);

    /**
     * @brief Remove the coordinate system passed in parameter
     * @return false if it is the current system and it can not be changed or if it was the default system.
     */
    bool removeCoordinateSystem(CT_AbstractCoordinateSystem *cs);

    /**
     * @brief Remove the coordinate system at 'index'
     * @return false if it is the current system and it can not be changed or if index == 0.
     */
    bool removeCoordinateSystem(int index);

    /**
     * @brief Returns the number of coordinate system installed. At least one coordinate system
     *        of type CT_DefaultCoordinateSystem is present in this manager.
     */
    int size() const;

    /**
     * @brief Returns the coordinate system at 'index'
     */
    CT_AbstractCoordinateSystem* coordinateSystemAt(int index) const;

    /**
     * @brief Set the passed coordinate system the current used.
     * @return false if the current coordinate system can not be changed
     */
    bool setCurrentCoordinateSystem(CT_AbstractCoordinateSystem *cs);

    /**
     * @brief Set the coordinate system at 'index' the current used.
     * @return false if the current coordinate system can not be changed
     */
    bool setCurrentCoordinateSystem(int index);

    /**
     * @brief Returns the current coordinate system manager. Can not be NULL.
     */
    CT_AbstractCoordinateSystem* currentCoordinateSystem() const;

    /**
     * @brief Returns true if we can change or modify a coordinate system
     */
    static bool staticCanChangeOrModifyCoordinateSystem();

private:
    QList<CT_AbstractCoordinateSystem*>     m_cs;
    CT_AbstractCoordinateSystem             *m_current;

protected:
    friend class CT_AbstractReader;
    friend class CT_AbstractExporter;

    /**
     * @brief Called by CT_AbstractReader and CT_AbstractExporter to call initUsed() of all coordinate system
     */
    void initUsedOfAllCoordinateSystem();
};

#endif // CT_COORDINATESYSTEMMANAGER_H
