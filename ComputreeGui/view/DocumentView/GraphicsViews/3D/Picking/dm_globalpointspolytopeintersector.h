#ifndef DM_GLOBALPOINTSPOLYTOPEINTERSECTOR_H
#define DM_GLOBALPOINTSPOLYTOPEINTERSECTOR_H

#include <osgUtil/IntersectionVisitor>

/**
 * @brief Use this class to pick points and get indexes of picked points. Typically this class is used to pick points from global points cloud.
 * @warning Disable picking on local geometries otherwise indexes will be wrong !
 */
class DM_GlobalPointsPolytopeIntersector : public osgUtil::Intersector
{
public:
    typedef std::list<size_t> Intersections;

    enum ElementFilter {
        POINTS,
        EDGES,
        FACES
    };

    /**
     * Construct a PolytopeIntersector using specified polytope in MODEL coordinates.
     */
    DM_GlobalPointsPolytopeIntersector(const osg::Polytope& polytope);

    /**
     * Construct a PolytopeIntersector using specified polytope in specified coordinate frame.
     */
    DM_GlobalPointsPolytopeIntersector(CoordinateFrame cf, const osg::Polytope& polytope);

    /** Convenience constructor for supporting picking in WINDOW, or PROJECTION coordinates
     *  In WINDOW coordinates (clip space cube) creates a five sided polytope box that has a front face at 0.0 and sides around box xMin, yMin, xMax, yMax.
     *  In PROJECTION coordinates (clip space cube) creates a five sided polytope box that has a front face at -1 and sides around box xMin, yMin, xMax, yMax.
     *  In VIEW and MODEL coordinates (clip space cube) creates a five sided polytope box that has a front face at 0.0 and sides around box xMin, yMin, xMax, yMax.
     */
    DM_GlobalPointsPolytopeIntersector(CoordinateFrame cf, double xMin, double yMin, double xMax, double yMax);

    /**
     * @brief Set if you want to check points, edges or faces
     */
    void setFilter(ElementFilter filter);

    ElementFilter getFilter() const;

    virtual Intersector* clone(osgUtil::IntersectionVisitor& iv);

    virtual bool enter(const osg::Node& node);

    virtual void leave();

    virtual void intersect(osgUtil::IntersectionVisitor& iv, osg::Drawable* drawable);

    virtual void reset();

    virtual bool containsIntersections() { return !getIndexesCollection().empty(); }

    const Intersections& getIntersections() const;

private:
    DM_GlobalPointsPolytopeIntersector* m_parent;
    osg::Polytope                       m_polytope;
    Intersections                       m_indexes;
    ElementFilter                       m_filter;

    Intersections& getIndexesCollection();

    void insertIndex(const size_t &index);
};

#endif // DM_GLOBALPOINTSPOLYTOPEINTERSECTOR_H
