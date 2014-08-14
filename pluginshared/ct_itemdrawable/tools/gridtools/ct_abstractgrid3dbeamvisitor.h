#ifndef CT_ABSTRACTGRID3DBEAMVISITOR_H
#define CT_ABSTRACTGRID3DBEAMVISITOR_H

#include "ct_itemdrawable/ct_beam.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractGrid3DBeamVisitor
{
public:   
    virtual void visit(const size_t &index, const CT_Beam *beam) = 0;

};

#endif // CT_ABSTRACTGRID3DBEAMVISITOR_H
