#include "octreebuilder.h"

OctreeBuilder::OctreeBuilder()
{
    m_octree = NULL;
    m_doc = NULL;
}

void OctreeBuilder::setDocument(const DM_Document *document)
{
    m_doc = (DM_Document*)document;
}

void OctreeBuilder::setOctreeController(const OctreeController *octree)
{
    m_octree = (OctreeController*)octree;

    connect(m_octree, SIGNAL(constructionInProgress(int)), this, SLOT(setProgress(int)), Qt::DirectConnection);
}

void OctreeBuilder::apply()
{
    m_doc->lock();
    m_octree->construct();
    m_doc->unlock();

    setFinished();
}
