#include "ct_triangulation2d.h"

const CT_StandardTriangulation2DDrawManager CT_Triangulation2D::TRIANGULATION2D_DRAW_MANAGER;

CT_Triangulation2D::CT_Triangulation2D() : CT_AbstractItemDrawableWithoutPointCloud()
{
    _delaunay = NULL;
    setBaseDrawManager(&TRIANGULATION2D_DRAW_MANAGER);
}

CT_Triangulation2D::CT_Triangulation2D(const CT_OutAbstractSingularItemModel *model,
                                       const CT_AbstractResult *result,
                                       CT_DelaunayTriangulation *delaunay) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _delaunay = delaunay;
    setBaseDrawManager(&TRIANGULATION2D_DRAW_MANAGER);

    if (_delaunay != NULL)
    {
        _minCoordinates(0) =  delaunay->getMinX();
        _minCoordinates(1) =  delaunay->getMinY();
        _minCoordinates(2) =  0;

        _maxCoordinates(0) =  delaunay->getMaxX();
        _maxCoordinates(1) =  delaunay->getMaxY();
        _maxCoordinates(2) =  0;
    }

}

CT_Triangulation2D::CT_Triangulation2D(const QString &modelName,
                                       const CT_AbstractResult *result,
                                       CT_DelaunayTriangulation *delaunay) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _delaunay = delaunay;
    setBaseDrawManager(&TRIANGULATION2D_DRAW_MANAGER);

    if (_delaunay != NULL)
    {
        _minCoordinates(0) =  delaunay->getMinX();
        _minCoordinates(1) =  delaunay->getMinY();
        _minCoordinates(2) =  0;

        _maxCoordinates(0) =  delaunay->getMaxX();
        _maxCoordinates(1) =  delaunay->getMaxY();
        _maxCoordinates(2) =  0;
    }
}

CT_Triangulation2D::~CT_Triangulation2D()
{
    delete _delaunay;
}

CT_DelaunayTriangulation *CT_Triangulation2D::getDelaunayT() const
{
    return _delaunay;
}

CT_AbstractItemDrawable* CT_Triangulation2D::copy(const CT_OutAbstractItemModel *model,
                                                  const CT_AbstractResult *result,
                                                  CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_Triangulation2D *t2d = new CT_Triangulation2D((const CT_OutAbstractSingularItemModel *)model, result, NULL);
    t2d->setId(id());

    if(_delaunay != NULL)
        t2d->_delaunay = _delaunay->copy();

    t2d->setAlternativeDrawManager(getAlternativeDrawManager());

    return t2d;
}
