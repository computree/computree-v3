#include "dm_boundingsphereitemselectedvisitor.h"

#include "view/DocumentView/GraphicsViews/3D/dm_osgscenemanager.h"

DM_BoundingSphereItemSelectedVisitor::DM_BoundingSphereItemSelectedVisitor() : NodeVisitor( NodeVisitor::TRAVERSE_ACTIVE_CHILDREN )
{
    m_transformMatrix.makeIdentity();
}

const osg::BoundingBox &DM_BoundingSphereItemSelectedVisitor::getBound() const
{
    return m_bound;
}

void DM_BoundingSphereItemSelectedVisitor::apply(osg::Group &node)
{
    CT_AbstractItemDrawable *item = DM_OsgSceneManager::staticConvertNodeToItemDrawable(node);

    if(item != NULL) {

        if(item->isSelected()) {
            osg::BoundingBox bbox;
            bbox.expandBy( node.getBound());

            osg::BoundingBox bboxTrans;

            for( unsigned int i = 0; i < 8; ++i ) {
                osg::Vec3 xvec = bbox.corner( i ) * m_transformMatrix;
                bboxTrans.expandBy( xvec );
            }

            m_bound.expandBy( bboxTrans );
        }
    } else {
        traverse( node );
    }
}

void DM_BoundingSphereItemSelectedVisitor::apply( osg::MatrixTransform &node )
{
    m_transformMatrix *= node.getMatrix();
    traverse( node );
}
