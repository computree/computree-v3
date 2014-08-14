#ifndef CT_ABSTRACTMESHMODEL_H
#define CT_ABSTRACTMESHMODEL_H

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include "ct_itemdrawable/accessibility/ct_iaccesspointcloud.h"
#include "ct_itemdrawable/accessibility/ct_iaccessfacecloud.h"
#include "ct_itemdrawable/accessibility/ct_iaccessedgecloud.h"
/**
 * @brief A singular item that can manage a mesh
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractMeshModel : public CT_AbstractSingularItemDrawable, public CT_IAccessPointCloud, public CT_IAccessFaceCloud, public CT_IAccessEdgeCloud
{
    // IMPORTANT to have the name of the ItemDrawable in the view
    Q_OBJECT

public:
    CT_AbstractMeshModel();

    CT_AbstractMeshModel(const CT_OutAbstractItemModel *model,
                         const CT_AbstractResult *result);

    CT_AbstractMeshModel(const QString &modelName,
                         const CT_AbstractResult *result);


    /**
      * WARNING : don't forget to redefined this methods if you inherit this class
      */
    virtual QString getType() const;
    static QString staticGetType();
};

#endif // CT_ABSTRACTMESHMODEL_H
