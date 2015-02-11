#ifndef CT_TRANSFORMATIONMATRIX_H
#define CT_TRANSFORMATIONMATRIX_H

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include <eigen/Eigen/Core>

/**
 * @brief A singular item that contains item attributes
 */
class PLUGINSHAREDSHARED_EXPORT CT_TransformationMatrix : public CT_AbstractSingularItemDrawable
{
    // IMPORTANT to have the name of the ItemDrawable in the view
    Q_OBJECT

public:
    CT_TransformationMatrix();

    CT_TransformationMatrix(const CT_OutAbstractSingularItemModel *model,
                         const CT_AbstractResult *result, const Eigen::Matrix4d &trMat);

    CT_TransformationMatrix(const QString &modelName,
                         const CT_AbstractResult *result, const Eigen::Matrix4d &trMat);

    /**
      * WARNING : don't forget to redefined this methods if you inherit this class
      */
    virtual QString getType() const;
    static QString staticGetType();

    inline const Eigen::Matrix4d& getTransformationMatrix() const {return _transformationMatrix;}
    QString getTransformationMatrixAsString() const;

    void transform(Eigen::Vector3d &vec) const;

    /**
     * @brief Returns a copy of this item
     */
    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

protected:
    Eigen::Matrix4d     _transformationMatrix;

private:

    CT_DEFAULT_IA_BEGIN(CT_TransformationMatrix)
    CT_DEFAULT_IA_V2(CT_TransformationMatrix, CT_AbstractCategory::staticInitDataValue(), &CT_TransformationMatrix::getTransformationMatrixAsString, QObject::tr("Matrice de transformation"))
    CT_DEFAULT_IA_END(CT_TransformationMatrix)

};

#endif // CT_TRANSFORMATIONMATRIX_H
