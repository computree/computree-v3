#ifndef CT_STEPSMENU_H
#define CT_STEPSMENU_H

#include "pluginShared_global.h"

#include <QMap>
#include <QObject>

class CT_MenuLevel;

/**
 * @brief Class to construct a menu of steps
 */
class PLUGINSHAREDSHARED_EXPORT CT_StepsMenu : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief The root level must be one of this type of operation
     */
    enum LevelOperationType {
        LO_Favorites,       // Favorites Step. Must not be used by plugins ! Only by gui or core
        LO_Load,                // Steps that load/import files
        LO_Export,              // Steps that exports files
        LO_WorkFlow,            // Steps that can control the worklow of all other steps
        LO_Points,              // Steps that produce points clouds / clusters
        LO_GeometricalShapes,   // Steps that produce 3D geometrical shapes
        LO_Voxels,              // Steps that produce voxels
        LO_Shapes2D,            // Steps that produce 2D geometrical shapes
        LO_Rasters,             // Steps that produce rasters
        LO_Meshes,              // Steps that produce meshes
        LO_Metrics,             // Steps that produce metrics
        LO_Other,               // Steps that can not be ordered in one of other operation
        LO_Beta,                // Steps that are in Beta(work in progress)
        LO_InternalUse
    };
    Q_ENUMS(LevelOperationType)

    /**
     * @brief Other levels can use this predefined values to be sorted in the same level for all plugins
     */
    enum LevelPredefined {
        LP_Filter,
        LP_Create,
        LP_Extract,
        LP_Detect,
        LP_Transform,
        LP_Classify,
        LP_Analyze,
        LP_Fit,
        LP_Colorize,
        LP_Clusters,
        LP_Loops,
        LP_Plots,
        LP_Footprints,
        LP_DEM,
        LP_Crowns,
        LP_Stems,
        LP_Points,
        LP_Items,
        LP_Raster,
        LP_Vector,
        LP_Voxels,
        LP_Meshes,
        LP_ItemAttributes,
        LP_Others
    };
    Q_ENUMS(LevelPredefined)

    CT_StepsMenu();
    ~CT_StepsMenu();

    /**
     * @brief Returns the root level that correspond to the operation passed in parameter
     *        and create it if it does not already exist.
     */
    CT_MenuLevel* getOrCreateRootLevel(LevelOperationType operation);

    /**
     * @brief Returns the existing level that correspond to operation passed in parameter. NULL
     *        if it does not exist.
     */
    CT_MenuLevel* levelFromOperation(LevelOperationType operation) const;

    /**
     * @brief Returns all levels of this menu
     */
    QList<CT_MenuLevel*> levels() const;

    /**
     * @brief Remove a level that was in favorites menu (the level and it's sub level will be deleted from memory)
     */
    bool removeLevelFromFavorites(CT_MenuLevel *level);

    /**
     * @brief Returns a displayable name for operation passed in parameter
     */
    static QString staticOperationToDisplayableName(LevelOperationType operation);

    /**
     * @brief Convert a predefined enum to displayable name
     */
    static QString staticPredefinedToString(LevelPredefined predefined);

    /**
     * @brief Returns true if the name is a predefined string for a level
     */
    static bool staticIsPredefinedLevelString(const QString &name);

private:
    QMap<LevelOperationType, CT_MenuLevel*>     m_rootLevels;
};

#endif // CT_STEPSMENU_H
