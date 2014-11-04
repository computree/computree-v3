#ifndef CT_ABSTRACTPOINTSATTRIBUTESCONTAINER_H
#define CT_ABSTRACTPOINTSATTRIBUTESCONTAINER_H

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractPointsAttributesContainer : public CT_AbstractSingularItemDrawable
{
    Q_OBJECT

public:
    CT_AbstractPointsAttributesContainer();

    CT_AbstractPointsAttributesContainer(const CT_OutAbstractSingularItemModel *model,
                                         const CT_AbstractResult *result);

    CT_AbstractPointsAttributesContainer(const QString &modelName,
                                         const CT_AbstractResult *result);

    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const = 0;
    static QString staticGetType();

    /**
     * @brief Returns the CT_AbstractPointAttributesScalar at key defined in parameter
     */
    CT_AbstractPointsAttributes* pointsAttributesAt(const int &key) const;

private:
    QHash<int, CT_AbstractPointsAttributes*>   m_att;

protected:

    /**
     * @brief Insert a CT_AbstractPointsAttributes at key in the map
     */
    void internalInsertAttributes(const int &key, const CT_AbstractPointsAttributes *att);
};

#endif // CT_ABSTRACTPOINTSATTRIBUTESCONTAINER_H
