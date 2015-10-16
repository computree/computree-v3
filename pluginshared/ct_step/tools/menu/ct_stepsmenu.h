#ifndef CT_STEPSMENU_H
#define CT_STEPSMENU_H

#include "pluginShared_global.h"

#include <QMap>

class CT_MenuLevel;

/**
 * @brief Class to construct a menu of steps
 */
class PLUGINSHAREDSHARED_EXPORT CT_StepsMenu
{
public:

    /**
     * @brief The root level must be one of this type of operation
     */
    enum LevelOperationType {
        LO_Favorites,       // Favorites Step. Must not be used by plugins ! Only by gui or core
        LO_Load,                // Steps that load/import files
        LO_Export,              // Steps that exports files
        LO_Create,              // Steps that create something new from scratch
        LO_Filter,              // Steps that filter/reduce/remove something
        LO_Extract,             // Steps that extract something from a result
        LO_GeometryTransform,   // Steps that modify the gometry of elements
        LO_Analyze,             // Steps that analyze and produce metrics or compare elements per example
        LO_Detect,              // Steps that detect elements
        LO_WorkFlow,            // Steps that can control the worklow of all other steps
        LO_Other,               // Steps that can not be ordered in one of other operation
        LO_InternalUse
    };

    /**
     * @brief Other levels can use this predefined values to be sorted in the same level for all plugins
     */
    enum LevelPredefined {
        LP_Rotate,
        LP_Translate,
        LP_Scale,
        LP_Colorize,
        LP_Triangulate,
        LP_Smooth,
        LP_Deforme,
        LP_Repair,
        LP_Simplify,
        LP_Texturize,
        LP_Beam,
        LP_Box,
        LP_Circle,
        LP_Cluster,
        LP_Cylinder,
        LP_DataSource,
        LP_DataSourceGeo,
        LP_EdgeAttributes,
        LP_Ellipse,
        LP_FaceAttributes,
        LP_Grid,
        LP_Image,
        LP_Line,
        LP_Mesh,
        LP_BSpline,
        LP_Point,
        LP_PointCloud,
        LP_PointAttributes,
        LP_Polygon,
        LP_Polyline,
        LP_Profile,
        LP_Scanner,
        LP_Sphere,
        LP_Normal,
        LP_ItemAttributes
    };

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

private:
    QMap<LevelOperationType, CT_MenuLevel*>     m_rootLevels;
};

#endif // CT_STEPSMENU_H
