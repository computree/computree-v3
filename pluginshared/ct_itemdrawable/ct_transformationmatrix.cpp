#include "ct_transformationmatrix.h"

CT_DEFAULT_IA_INIT(CT_TransformationMatrix)

CT_TransformationMatrix::CT_TransformationMatrix() : CT_AbstractSingularItemDrawable()
{
    _transformationMatrix = Eigen::Matrix3f::Identity(3,3);
}

CT_TransformationMatrix::CT_TransformationMatrix(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, const Eigen::Matrix3f &trMat) : CT_AbstractSingularItemDrawable(model, result)
{
    _transformationMatrix = trMat;
}

CT_TransformationMatrix::CT_TransformationMatrix(const QString &modelName, const CT_AbstractResult *result, const Eigen::Matrix3f &trMat) : CT_AbstractSingularItemDrawable(modelName, result)
{
    _transformationMatrix = trMat;
}

QString CT_TransformationMatrix::getType() const
{
    return staticGetType();
}

QString CT_TransformationMatrix::staticGetType()
{
    return CT_AbstractSingularItemDrawable::staticGetType() + "/CT_TransformationMatrix";
}

QString CT_TransformationMatrix::getTransformationMatrixAsString() const
{
    return QString("%1\t%2\t%3\n%4\t%5\t%6\n%7\t%8\t%9")    .arg(QString::number(_transformationMatrix(0,0), 'f', 10))
                                                            .arg(QString::number(_transformationMatrix(0,1), 'f', 10))
                                                            .arg(QString::number(_transformationMatrix(0,2), 'f', 10))
                                                            .arg(QString::number(_transformationMatrix(1,0), 'f', 10))
                                                            .arg(QString::number(_transformationMatrix(1,1), 'f', 10))
                                                            .arg(QString::number(_transformationMatrix(1,2), 'f', 10))
                                                            .arg(QString::number(_transformationMatrix(2,0), 'f', 10))
                                                            .arg(QString::number(_transformationMatrix(2,1), 'f', 10))
                                                            .arg(QString::number(_transformationMatrix(2,2), 'f', 10));
}

CT_AbstractItemDrawable* CT_TransformationMatrix::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList)

    return new CT_TransformationMatrix((const CT_OutAbstractSingularItemModel *)model, result, _transformationMatrix);
}
