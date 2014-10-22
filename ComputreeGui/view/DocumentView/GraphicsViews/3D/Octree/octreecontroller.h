#ifndef OCTREECONTROLLER_H
#define OCTREECONTROLLER_H

#include <QList>
#include <QVector3D>

#include "view/DocumentView/GraphicsViews/3D/Octree/octree.h"

#include "ct_global/ct_context.h"

class CT_AbstractPointCloudIndex;

/**
 * @brief Class that control the generic octree
 */
class OctreeController : public OctreeInterface
{
    Q_INTERFACES(OctreeInterface)
    Q_OBJECT

public:
    OctreeController();
    ~OctreeController();

    /**
     * @brief Set the number of cells between min and max
     */
    void setNumberOfCells(const int &s);

    /**
     * @brief Returns number of cells between min and max
     */
    int numberOfCells() const;

    /**
     * @brief Add points to the octree. After add points you must call "mustBeReconstructed" to know
     *        if you must call "construct" to reconstruct the octree
     * @param min : NULL if you want than min and max value was calculated otherwise pass a pointer to the min corner
     * @param max : NULL if you want than min and max value was calculated otherwise pass a pointer to the max corner
     */
    void addPoints(const CT_AbstractPointCloudIndex *index, QVector3D *min = NULL, QVector3D *max = NULL);

    /**
     * @brief Remove points from the octree.
     */
    void removePoints(const CT_AbstractPointCloudIndex *index);

    /**
     * @brief Get the index of the point in the octree
     */
    void indexOfPoint(const CT_Point &point, int &x, int &y, int &z);

    /**
     * @brief Returns true if the octree has points
     */
    bool hasElements() const;

    /**
     * @brief Returns true if the octree has points in the cell at x/y/z
     */
    bool hasElementsAt(int x, int y, int z) const;

    /**
     * @brief Returns point cloud index in cell at x/y/z. NULL if cell is empty.
     */
    const CT_AbstractCloudIndexT<CT_Point>* pointsAt(int x, int y, int z) const;
    const CT_AbstractCloudIndex* at(int x, int y, int z) const;

    /**
     * @brief Returns true if the cell at x, y, z is visible in frustrum
     */
    bool isCellVisibleInFrustrum(int x, int y, int z, GLdouble planeCoefficients[6][4]) const;

    /**
     * @brief Returns the cells size
     */
    double cellsSize() const;

    /**
     * @brief The min corner of the octree
     */
    QVector3D octreeMinCorner() const;

    /**
     * @brief The max corner of the octree
     */
    QVector3D octreeMaxCorner() const;

    /**
     * @brief Returns true if the octree must be reconstructed. (If new min or max is greather or lower than old min or max OR if number of cells has changed)
     */
    bool mustBeReconstructed() const;

    /**
     * @brief Returns true if at least one corner of the octree has changed
     */
    bool cornersChanged() const;

    /**
     * @brief Clear the octree (remove all points)
     */
    void clear();

public slots:

    /**
     * @brief (Re)construct the octree
     */
    void construct();

private:
    class Corner {
    public:
        Corner() { m_x = 0; m_y = 0; m_z = 0;}
        Corner(double x, double y, double z) { m_x = x; m_y = y; m_z = z; }

        bool operator<(const Corner &o) const { return ((m_x < o.m_x) && (m_y < o.m_y) && (m_z < o.m_z)); }
        bool operator>(const Corner &o) const { return ((m_x > o.m_x) && (m_y > o.m_y) && (m_z > o.m_z)); }
        bool operator!=(const Corner &o) const { return ((o.m_x != m_x) || (o.m_y != m_y) || (o.m_z != m_z)); }
        bool operator==(const Corner &o) const { return ((o.m_x == m_x) && (o.m_y == m_y) && (o.m_z == m_z)); }
        void operator=(const QVector3D &o) { m_x = o.x(); m_y = o.y(); m_z = o.z(); }

        inline void setX(double x) { m_x = x; }
        inline void setY(double y) { m_y = y; }
        inline void setZ(double z) { m_z = z; }

        inline double x() const { return m_x; }
        inline double y() const { return m_y; }
        inline double z() const { return m_z; }

    private:
        double m_x;
        double m_y;
        double m_z;
    };

    class PointsInfo {
    public:
        PointsInfo() {}
        Corner   m_min;
        Corner   m_max;
    };

    QHash<CT_AbstractPointCloudIndex*, PointsInfo>                      m_points;           // collection of points in the octree
    QHash<CT_AbstractPointCloudIndex*, PointsInfo>                      m_pointsToAdd;      // collection of points not added
    Octree< CT_Repository::CT_AbstractModifiablePCIR >                  *m_octree;          // octree
    double                                                              m_size;             // size of a cells (in meters)
    Corner                                                              m_min;              // minimum corner of the complete octree
    Corner                                                              m_max;              // maximum corner of the complete octree
    Corner                                                              m_newMin;           // new minimum corner of the complete octree (if add or remove points)
    Corner                                                              m_newMax;           // new maximum corner of the complete octree (if add or remove points)
    Corner                                                              m_octreeMinCorner;  // min corner of the octree
    Corner                                                              m_octreeMaxCorner;  // max corner of the octree
    int                                                                 m_newNumberOfCells; // new number of cells
    size_t                                                              m_nPointsToAdd;
    size_t                                                              m_pointsAdded;
    size_t                                                              m_progress;
    bool                                                                m_forceMustBeReconstructed;

    /**
     * @brief Return the distance between a point and a face of the frustrum
     */
    float distanceToFrustumPlane(int index, const double &x, const double &y, const double &z, GLdouble planeCoefficients[6][4]) const;

    /**
     * @brief Return true if an aabox is visible in frustrum
     */
    bool aaBoxIsVisible(const QVector3D &p1, const QVector3D &p2, GLdouble m_planeCoefficients[6][4], bool *entirely = NULL) const;

    void resetNewMinAndMax();

    void computeNewMinAndMax();

    void adjustNewMin(const Corner &min);

    void adjustNewMax(const Corner &max);

    void addPointsToOctree(const CT_AbstractPointCloudIndex *index);

    void removePoinsFromOctree(const CT_AbstractPointCloudIndex *index);

    static void staticComputeMinMax(const CT_AbstractPointCloudIndex *index, PointsInfo &info);
};

#endif // OCTREECONTROLLER_H
