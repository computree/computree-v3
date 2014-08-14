#ifndef CT_EDGEATTRIBUTESNORMAL_H
#define CT_EDGEATTRIBUTESNORMAL_H

#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"
#include "ct_attributes/ct_attributesnormal.h"

class PLUGINSHAREDSHARED_EXPORT CT_EdgeAttributesNormal : public CT_AbstractEdgeAttributes, public CT_AttributesNormal
{
    Q_OBJECT

public:
    CT_EdgeAttributesNormal();

    CT_EdgeAttributesNormal(const CT_OutAbstractItemModel *model,
                            const CT_AbstractResult *result,
                            CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir);

    CT_EdgeAttributesNormal(const CT_OutAbstractItemModel *model,
                            const CT_AbstractResult *result,
                            CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir,
                            CT_AbstractNormalCloud *nc);

    CT_EdgeAttributesNormal(const QString &modelName,
                            const CT_AbstractResult *result,
                            CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir);


    CT_EdgeAttributesNormal(const QString &modelName,
                            const CT_AbstractResult *result,
                            CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir,
                            CT_AbstractNormalCloud *nc);

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    QString getType() const;
    static QString staticGetType();

    size_t attributesSize() const { return CT_AttributesNormal::attributesSize(); }

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_EDGEATTRIBUTESNORMAL_H
