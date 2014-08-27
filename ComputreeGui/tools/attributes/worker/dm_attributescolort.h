#ifndef DM_ATTRIBUTESCOLORT_H
#define DM_ATTRIBUTESCOLORT_H

#include "tools/attributes/worker/abstract/dm_abstractattributescolor.h"

#include "view/DocumentView/gdocumentviewforgraphics.h"

#include "ct_attributes/ct_attributescolor.h"

template<typename Type, typename TypeCloudIndex>
class DM_AttributesColorT : public DM_AbstractAttributesColor
{
public:
    DM_AttributesColorT();

    void checkAndSetColorCloudToDoc();

    /**
     * @brief accept only Type (Point, Face, etc...) and colors
     */
    bool setTypeAttributes(const Type *ta, const CT_AttributesColor *ac);

    /**
     * @brief getter of I[Type]Attributes
     */
    CT_AttributesColor* colorAttributes() const;
    Type* abstractTypeAttributes() const;

protected:

    /**
     * @brief Apply the color to the [Type] of the document. Use [Type] cloud index from I[Type]Attributes.
     * @param doc : document to use
     */
    bool process(GDocumentViewForGraphics *doc);

    void attributesDeleted();

private:
    CT_AttributesColor  *m_ac;
};

#include "tools/attributes/worker/dm_attributescolort.hpp"

#endif // DM_ATTRIBUTESCOLORT_H
