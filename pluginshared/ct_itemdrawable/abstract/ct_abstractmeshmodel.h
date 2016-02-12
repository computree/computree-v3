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

    CT_AbstractMeshModel(const CT_OutAbstractSingularItemModel *model,
                         const CT_AbstractResult *result);

    CT_AbstractMeshModel(const QString &modelName,
                         const CT_AbstractResult *result);


    /**
      * WARNING : don't forget to redefined this methods if you inherit this class
      */
    virtual QString getType() const;
    static QString staticGetType();

    /**
     * @brief Draw only faces
     */
    virtual void drawFaces(GraphicsViewInterface &view, PainterInterface &painter) = 0;

    /**
     * @brief Begin draw multiple faces
     */
    virtual void beginDrawMultipleFace(GraphicsViewInterface &view, PainterInterface &painter) = 0;

    /**
     * @brief Draw only face at index 'index'
     */
    virtual void drawFaceAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter) = 0;

    /**
     * @brief Finish draw multiple faces
     */
    virtual void endDrawMultipleFace(GraphicsViewInterface &view, PainterInterface &painter) = 0;

    /**
     * @brief Draw only edges
     */
    virtual void drawEdges(GraphicsViewInterface &view, PainterInterface &painter) = 0;

    /**
     * @brief Begin draw multiple edges
     */
    virtual void beginDrawMultipleEdge(GraphicsViewInterface &view, PainterInterface &painter) = 0;

    /**
     * @brief Draw only edge at index 'index'
     */
    virtual void drawEdgeAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter) = 0;

    /**
     * @brief Finish draw multiple edges
     */
    virtual void endDrawMultipleEdge(GraphicsViewInterface &view, PainterInterface &painter) = 0;
};

#endif // CT_ABSTRACTMESHMODEL_H
