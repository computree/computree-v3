#ifndef OCTREEBUILDER_H
#define OCTREEBUILDER_H

#include "dm_document.h"
#include "tools/worker/dm_abstractworker.h"
#include "view/DocumentView/GraphicsViews/3D/Octree/octreecontroller.h"

class OctreeBuilder : public DM_AbstractWorker
{
public:
    OctreeBuilder();

    /**
     * @brief Set the document who contains the octree controller
     */
    void setDocument(const DM_Document *document);

    /**
     * @brief Set the controller of the octree
     */
    void setOctreeController(const OctreeController *octree);

private:
    OctreeController    *m_octree;
    DM_Document         *m_doc;

public slots:

    /**
     * @brief Construct the octree.
     */
    void apply();
};

#endif // OCTREEBUILDER_H
