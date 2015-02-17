#ifndef CT_ABSTRACTFACEATTRIBUTESSCALAR_H
#define CT_ABSTRACTFACEATTRIBUTESSCALAR_H

#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractFaceAttributesScalar : public CT_AbstractFaceAttributes
{
    Q_OBJECT

public:
    CT_AbstractFaceAttributesScalar();
    CT_AbstractFaceAttributesScalar(const CT_OutAbstractSingularItemModel *model,
                                    CT_AbstractResult *result,
                                    CT_FCIR cir);

    CT_AbstractFaceAttributesScalar(const QString &modelName,
                                    CT_AbstractResult *result,
                                    CT_FCIR cir);


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

#endif // CT_ABSTRACTFACEATTRIBUTESSCALAR_H
