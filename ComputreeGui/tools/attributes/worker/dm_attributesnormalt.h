#ifndef DM_ATTRIBUTESNORMALT_H
#define DM_ATTRIBUTESNORMALT_H

#include "tools/attributes/worker/abstract/dm_abstractattributesnormal.h"
#include "view/DocumentView/gdocumentviewforgraphics.h"
#include "dm_guimanager.h"

#include "ct_attributes/ct_attributesnormal.h"
#include "ct_normalcloud/registered/ct_standardnormalcloudregistered.h"
#include "ct_normalcloud/abstract/ct_abstractnormalcloud.h"

template<typename Type, typename TypeCloudIndex>
class DM_AttributesNormalT : public DM_AbstractAttributesNormal
{
public:
    DM_AttributesNormalT();

    void checkAndSetNormalCloudToDoc();

    /**
     * @brief accept only Type (Point, Face, etc...) and normals
     */
    bool setTypeAttributes(const Type *ta, const CT_AttributesNormal *an);

    /**
     * @brief getter of I[Type]Attributes
     */
    CT_AttributesNormal* normalAttributes() const;
    Type* abstractTypeAttributes() const;

protected:

    /**
     * @brief Apply the normal to the [Type] of the document. Use [Type] cloud index from I[Type]Attributes.
     * @param doc : document to use
     */
    bool process(GDocumentViewForGraphics *doc) {  Q_UNUSED(doc) return false; }

    void attributesDeleted();

private:
    CT_AttributesNormal *m_an;
};

// specialisation for points
template<>
bool DM_AttributesNormalT<CT_AbstractPointsAttributes, CT_AbstractCloudIndex>::process(GDocumentViewForGraphics *doc);

template<>
bool DM_AttributesNormalT<CT_AbstractFaceAttributes, CT_AbstractCloudIndex>::process(GDocumentViewForGraphics *doc);

#include "tools/attributes/worker/dm_attributesnormalt.hpp"

#endif // DM_ATTRIBUTESNORMALT_H
