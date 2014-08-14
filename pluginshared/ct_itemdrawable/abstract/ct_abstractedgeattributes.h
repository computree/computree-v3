#ifndef CT_ABSTRACTEDGEATTRIBUTES_H
#define CT_ABSTRACTEDGEATTRIBUTES_H

#include "ct_mesh/cloud/abstract/ct_abstractedgecloudindex.h"

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"

#include "ct_itemdrawable/abstract/ct_abstractattributes.h"

/**
 * @brief Represents a cloud attribute for edge.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractEdgeAttributes : public CT_AbstractAttributes
{
    Q_OBJECT

public:
    CT_AbstractEdgeAttributes();
    CT_AbstractEdgeAttributes(const CT_OutAbstractItemModel *model,
                              const CT_AbstractResult *result,
                              CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR cir);

    CT_AbstractEdgeAttributes(const QString &modelName,
                              const CT_AbstractResult *result,
                              CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR cir);

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    QString getType() const;
    static QString staticGetType();

    inline const CT_AbstractEdgeCloudIndex* abstractCloudIndex() const { return getEdgeCloudIndex(); }

    /**
      * \brief Retourne les index du nuage de edge.
      */
    const CT_AbstractEdgeCloudIndex* getEdgeCloudIndex() const;

    /**
      * \brief Retourne les index du nuage de edge.
      */
    const CT_AbstractCloudIndexT<CT_Edge>* getEdgeCloudIndexT() const;

    /**
     * @brief EdgeCloudIndexRegistered getter
     * @return retourne le nuage de points enregistré
     */
    CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR getEdgeCloudIndexRegistered() const;

private:
    // TODO
    //static CT_StandardAbstractEdgeAttributesDrawManager AEA_DRAW_MANAGER;

    CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR m_cir;
    CT_AbstractEdgeCloudIndex* m_aci;
};

#endif // CT_ABSTRACTEDGEATTRIBUTES_H
