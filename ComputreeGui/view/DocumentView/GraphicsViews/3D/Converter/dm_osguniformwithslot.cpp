#include "dm_osguniformwithslot.h"

DM_OsgUniformWithSlot::DM_OsgUniformWithSlot(const char* name, float f) : osg::Uniform(name, f)
{
}

DM_OsgUniformWithSlot::DM_OsgUniformWithSlot(const char *name, const osg::Vec4 &v4) : osg::Uniform(name, v4)
{
}

void DM_OsgUniformWithSlot::setBySlot(float f)
{
    set(f);
}

void DM_OsgUniformWithSlot::setBySlot(const osg::Vec4 &v4)
{
    set(v4);
}

void DM_OsgUniformWithSlot::setBySlot(const QColor &c)
{
    set(osg::Vec4(c.redF(), c.greenF(), c.blueF(), c.alphaF()));
}
