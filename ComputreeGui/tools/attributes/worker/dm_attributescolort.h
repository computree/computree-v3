#ifndef DM_ATTRIBUTESCOLORT_H
#define DM_ATTRIBUTESCOLORT_H

#include "tools/attributes/worker/abstract/dm_abstractattributescolor.h"

#include "dm_guimanager.h"

#include "view/DocumentView/gdocumentviewforgraphics.h"

#include "ct_attributes/ct_attributescolor.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_cloudindex/ct_cloudindexlessmemoryt.h"

template<typename Type>
class DM_AttributesColorT : public DM_AbstractAttributesColor
{
public:
    DM_AttributesColorT();

    /**
     * @brief Check if the doc has the necessary cloud (color cloud) or set it if not
     */
    void checkAndSetNecessaryCloudToDoc();

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
