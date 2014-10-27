#ifndef CT_FACE_H
#define CT_FACE_H

#include "pluginShared_global.h"

class CT_Edge;
class CT_Point;

/**
 * A Face for a mesh. You can create Face with the help of the class CT_MeshAllocatorT.
 */
class PLUGINSHAREDSHARED_EXPORT CT_Face
{
public:
    inline CT_Face() { m_edge = 0; }

    // vcg method
    CT_Point* V(const int &j) const;
    CT_Point* P(const int &j) const;

    // pluginshared method
    CT_Point* pointAt(const int &j) const;
    CT_Edge* edge() const;
    size_t iPointAt(const int &j) const;
    size_t iEdge() const { return m_edge; }

    void setEdge(CT_Edge *edge);
    inline void setEdge(size_t edgeGlobalIndex) { m_edge = edgeGlobalIndex; }

private:
    size_t  m_edge;   // edge
};

#endif // CT_FACE_H
