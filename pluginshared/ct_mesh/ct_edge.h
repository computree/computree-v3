#ifndef CT_EDGE_H
#define CT_EDGE_H

#include "ct_global/ct_context.h"
#include "ct_face.h"

/**
 * A Edge for a mesh. You can create Edge with the help of the class CT_MeshAllocatorT.
 */
class PLUGINSHAREDSHARED_EXPORT CT_Edge
{
public:
    inline CT_Edge()
    {
        v[0] = 0;
        v[1] = 0;

        e[0] = 0;
        e[1] = 0;
        e[2] = 0;

        m_face = 0;
    }

    // vcg method
    inline CT_Point* V(const int &j) const { return &PS_REPOSITORY->globalCloud<CT_Point>()->tAt(v[j]); }
    inline CT_Point* P(const int &j) const { return V(j); }

    // pluginshared method
    inline CT_Point* pointAt(const int &j) const { return V(j); }
    inline CT_Edge* edge(const int &j) const { return &PS_REPOSITORY->globalCloud<CT_Edge>()->tAt(e[j]); }
    inline CT_Edge* next() const { return edge(0); }
    inline CT_Edge* previous() const { return edge(1); }
    inline CT_Edge* twin() const { return edge(2); }
    inline CT_Face* face() const { return &PS_REPOSITORY->globalCloud<CT_Face>()->tAt(m_face); }

    inline size_t iPointAt(const int &j) const { return v[j]; }
    inline size_t iEdge(const int &j) const { return e[j]; }
    inline size_t iNext() const { return iEdge(0); }
    inline size_t iPrevious() const { return iEdge(1); }
    inline size_t iTwin() const { return iEdge(2); }
    inline size_t iFace() const { return m_face; }


    inline void setPoint(const int &j, CT_Point *point) { setPoint(j, PS_REPOSITORY->globalCloud<CT_Point>()->indexOf(point)); }
    inline void setPoint(const int &j, const size_t &pointGlobalIndex) { v[j] = pointGlobalIndex; }
    inline void setPoint0(CT_Point *point) { setPoint0(PS_REPOSITORY->globalCloud<CT_Point>()->indexOf(point)); }
    inline void setPoint0(const size_t &pointGlobalIndex) { v[0] = pointGlobalIndex; }
    inline void setPoint1(CT_Point *point) { setPoint1(PS_REPOSITORY->globalCloud<CT_Point>()->indexOf(point)); }
    inline void setPoint1(const size_t &pointGlobalIndex) { v[1] = pointGlobalIndex; }

    inline void setEdge(const int &j, CT_Edge *edge) { setEdge(j, PS_REPOSITORY->globalCloud<CT_Edge>()->indexOf(edge)); }
    inline void setEdge(const int &j, const size_t &edgeGlobalIndex) { e[j] = edgeGlobalIndex; }

    inline void setNext(CT_Edge *edge) { setNext(PS_REPOSITORY->globalCloud<CT_Edge>()->indexOf(edge)); }
    inline void setNext(const size_t &edgeGlobalIndex) { e[0] = edgeGlobalIndex; }
    inline void setPrevious(CT_Edge *edge) { setPrevious(PS_REPOSITORY->globalCloud<CT_Edge>()->indexOf(edge)); }
    inline void setPrevious(const size_t &edgeGlobalIndex) { e[1] = edgeGlobalIndex; }
    inline void setTwin(CT_Edge *edge) { setTwin(PS_REPOSITORY->globalCloud<CT_Edge>()->indexOf(edge)); }
    inline void setTwin(const size_t &edgeGlobalIndex) { e[2] = edgeGlobalIndex; }
    inline void setFace(CT_Face *face) { setFace(PS_REPOSITORY->globalCloud<CT_Face>()->indexOf(face)); }
    inline void setFace(const size_t &faceGlobalIndex) { m_face = faceGlobalIndex; }

private:
    size_t      v[2];   // tail and head point (TODO perhaps we can have only one points ? the other other points is in twin edge)
    size_t      e[3];   // next, prev, twin edge
    size_t      m_face; // face
};

#endif // CT_EDGE_H
