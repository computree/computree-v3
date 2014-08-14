#ifndef CT_ABSTRACTFACEATTRIBUTES_H
#define CT_ABSTRACTFACEATTRIBUTES_H

#include "ct_mesh/cloud/abstract/ct_abstractfacecloudindex.h"

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"

#include "ct_itemdrawable/abstract/ct_abstractattributes.h"

/**
 * @brief Represents a cloud attribute for face.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractFaceAttributes : public CT_AbstractAttributes
{
    Q_OBJECT

public:
    CT_AbstractFaceAttributes();
    CT_AbstractFaceAttributes(const CT_OutAbstractItemModel *model,
                              const CT_AbstractResult *result,
                              CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR cir);

    CT_AbstractFaceAttributes(const QString &modelName,
                              const CT_AbstractResult *result,
                              CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR cir);

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    QString getType() const;
    static QString staticGetType();

    inline const CT_AbstractFaceCloudIndex* abstractCloudIndex() const { return getFaceCloudIndex(); }

    /**
      * \brief Retourne les index du nuage de face.
      */
    const CT_AbstractFaceCloudIndex* getFaceCloudIndex() const;

    /**
      * \brief Retourne les index du nuage de face.
      */
    const CT_AbstractCloudIndexT<CT_Face>* getFaceCloudIndexT() const;

    /**
     * @brief FaceCloudIndexRegistered getter
     * @return retourne le nuage de points enregistré
     */
    CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR getFaceCloudIndexRegistered() const;

private:
    // TODO
    //static CT_StandardAbstractFaceAttributesDrawManager AEA_DRAW_MANAGER;

    CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR m_cir;
    CT_AbstractFaceCloudIndex* m_aci;
};

#endif // CT_ABSTRACTFACEATTRIBUTES_H
