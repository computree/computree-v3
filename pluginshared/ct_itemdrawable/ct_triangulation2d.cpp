#include "ct_triangulation2d.h"

#include "ct_triangulation/ct_trianglet.h"
#include "ct_triangulation/ct_edget.h"
#include "ct_triangulation/ct_nodet.h"

const CT_StandardTriangulation2DDrawManager CT_Triangulation2D::TRIANGULATION2D_DRAW_MANAGER;

CT_Triangulation2D::CT_Triangulation2D() : CT_AbstractItemDrawableWithoutPointCloud()
{
    _delaunay = NULL;
    setBaseDrawManager(&TRIANGULATION2D_DRAW_MANAGER);
}

CT_Triangulation2D::CT_Triangulation2D(const CT_OutAbstractItemModel *model,
                                       const CT_AbstractResult *result,
                                       CT_DelaunayT *delaunay) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _delaunay = delaunay;
    setBaseDrawManager(&TRIANGULATION2D_DRAW_MANAGER);
}

CT_Triangulation2D::CT_Triangulation2D(const QString &modelName,
                                       const CT_AbstractResult *result,
                                       CT_DelaunayT *delaunay) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _delaunay = delaunay;
    setBaseDrawManager(&TRIANGULATION2D_DRAW_MANAGER);
}

CT_Triangulation2D::~CT_Triangulation2D()
{
    delete _delaunay;
}

QString CT_Triangulation2D::getType() const
{
    return staticGetType();
}

QString CT_Triangulation2D::staticGetType()
{
    return CT_AbstractItemDrawableWithoutPointCloud::staticGetType() + "/CT_Triangulation2D";
}

CT_DelaunayT* CT_Triangulation2D::getDelaunayT() const
{
    return _delaunay;
}

CT_AbstractItemDrawable* CT_Triangulation2D::copy(const CT_OutAbstractItemModel *model,
                                                  const CT_AbstractResult *result,
                                                  CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Triangulation2D *t2d = new CT_Triangulation2D(model, result, NULL);
    t2d->setId(id());

    if(_delaunay != NULL)
        t2d->_delaunay = _delaunay->copy();

    t2d->setAlternativeDrawManager(getAlternativeDrawManager());

    return t2d;
}
