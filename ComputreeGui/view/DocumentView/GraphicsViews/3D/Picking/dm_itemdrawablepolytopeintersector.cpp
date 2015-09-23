#include "dm_itemdrawablepolytopeintersector.h"

#include <osg/TemplatePrimitiveFunctor>
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_paintertoosgelements.h"

namespace ItemDrawablePolytopeIntersectorUtils
{
    typedef osg::Plane::Vec3_type Vec3_type;
    typedef Vec3_type::value_type value_type;
    typedef osg::Polytope::ClippingMask PlaneMask;
    typedef std::list<std::pair<PlaneMask,Vec3_type> > CandList_t;

    class PolytopePrimitiveIntersector {
    public:

        typedef osg::Polytope::PlaneList    PlaneList;

        /// a line defined by the intersection of two planes
        struct PlanesLine
        {
            PlanesLine(PlaneMask m, Vec3_type p, Vec3_type d) :
            mask(m), pos(p), dir(d) {}
            PlaneMask mask;
            Vec3_type pos;
            Vec3_type dir;
        };
        typedef std::vector<PlanesLine>     LinesList;

        PolytopePrimitiveIntersector() :
            _plane_mask(0x0),
            m_isIntersected(false) {
        }

        bool isIntersected() const { return m_isIntersected; }

        value_type eps() { return 1e-6; }

        /* check which candidate points lie within the polytope volume
           mark outliers with mask == 0, return number of remaining candidates */
        unsigned int checkCandidatePoints(CandList_t &candidates, PlaneMask inside_mask)
        {
            PlaneMask selector_mask = 0x1;
            unsigned int numCands=candidates.size();
            for(PlaneList::const_iterator it=_planes.begin();
                it!=_planes.end() && numCands>0;
                ++it, selector_mask <<= 1)
            {
                const osg::Plane& plane=*it;
                if (selector_mask & inside_mask) continue;

                for (CandList_t::iterator pointIt=candidates.begin(); pointIt!=candidates.end(); ++pointIt)
                {
                    PlaneMask& mask=pointIt->first;
                    if (mask==0) continue;
                    if (selector_mask & mask) continue;
                    if (plane.distance(pointIt->second)<0.0f)
                    {
                        mask=0;  // mark as outside
                        --numCands;
                        if (numCands==0) return 0;
                    }
                }
            }
            return numCands;
        }

        /// get boundary lines of polytope
        LinesList& getPolytopeLines()
        {
            if (!_lines.empty()) return _lines;

            PlaneMask selector_mask = 0x1;
            for (PlaneList::const_iterator it=_planes.begin(); it!=_planes.end();
             ++it, selector_mask <<= 1 ) {
            const osg::Plane& plane1=*it;
            const Vec3_type normal1=plane1.getNormal();
            const Vec3_type point1=normal1*(-plane1[3]);   /// canonical point on plane1
            PlaneMask sub_selector_mask = (selector_mask<<1);
            for (PlaneList::const_iterator jt=it+1; jt!=_planes.end(); ++jt, sub_selector_mask <<= 1 ) {
                const osg::Plane& plane2=*jt;
                const Vec3_type normal2=plane2.getNormal();
                if (osg::absolute(normal1*normal2) > (1.0-eps())) continue;
                const Vec3_type lineDirection = normal1^normal2;

                const Vec3_type searchDirection = lineDirection^normal1; /// search dir in plane1
                const value_type seachDist = -plane2.distance(point1)/(searchDirection*normal2);
                if (osg::isNaN(seachDist)) continue;
                const Vec3_type linePoint=point1+searchDirection*seachDist;
                _lines.push_back(PlanesLine(selector_mask|sub_selector_mask, linePoint, lineDirection));
            }
            }
            return _lines;
        }

        // handle points
        void operator()(const Vec3_type v1, bool /*treatVertexDataAsTemporary*/)
        {
            if (m_isIntersected) return;

            for (PlaneList::const_iterator it=_planes.begin(); it!=_planes.end(); ++it)
            {
                const osg::Plane& plane=*it;
                const value_type d1=plane.distance(v1);
                if (d1<0.0f) return;   // point outside
            }

            m_isIntersected = true;
        }

        // handle lines
        void operator()(const Vec3_type v1, const Vec3_type v2, bool /*treatVertexDataAsTemporary*/)
        {
            if (m_isIntersected) return;

            PlaneMask selector_mask = 0x1;
            PlaneMask inside_mask = 0x0;

            CandList_t candidates;

            bool v1Inside = true;
            bool v2Inside = true;
            for (PlaneList::const_iterator it=_planes.begin(); it!=_planes.end(); ++it, selector_mask<<=1)
            {
                const osg::Plane& plane=*it;
                const value_type d1=plane.distance(v1);
                const value_type d2=plane.distance(v2);
                const bool d1IsNegative = (d1<0.0f);
                const bool d2IsNegative = (d2<0.0f);
                if (d1IsNegative && d2IsNegative) return;      // line outside

                if (!d1IsNegative && !d2IsNegative)
                {
                    inside_mask |= selector_mask;
                    continue;   // completly inside
                }
                if (d1IsNegative) v1Inside = false;
                if (d2IsNegative) v2Inside = false;
                if (d1==0.0f)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, v1) );
                }
                else if (d2==0.0f)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, v2) );
                }
                else if (d1IsNegative && !d2IsNegative)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, (v1-(v2-v1)*(d1/(-d1+d2))) ) );
                } else if (!d1IsNegative && d2IsNegative)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, (v1+(v2-v1)*(d1/(d1-d2))) ) );
                }

            }

            if ((inside_mask==_plane_mask) || checkCandidatePoints(candidates, inside_mask)>0)
                m_isIntersected = true;
        }

        // handle triangles
        void operator()(const Vec3_type v1, const Vec3_type v2, const Vec3_type v3, bool /*treatVertexDataAsTemporary*/)
        {
            PlaneMask selector_mask = 0x1;
            PlaneMask inside_mask = 0x0;
            CandList_t candidates;

            for(PlaneList::const_iterator it=_planes.begin();
                it!=_planes.end();
                ++it, selector_mask <<= 1)
            {
                const osg::Plane& plane=*it;
                const value_type d1=plane.distance(v1);
                const value_type d2=plane.distance(v2);
                const value_type d3=plane.distance(v3);
                const bool d1IsNegative = (d1<0.0f);
                const bool d2IsNegative = (d2<0.0f);
                const bool d3IsNegative = (d3<0.0f);
                if (d1IsNegative && d2IsNegative && d3IsNegative) return;      // triangle outside
                if (!d1IsNegative && !d2IsNegative && !d3IsNegative)
                {
                    inside_mask |= selector_mask;
                    continue;   // completly inside
                }

                // edge v1-v2 intersects
                if (d1==0.0f)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, v1) );
                }
                else if (d2==0.0f)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, v2) );
                }
                else if (d1IsNegative && !d2IsNegative)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, (v1-(v2-v1)*(d1/(-d1+d2))) ) );
                }
                else if (!d1IsNegative && d2IsNegative)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, (v1+(v2-v1)*(d1/(d1-d2))) ) );
                }

                // edge v1-v3 intersects
                if (d3==0.0f)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, v3) );
                }
                else if (d1IsNegative && !d3IsNegative)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, (v1-(v3-v1)*(d1/(-d1+d3))) ) );
                }
                else if (!d1IsNegative && d3IsNegative)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, (v1+(v3-v1)*(d1/(d1-d3))) ) );
                }

                // edge v2-v3 intersects
                if (d2IsNegative && !d3IsNegative)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, (v2-(v3-v2)*(d2/(-d2+d3))) ) );
                } else if (!d2IsNegative && d3IsNegative)
                {
                    candidates.push_back( CandList_t::value_type(selector_mask, (v2+(v3-v2)*(d2/(d2-d3))) ) );
                }
            }

            if (_plane_mask==inside_mask)
            { // triangle lies inside of all planes
                m_isIntersected = true;
                return;
            }

            if (candidates.empty() && _planes.size()<3) return;

            if (checkCandidatePoints(candidates, inside_mask)>0)
            {
                m_isIntersected = true;
                return;
            }

            // handle case where the polytope goes through the triangle
            // without containing any point of it

            LinesList& lines=getPolytopeLines();
            candidates.clear();

            // check all polytope lines against the triangle
            // use algorithm from "Real-time rendering" (second edition) pp.580
            const Vec3_type e1=v2-v1;
            const Vec3_type e2=v3-v1;

            for (LinesList::const_iterator it=lines.begin(); it!=lines.end(); ++it)
            {
                const PlanesLine& line=*it;

                Vec3_type p=line.dir^e2;
                const value_type a=e1*p;
                if (osg::absolute(a)<eps()) continue;

                const value_type f=1.0f/a;
                const Vec3_type s=(line.pos-v1);
                const value_type u=f*(s*p);
                if (u<0.0f || u>1.0f) continue;

                const Vec3_type q=s^e1;
                const value_type v=f*(line.dir*q);
                if (v<0.0f || u+v>1.0f) continue;

                const value_type t=f*(e2*q);

                candidates.push_back(CandList_t::value_type(line.mask, line.pos+line.dir*t));
            }

            if (checkCandidatePoints(candidates, inside_mask)>0)
            {
                m_isIntersected = true;
                return;
            }
        }

        // handle quads
        void operator()(const Vec3_type v1, const Vec3_type v2, const Vec3_type v3, const Vec3_type v4, bool treatVertexDataAsTemporary)
        {
            this->operator()(v1,v2,v3,treatVertexDataAsTemporary);
            this->operator()(v1,v3,v4,treatVertexDataAsTemporary);
        }

        void setPolytope(osg::Polytope& polytope)
        {
            const PlaneMask currentMask = polytope.getCurrentMask();
            PlaneMask selector_mask = 0x1;

            const PlaneList& planeList = polytope.getPlaneList();
            unsigned int numActivePlanes = 0;

            PlaneList::const_iterator itr;
            for(itr=planeList.begin(); itr!=planeList.end(); ++itr)
            {
                if (currentMask&selector_mask) ++numActivePlanes;
                selector_mask <<= 1;
            }

            _plane_mask = 0x0;
            _planes.clear();
            _planes.reserve(numActivePlanes);
            _lines.clear();

            selector_mask=0x1;
            for(itr=planeList.begin(); itr!=planeList.end(); ++itr)
            {
                if (currentMask&selector_mask)
                {
                    _planes.push_back(*itr);
                    _plane_mask <<= 1;
                    _plane_mask |= 0x1;
                }
                selector_mask <<= 1;
            }
        }

        private:
        PlaneList   _planes;                   ///< active planes extracted from polytope
        LinesList   _lines;                    ///< all intersection lines of two polytope planes
        PlaneMask   _plane_mask;               ///< mask for all planes of the polytope
        bool        m_isIntersected;
    }; // class PolytopePrimitiveIntersector

} // namespace PolytopeIntersectorUtils

DM_ItemDrawablePolytopeIntersector::DM_ItemDrawablePolytopeIntersector(const osg::Polytope& polytope):
    m_parent(NULL),
    m_polytope(polytope),
    m_isIntersected(false)
{
}

DM_ItemDrawablePolytopeIntersector::DM_ItemDrawablePolytopeIntersector(CoordinateFrame cf, const osg::Polytope& polytope):
    Intersector(cf),
    m_parent(NULL),
    m_polytope(polytope),
    m_isIntersected(false)
{
}

DM_ItemDrawablePolytopeIntersector::DM_ItemDrawablePolytopeIntersector(CoordinateFrame cf, double xMin, double yMin, double xMax, double yMax):
    Intersector(cf),
    m_parent(NULL),
    m_isIntersected(false)
{
    double zNear = 0.0;
    switch(cf)
    {
        case WINDOW : zNear = 0.0; break;
        case PROJECTION : zNear = 1.0; break;
        case VIEW : zNear = 0.0; break;
        case MODEL : zNear = 0.0; break;
    }

    m_polytope.add(osg::Plane(1.0, 0.0, 0.0, -xMin));
    m_polytope.add(osg::Plane(-1.0,0.0 ,0.0, xMax));
    m_polytope.add(osg::Plane(0.0, 1.0, 0.0,-yMin));
    m_polytope.add(osg::Plane(0.0,-1.0,0.0, yMax));
    m_polytope.add(osg::Plane(0.0,0.0,1.0, zNear));
}

osgUtil::Intersector* DM_ItemDrawablePolytopeIntersector::clone(osgUtil::IntersectionVisitor& iv)
{
    if (_coordinateFrame==MODEL && iv.getModelMatrix()==0)
    {
        osg::ref_ptr<DM_ItemDrawablePolytopeIntersector> pi = new DM_ItemDrawablePolytopeIntersector(m_polytope);
        pi->_intersectionLimit = this->_intersectionLimit;
        pi->m_isIntersected = this->m_isIntersected;
        pi->m_parent = this;
        return pi.release();
    }

    // compute the matrix that takes this Intersector from its CoordinateFrame into the local MODEL coordinate frame
    // that geometry in the scene graph will always be in.
    osg::Matrix matrix;
    switch (_coordinateFrame)
    {
        case(WINDOW):
            if (iv.getWindowMatrix()) matrix.preMult( *iv.getWindowMatrix() );
            if (iv.getProjectionMatrix()) matrix.preMult( *iv.getProjectionMatrix() );
            if (iv.getViewMatrix()) matrix.preMult( *iv.getViewMatrix() );
            if (iv.getModelMatrix()) matrix.preMult( *iv.getModelMatrix() );
            break;
        case(PROJECTION):
            if (iv.getProjectionMatrix()) matrix.preMult( *iv.getProjectionMatrix() );
            if (iv.getViewMatrix()) matrix.preMult( *iv.getViewMatrix() );
            if (iv.getModelMatrix()) matrix.preMult( *iv.getModelMatrix() );
            break;
        case(VIEW):
            if (iv.getViewMatrix()) matrix.preMult( *iv.getViewMatrix() );
            if (iv.getModelMatrix()) matrix.preMult( *iv.getModelMatrix() );
            break;
        case(MODEL):
            if (iv.getModelMatrix()) matrix = *iv.getModelMatrix();
            break;
    }

    osg::Polytope transformedPolytope;
    transformedPolytope.setAndTransformProvidingInverse(m_polytope, matrix);

    osg::ref_ptr<DM_ItemDrawablePolytopeIntersector> pi = new DM_ItemDrawablePolytopeIntersector(transformedPolytope);
    pi->_intersectionLimit = this->_intersectionLimit;
    pi->m_isIntersected = this->m_isIntersected;
    pi->m_parent = this;
    return pi.release();
}

bool DM_ItemDrawablePolytopeIntersector::enter(const osg::Node& node)
{
    if (reachedLimit()) return false;
    return !node.isCullingActive() || m_polytope.contains( node.getBound() );
}

void DM_ItemDrawablePolytopeIntersector::leave()
{
    // do nothing.
}

void DM_ItemDrawablePolytopeIntersector::intersect(osgUtil::IntersectionVisitor& iv, osg::Drawable* drawable)
{
    if (reachedLimit()) return;

    if ( !m_polytope.contains( drawable->getBound() ) ) return;

    osg::TemplatePrimitiveFunctor<ItemDrawablePolytopeIntersectorUtils::PolytopePrimitiveIntersector> func;
    func.setPolytope( m_polytope);

    drawable->accept(func);

    if(func.isIntersected()) {

        osg::Group *g = DM_PainterToOsgElements::staticGetResultFromDrawable(drawable);

        if(g != NULL)
            setIntersected(g);
    }
}

void DM_ItemDrawablePolytopeIntersector::reset()
{
    Intersector::reset();

    m_isIntersected = false;
}

void DM_ItemDrawablePolytopeIntersector::setIntersected(osg::Group *r)
{
    if(m_parent != NULL)
        m_parent->setIntersected(r);

    m_isIntersected = true;
    m_results.insert(r);
}


