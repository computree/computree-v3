#include "octreebuilder.h"

#include <QApplication>

OctreeBuilder::OctreeBuilder() : DM_AbstractWorker()
{
    m_octree = NULL;
    m_doc = NULL;
}

OctreeBuilder::~OctreeBuilder()
{
}

void OctreeBuilder::setDocument(const DM_Document *document)
{
    m_doc = (DM_Document*)document;
}

void OctreeBuilder::setOctreeController(const OctreeController *octree)
{
    m_octree = (OctreeController*)octree;
}

void OctreeBuilder::apply()
{
    connect(m_octree, SIGNAL(constructionInProgress(int)), this, SLOT(setProgress(int)), Qt::DirectConnection);

    m_octree->construct();

    setFinished();
}
