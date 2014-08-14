#ifndef DM_ATTRIBUTESNORMALT_H
#define DM_ATTRIBUTESNORMALT_H

#include "interfaces.h"

#include "tools/attributes/worker/abstract/dm_abstractattributesnormal.h"
#include "view/DocumentView/gdocumentviewforgraphics.h"
#include "dm_guimanager.h"

template<typename Type, typename TypeCloudIndex>
class DM_AttributesNormalT : public DM_AbstractAttributesNormal
{
public:
    DM_AttributesNormalT();

    void checkAndSetNormalCloudToDoc();

    /**
     * @brief accept only Type (Point, Face, etc...) and normals
     */
    bool setTypeAttributes(const Type *ta, const IAttributesNormal *an);

    /**
     * @brief getter of I[Type]Attributes
     */
    IAttributesNormal* normalAttributes() const;
    Type* abstractTypeAttributes() const;

protected:

    /**
     * @brief Apply the normal to the [Type] of the document. Use [Type] cloud index from I[Type]Attributes.
     * @param doc : document to use
     */
    bool process(GDocumentViewForGraphics *doc) {  Q_UNUSED(doc) return false; }

    void attributesDeleted();

private:
    IAttributesNormal *m_an;
};

// specialisation for points
template<>
bool DM_AttributesNormalT<IPointAttributes, ICloudIndex>::process(GDocumentViewForGraphics *doc);

template<>
bool DM_AttributesNormalT<IFaceAttributes, ICloudIndex>::process(GDocumentViewForGraphics *doc);

#include "tools/attributes/worker/dm_attributesnormalt.hpp"

#endif // DM_ATTRIBUTESNORMALT_H
