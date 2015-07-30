#ifndef DM_BOUNDINGSPHEREITEMSELECTEDVISITOR_H
#define DM_BOUNDINGSPHEREITEMSELECTEDVISITOR_H

#include <osg/NodeVisitor>
#include <osg/BoundingBox>
#include <osg/Group>
#include <osg/MatrixTransform>

class DM_BoundingSphereItemSelectedVisitor : public osg::NodeVisitor
{
public:
    DM_BoundingSphereItemSelectedVisitor();

    const osg::BoundingBox& getBound() const;

    void apply(osg::Group& node);
    void apply(osg::MatrixTransform &node);

private:

    osg::BoundingBox    m_bound;            // the overall resultant bounding sphere
    osg::Matrix         m_transformMatrix;  // the current transform matrix
};

#endif // DM_BOUNDINGSPHEREITEMSELECTEDVISITOR_H
