#ifndef CT_ABSTRACTPOINTSATTRIBUTES_H
#define CT_ABSTRACTPOINTSATTRIBUTES_H

#include "ct_defines.h"

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractpointsattributesdrawmanager.h"
#include "ct_itemdrawable/accessibility/ct_iaccesspointcloud.h"

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"

#include "ct_itemdrawable/abstract/ct_abstractattributes.h"

/**
 * @brief Represents a cloud attribute for points.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractPointsAttributes : public CT_AbstractAttributes, public CT_IAccessPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractPointsAttributes, CT_AbstractAttributes, Point attributes)

public:
    CT_AbstractPointsAttributes();
    CT_AbstractPointsAttributes(const CT_OutAbstractSingularItemModel *model,
                                const CT_AbstractResult *result,
                                CT_PCIR pcir);

    CT_AbstractPointsAttributes(const QString &modelName,
                                const CT_AbstractResult *result,
                                CT_PCIR pcir);

    inline const CT_AbstractPointCloudIndex* abstractCloudIndex() const { return getPointCloudIndex(); }

    /**
      * \brief Retourne les index du nuage de points.
      */
    virtual const CT_AbstractPointCloudIndex* getPointCloudIndex() const;

    /**
     * @brief PointCloudIndexRegistered getter
     * @return retourne le nuage de points enregistré
     */
    virtual CT_PCIR getPointCloudIndexRegistered() const;

    /**
      * \brief Retourne le nombre de point  sauter lorsqu'il faut dessiner rapidement (par dfaut 16).
      */
    virtual int getFastestIncrement() const;


private:
    static CT_StandardAbstractPointsAttributesDrawManager APA_DRAW_MANAGER;

    CT_PCIR                     m_pcir;
    CT_AbstractPointCloudIndex* m_apci;
};

#endif // CT_ABSTRACTPOINTSATTRIBUTES_H
