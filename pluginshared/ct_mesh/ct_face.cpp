#include "ct_face.h"
#include "ct_mesh/ct_edge.h"

CT_Point* CT_Face::V(const int &j) const { if(j==2) return edge()->next()->V(1); else return edge()->V(j); }
CT_Point* CT_Face::P(const int &j) const { return V(j); }

// pluginshared method
CT_Point* CT_Face::pointAt(const int &j) const { return V(j); }
CT_Edge* CT_Face::edge() const { return &PS_REPOSITORY->globalCloud<CT_Edge>()->tAt(m_edge); }

size_t CT_Face::iPointAt(const int &j) const
{
    if(j==2) return edge()->next()->iPointAt(1); else return edge()->iPointAt(j);
}

void CT_Face::setEdge(CT_Edge *edge) { setEdge(PS_REPOSITORY->globalCloud<CT_Edge>()->indexOf(edge)); }
