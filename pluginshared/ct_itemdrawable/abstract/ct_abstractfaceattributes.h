#ifndef CT_ABSTRACTFACEATTRIBUTES_H
#define CT_ABSTRACTFACEATTRIBUTES_H

#include "ct_defines.h"

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"

#include "ct_itemdrawable/abstract/ct_abstractattributes.h"

/**
 * @brief Represents a cloud attribute for face.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractFaceAttributes : public CT_AbstractAttributes
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractFaceAttributes, CT_AbstractAttributes, Face attributes)

public:
    CT_AbstractFaceAttributes();
    CT_AbstractFaceAttributes(const CT_OutAbstractSingularItemModel *model,
                              const CT_AbstractResult *result,
                              CT_FCIR cir);

    CT_AbstractFaceAttributes(const QString &modelName,
                              const CT_AbstractResult *result,
                              CT_FCIR cir);

    inline const CT_AbstractFaceCloudIndex* abstractCloudIndex() const { return getFaceCloudIndex(); }

    /**
      * \brief Retourne les index du nuage de face.
      */
    const CT_AbstractFaceCloudIndex* getFaceCloudIndex() const;

    /**
     * @brief FaceCloudIndexRegistered getter
     * @return retourne le nuage de points enregistré
     */
    CT_FCIR getFaceCloudIndexRegistered() const;


private:
    // TODO
    //static CT_StandardAbstractFaceAttributesDrawManager AEA_DRAW_MANAGER;

    CT_FCIR m_cir;
    CT_AbstractFaceCloudIndex* m_aci;
};

#endif // CT_ABSTRACTFACEATTRIBUTES_H
