#ifndef DM_ITEMDRAWABLEPOLYTOPEINTERSECTOR_H
#define DM_ITEMDRAWABLEPOLYTOPEINTERSECTOR_H

#include <osgUtil/IntersectionVisitor>
#include <QSet>

/**
 * @brief Use this class to pick itemdrawable.
 * @warning You must enable the picking (use DM_OsgSceneManager::staticSetEnablePicking) for only one item at time and call "accept" at camera of the
 *          view. This class will check if at least one point is contained in the polytope defined. At the first element founded it will stop
 *          the process.
 */
class DM_ItemDrawablePolytopeIntersector : public osgUtil::Intersector
{
public:
    /**
     * Construct a PolytopeIntersector using specified polytope in MODEL coordinates.
     */
    DM_ItemDrawablePolytopeIntersector(const osg::Polytope& polytope);

    /**
     * Construct a PolytopeIntersector using specified polytope in specified coordinate frame.
     */
    DM_ItemDrawablePolytopeIntersector(CoordinateFrame cf, const osg::Polytope& polytope);

    /** Convenience constructor for supporting picking in WINDOW, or PROJECTION coordinates
     *  In WINDOW coordinates (clip space cube) creates a five sided polytope box that has a front face at 0.0 and sides around box xMin, yMin, xMax, yMax.
     *  In PROJECTION coordinates (clip space cube) creates a five sided polytope box that has a front face at -1 and sides around box xMin, yMin, xMax, yMax.
     *  In VIEW and MODEL coordinates (clip space cube) creates a five sided polytope box that has a front face at 0.0 and sides around box xMin, yMin, xMax, yMax.
     */
    DM_ItemDrawablePolytopeIntersector(CoordinateFrame cf, double xMin, double yMin, double xMax, double yMax);

    virtual Intersector* clone(osgUtil::IntersectionVisitor& iv);

    virtual bool enter(const osg::Node& node);

    virtual void leave();

    virtual void intersect(osgUtil::IntersectionVisitor& iv, osg::Drawable* drawable);

    virtual void reset();

    virtual bool containsIntersections() { return m_parent ? m_parent->m_isIntersected : m_isIntersected; }

    virtual const QSet<osg::Group*>& results() { return m_parent ? m_parent->results() : m_results; }

private:
    DM_ItemDrawablePolytopeIntersector* m_parent;
    osg::Polytope                       m_polytope;
    bool                                m_isIntersected; // at true if at least one Vec3 is contained in the polytope
    QSet<osg::Group*>                   m_results;

    /**
     * @brief Call this if intersection is founded
     */
    void setIntersected(osg::Group *r);
};

#endif // DM_ITEMDRAWABLEPOLYTOPEINTERSECTOR_H
