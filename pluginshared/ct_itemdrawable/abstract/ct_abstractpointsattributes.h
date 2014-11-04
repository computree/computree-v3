#ifndef CT_ABSTRACTPOINTSATTRIBUTES_H
#define CT_ABSTRACTPOINTSATTRIBUTES_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractpointsattributesdrawmanager.h"
#include "ct_itemdrawable/accessibility/ct_iaccesspointcloud.h"

#include "ct_pointcloud/abstract/ct_abstractpointcloud.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"

#include "ct_itemdrawable/abstract/ct_abstractattributes.h"

/**
 * @brief Represents a cloud attribute for points.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractPointsAttributes : public CT_AbstractAttributes, public CT_IAccessPointCloud
{
    Q_OBJECT

public:
    CT_AbstractPointsAttributes();
    CT_AbstractPointsAttributes(const CT_OutAbstractSingularItemModel *model,
                                const CT_AbstractResult *result,
                                CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir);

    CT_AbstractPointsAttributes(const QString &modelName,
                                const CT_AbstractResult *result,
                                CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir);

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    QString getType() const;
    static QString staticGetType();

    inline const CT_AbstractPointCloudIndex* abstractCloudIndex() const { return getPointCloudIndex(); }

    /**
      * \brief Retourne le nuage de points.
      */
    const CT_AbstractPointCloud* getPointCloud() const;

    /**
      * \brief Retourne les index du nuage de points.
      */
    virtual const CT_AbstractPointCloudIndex* getPointCloudIndex() const;

    /**
      * \brief Retourne les index du nuage de points.
      */
    virtual const CT_AbstractCloudIndexT<CT_Point>* getPointCloudIndexT() const;

    /**
     * @brief PointCloudIndexRegistered getter
     * @return retourne le nuage de points enregistré
     */
    virtual CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR getPointCloudIndexRegistered() const;

    /**
      * \brief Retourne le nombre de point  sauter lorsqu'il faut dessiner rapidement (par dfaut 16).
      */
    virtual int getFastestIncrement() const;

private:
    static CT_StandardAbstractPointsAttributesDrawManager APA_DRAW_MANAGER;

    CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR m_pcir;
    CT_AbstractPointCloudIndex* m_apci;
};

#endif // CT_ABSTRACTPOINTSATTRIBUTES_H
