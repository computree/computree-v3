#ifndef CT_ABSTRACTPOINTATTRIBUTESSCALAR_H
#define CT_ABSTRACTPOINTATTRIBUTESSCALAR_H

#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractPointAttributesScalar : public CT_AbstractPointsAttributes
{
    Q_OBJECT

public:
    CT_AbstractPointAttributesScalar();
    CT_AbstractPointAttributesScalar(const CT_OutAbstractSingularItemModel *model,
                                     const CT_AbstractResult *result,
                                     CT_PCIR pcir);

    CT_AbstractPointAttributesScalar(const QString &modelName,
                                     const CT_AbstractResult *result,
                                     CT_PCIR pcir);

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    QString getType() const;
    static QString staticGetType();

    virtual double dMin() const = 0;
    virtual double dMax() const = 0;

    virtual double dValueAt(const size_t &index) const = 0;

    virtual size_t attributesSize() const = 0;
};

#endif // CT_ABSTRACTPOINTATTRIBUTESSCALAR_H
