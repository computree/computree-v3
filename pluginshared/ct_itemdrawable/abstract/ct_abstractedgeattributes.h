#ifndef CT_ABSTRACTEDGEATTRIBUTES_H
#define CT_ABSTRACTEDGEATTRIBUTES_H

#include "ct_defines.h"

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"

#include "ct_itemdrawable/abstract/ct_abstractattributes.h"

/**
 * @brief Represents a cloud attribute for edge.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractEdgeAttributes : public CT_AbstractAttributes
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractEdgeAttributes, CT_AbstractAttributes, Edge attributes)

public:
    CT_AbstractEdgeAttributes();
    CT_AbstractEdgeAttributes(const CT_OutAbstractSingularItemModel *model,
                              const CT_AbstractResult *result,
                              CT_ECIR cir);

    CT_AbstractEdgeAttributes(const QString &modelName,
                              const CT_AbstractResult *result,
                              CT_ECIR cir);

    inline const CT_AbstractEdgeCloudIndex* abstractCloudIndex() const { return getEdgeCloudIndex(); }

    /**
      * \brief Retourne les index du nuage de edge.
      */
    const CT_AbstractEdgeCloudIndex* getEdgeCloudIndex() const;

    /**
     * @brief EdgeCloudIndexRegistered getter
     * @return retourne le nuage de points enregistré
     */
    CT_ECIR getEdgeCloudIndexRegistered() const;

private:
    // TODO
    //static CT_StandardAbstractEdgeAttributesDrawManager AEA_DRAW_MANAGER;

    CT_ECIR m_cir;
    CT_AbstractEdgeCloudIndex* m_aci;
};

#endif // CT_ABSTRACTEDGEATTRIBUTES_H
